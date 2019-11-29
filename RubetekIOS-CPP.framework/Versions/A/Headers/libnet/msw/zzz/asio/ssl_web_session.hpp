#pragma once

#include <deque>
#include <memory>
#include <functional>

#include <beast/core/to_string.hpp>

#include <msw/int.hpp>
#include <msw/range.hpp>
#include <msw/config.hpp>
#include <msw/assert.hpp>
#include <msw/buffer.hpp>
#include <msw/noncopyable.hpp>
#include <msw/plain_types.hpp>
#include <msw/include/boost/asio.hpp>
#include <msw/asio/ssl_web_socket.hpp>

#include <msw/zzz/asio/session.hpp>
#include <msw/zzz/asio/error_handler.hpp>
#include <msw/throw_runtime_error.hpp>


namespace msw {
namespace zzz {

    template <>
    struct session<ssl_web_socket>
        : noncopyable
        , std::enable_shared_from_this<session<ssl_web_socket>>
    {
        typedef ssl_web_socket                   socket        ;
        typedef std::function<void(range<byte>)> on_received   ;
        typedef std::function<void(range<byte>)> on_sent       ;
        typedef boost::asio::ip::tcp::endpoint   endpoint_type ;

                      session             (socket&&, on_received, on_sent, on_error)              ;

        void          start               ()                                                      ;
        void          cancel              ()                                                      ;

        void          send                (range<byte const>)                                     ;

        uint          queue_packets_count ()                                              const   ;
        size<byte>    queue_packets_size  ()                                              const   ;

        endpoint_type local_endpoint      ()                                              const   ;
        endpoint_type remote_endpoint     ()                                              const   ;

        template <typename Handler>
        void          eject_socket        (Handler)                                               ;

    private:

        void          do_receive          ()                                                      ;
        void          do_send             ()                                                      ;
        void          send_stream         ()                                                      ;
        void          stop                ()                                                      ;

        socket                   socket_          ;
        beast::streambuf         received_stream_ ;
        socket::opcode           opcode_          ;
        buffer<byte>             send_stream_     ;
        std::deque<buffer<byte>> deque_packets_   ;
        on_received              on_received_     ;
        on_sent                  on_sent_         ;
        error_handler            error_handler_   ;
    };

}}


namespace msw {
namespace zzz {

    inline session<ssl_web_socket>::session(socket&& socket, on_received on_received, on_sent on_sent, on_error on_error)
        : socket_        ( std::move(socket) )
        , on_received_   ( on_received       )
        , on_sent_       ( on_sent           )
        , error_handler_ ( on_error          )
    {}

    inline void session<ssl_web_socket>::start()
    {
        do_receive();
    }
    inline void session<ssl_web_socket>::cancel()
    {
        stop();
        socket_.close();
    }

    inline void session<ssl_web_socket>::send(range<byte const> packet)
    {
        auto self(MSW_SHARED_FROM_THIS);
        buffer<byte> pkt(packet);
        socket_.get_io_service().post
        (
            [this, self, pkt]()
            {
                bool send_in_progress = !deque_packets_.empty();
                deque_packets_.push_back(std::move(pkt));
                if (!send_in_progress) do_send();
            }
        );
    }

    inline uint session<ssl_web_socket>::queue_packets_count() const
    {
        return static_cast<uint>(deque_packets_.size());
    }

    inline size<byte> session<ssl_web_socket>::queue_packets_size() const
    {
        size<byte> total_size;
        for (auto const& packet : deque_packets_)
            total_size += packet.size();
        return total_size;
    }

    inline session<ssl_web_socket>::endpoint_type session<ssl_web_socket>::local_endpoint() const
    {
        //return socket_.local_endpoint();
        endpoint_type ep;
        try
        {
            ep = socket_.local_endpoint();
        }
        catch (std::exception const& e)
        {
            throw_runtime_error("###### local_endpoint fail: ", e.what());
        }
        return ep;
    }

    inline session<ssl_web_socket>::endpoint_type session<ssl_web_socket>::remote_endpoint() const
    {
        //return socket_.remote_endpoint();
        endpoint_type ep;
        try
        {
            ep = socket_.remote_endpoint();
        }
        catch (std::exception const& e)
        {
            throw_runtime_error("###### remote_endpoint fail: ", e.what());
        }
        return ep;
    }

    template <typename Handler>
    inline void session<ssl_web_socket>::eject_socket(Handler handler)
    {
        //socket_.cancel();
        stop();
        auto self(MSW_SHARED_FROM_THIS);
        socket_.get_io_service().post
        (
            [this, self, handler]()
            {
                handler(std::move(socket_));
            }
        );
    }

    inline void session<ssl_web_socket>::do_receive()
    {
        auto self(MSW_SHARED_FROM_THIS);
        socket_.async_receive
        (
            opcode_
            , received_stream_
            , [this, self](boost::system::error_code ec)
            {
                if (on_received_)
                {
                    if (ec) error_handler_(ec);
                    else
                    {
                        buffer<byte> pkt(make_range<byte>(beast::to_string(received_stream_.data())));
                        received_stream_ = beast::streambuf();
                        on_received_(pkt.all());
                        if (on_received_) do_receive();
                    }
                }
            }
        );
    }

    inline void session<ssl_web_socket>::do_send()
    {
        send_stream_ = std::move(deque_packets_.front());
        send_stream();
    }

    inline void session<ssl_web_socket>::send_stream()
    {
        auto self(MSW_SHARED_FROM_THIS);
        socket_.async_send
        (
            boost::asio::buffer(send_stream_.data().native(), send_stream_.size().count())
            , [this, self](boost::system::error_code ec)
            {
                if (on_received_)
                {
                    if (ec) error_handler_(ec);
                    else
                    {
                        deque_packets_.pop_front();
                        if (on_sent_) on_sent_(send_stream_.all());
                        if (!deque_packets_.empty()) do_send();
                    }
                }
            }
        );
    }

    inline void session<ssl_web_socket>::stop()
    {
        on_received_ = nullptr;
        on_sent_     = nullptr;
        deque_packets_.clear();
    }

}}
