#pragma once

#include <memory>
#include <functional>

#include <msw/assert.hpp>
#include <msw/buffer.hpp>
#include <msw/include/boost/asio.hpp>
#include <msw/asio/socket_options.hpp>
#include <msw/zzz/asio/error_handler.hpp>


namespace msw {
namespace zzz {

    struct udp_point
        : udp_sender
        , std::enable_shared_from_this<udp_point>
    {
        typedef udp_sender::endpoint_type                       endpoint_type ;
        typedef std::function<void(endpoint_type, range<byte>)> on_received   ;

                          udp_point           (boost::asio::io_service&, endpoint_type, on_received, on_error, bool reuse_address) ;

        void              start               ()                                                                                   ;
        void              cancel              ()                                                                                   ;

        using udp_sender::send_to                                                                                                  ;

        using udp_sender::close                                                                                                    ;

        using udp_sender::broadcast                                                                                                ;
        using udp_sender::send_buffer_size                                                                                         ;
        void              receive_buffer_size (size<byte>)                                                                         ;

        using udp_sender::get_io_service                                                                                           ;

    private:

        void              do_receive          ()                                                                                   ;

        buffer<byte>  received_packet_ ;
        endpoint_type remote_endpoint_ ;
        on_received   on_received_     ;
        error_handler error_handler_   ;
    };

}}


namespace msw {
namespace zzz {

    inline udp_point::udp_point(boost::asio::io_service& io_service, endpoint_type endpoint, on_received on_received, on_error on_error, bool reuse_address)
        : udp_sender       ( io_service, endpoint, reuse_address )
        , received_packet_ ( msw::KB * 64                        )
        , on_received_     ( on_received                         )
        , error_handler_   ( on_error                            )
    {}

    inline void udp_point::start()
    {
        do_receive();
    }
    inline void udp_point::cancel()
    {
        on_received_ = nullptr;
        close();
    }

    inline void udp_point::receive_buffer_size(size<byte> sz)
    {
        set_socket_option(socket_).receive_buffer_size(sz);
    }

    inline void udp_point::do_receive()
    {
        auto self(shared_from_this());
        socket_.async_receive_from
        (
            boost::asio::buffer(received_packet_.data().native(), received_packet_.size().count()), remote_endpoint_,
            [this, self](boost::system::error_code ec, std::size_t bytes_received)
            {
                if(on_received_)
                {
                    if (ec)
                    {
                        if (ec == boost::system::errc::connection_reset)
                        {
                            (error_handler(error_handler_))(ec);
                            if (on_received_) do_receive();
                        }
                        else if (ec != boost::system::errc::operation_canceled) (error_handler(error_handler_))(ec);
                    }
                    else
                    {
                        (on_received(on_received_))(remote_endpoint_, received_packet_.front(bytes_received));
                        if (on_received_) do_receive();
                    }
                }
            }
        );
    }

}}
