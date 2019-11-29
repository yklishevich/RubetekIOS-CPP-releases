#pragma once

#include <deque>
#include <memory>
#include <functional>

#include <msw/int.hpp>
#include <msw/range.hpp>
#include <msw/config.hpp>
#include <msw/assert.hpp>
#include <msw/buffer.hpp>
#include <msw/std/memory.hpp>
#include <msw/noncopyable.hpp>
#include <msw/plain_types.hpp>
#include <msw/asio/tcp_socket.hpp>
#include <msw/include/boost/asio.hpp>
#include <msw/zzz/asio/error_handler.hpp>


namespace msw {
namespace zzz {

    template <typename Socket>
    struct session
        : noncopyable
        , std::enable_shared_from_this<session<Socket>>
    {
        typedef Socket                           socket        ;
        typedef std::function<void(range<byte>)> on_received   ;
        typedef std::function<void(range<byte>)> on_sent       ;
        typedef boost::asio::ip::tcp::endpoint   endpoint_type ;

                      session             (socket&&, on_received, on_sent, on_error)       ;

        void          start               ()                                               ;
        void          cancel              ()                                               ;

        void          send                (range<byte const>)                              ;

        uint          queue_packets_count ()                                         const ;
        size<byte>    queue_packets_size  ()                                         const ;

        endpoint_type local_endpoint      ()                                         const ;
        endpoint_type remote_endpoint     ()                                         const ;
        endpoint_type remote_endpoint     (boost::system::error_code&)               const ;

        template <typename Handler>
        void          eject_socket        (Handler)                                        ;

    private:

        void          do_receive          ()                                               ;
        void          do_send             ()                                               ;
        void          send_stream         ()                                               ;
        void          stop                ()                                               ;

        socket                   socket_          ;
        buffer<byte>             received_stream_ ;
        buffer<byte>             send_stream_     ;
        std::deque<buffer<byte>> deque_packets_   ;
        on_received              on_received_     ;
        on_sent                  on_sent_         ;
        error_handler            error_handler_   ;
    };
}}


namespace msw {
namespace zzz {

    template <typename Socket>
    session<Socket>::session(socket&& socket, on_received on_received, on_sent on_sent, on_error on_error)
        : socket_          ( std::move(socket) )
        , received_stream_ ( msw::KB * 64      )
        , on_received_     ( on_received       )
        , on_sent_         ( on_sent           )
        , error_handler_   ( on_error          )
    {}

    template <typename Socket>
    void session<Socket>::start()
    {
        do_receive();
    }
    template <typename Socket>
    void session<Socket>::cancel()
    {
        stop();
        socket_.close();
    }

    template <typename Socket>
    void session<Socket>::send(range<byte const> packet)
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

    template <typename Socket>
    uint session<Socket>::queue_packets_count() const
    {
        return static_cast<uint>(deque_packets_.size());
    }
    template <typename Socket>
    size<byte> session<Socket>::queue_packets_size() const
    {
        size<byte> total_size;
        for (auto const& packet : deque_packets_)
            total_size += packet.size();
        return total_size;
    }

    template <typename Socket>
    typename session<Socket>::endpoint_type session<Socket>::local_endpoint() const
    {
        return socket_.local_endpoint();
    }
    template <typename Socket>
    typename session<Socket>::endpoint_type session<Socket>::remote_endpoint() const
    {
        return socket_.remote_endpoint();
    }
    template <typename Socket>
    typename session<Socket>::endpoint_type session<Socket>::remote_endpoint(boost::system::error_code& ec) const
    {
        return socket_.remote_endpoint(ec);
    }

    template <typename Socket>
    template <typename Handler>
    void session<Socket>::eject_socket(Handler handler)
    {
        socket_.cancel();
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

    template <typename Socket>
    void session<Socket>::do_receive()
    {
        auto self(MSW_SHARED_FROM_THIS);
        socket_.async_receive
        (
            boost::asio::buffer(received_stream_.data().native(), received_stream_.size().count())
            , [this, self](boost::system::error_code ec, std::size_t bytes_received)
            {
                if (on_received_)
                {
                    if (ec) error_handler_(ec);
                    else
                    {
                        on_received_(received_stream_.front(bytes_received));
                        if (on_received_) do_receive();
                    }
                }
            }
        );
    }
    template <typename Socket>
    void session<Socket>::do_send()
    {
        send_stream_ = std::move(deque_packets_.front());
        send_stream();
    }
    template <typename Socket>
    void session<Socket>::send_stream()
    {
        auto self(MSW_SHARED_FROM_THIS);
        socket_.async_send
        (
            boost::asio::buffer(send_stream_.data().native(), send_stream_.size().count())
            , [this, self](boost::system::error_code ec, std::size_t bytes_sended)
            {
                if (on_received_)
                {
                    if (ec) error_handler_(ec);
                    else
                    {
                        msw::size<msw::byte> const sent_len = bytes_sended;
                        if (send_stream_.size() == sent_len)
                        {
                            deque_packets_.pop_front();
                            if (on_sent_) on_sent_(send_stream_.all());
                            if (!deque_packets_.empty()) do_send();
                        }
                        else
                        {
                            if (on_sent_) on_sent_(send_stream_.front(sent_len));
                            send_stream_.erase_from_to_front(sent_len);
                            send_stream();
                        }
                    }
                }
            }
        );
    }
    template <typename Socket>
    void session<Socket>::stop()
    {
        on_received_ = nullptr;
        on_sent_     = nullptr;
        deque_packets_.clear();
    }

}}
