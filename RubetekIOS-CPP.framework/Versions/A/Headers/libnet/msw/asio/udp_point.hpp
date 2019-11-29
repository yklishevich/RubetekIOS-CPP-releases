#pragma once

#include <memory>

#include <msw/noncopyable.hpp>
#include <msw/asio/udp_sender.hpp>
#include <msw/zzz/asio/udp_point.hpp>


namespace msw
{
    struct udp_point
        : noncopyable
    {
        typedef udp_sender::io_service                          io_service    ;
        typedef udp_sender::endpoint_type                       endpoint_type ;
        typedef std::function<void(endpoint_type, range<byte>)> on_received   ;

                    udp_point           (io_service&, wbyte port, on_received, on_error = nullptr, bool reuse_address = false)    ;
                    udp_point           (io_service&, endpoint_type, on_received, on_error = nullptr, bool reuse_address = false) ;

                    ~udp_point          ()                                                                                        ;

        void        send_to             (endpoint_type, range<byte const>)                                                        ;
        void        send_to             (endpoint_type, range<byte const>, boost::system::error_code&)                            ;

        void        broadcast           (bool)                                                                                    ;
        void        send_buffer_size    (size<byte>)                                                                              ;
        void        receive_buffer_size (size<byte>)                                                                              ;

        io_service& get_io_service      ()                                                                                        ;

    private:
        std::shared_ptr<zzz::udp_point> udp_point_;
    };
}


namespace msw
{
    inline udp_point::udp_point(io_service& io_service, wbyte port, on_received on_received, on_error on_error, bool reuse_address)
        : udp_point(io_service, endpoint_type(boost::asio::ip::udp::v4(), port), on_received, on_error, reuse_address)
    {
        udp_point_->start();
    }
    inline udp_point::udp_point(io_service& io_service, endpoint_type endpoint, on_received on_received, on_error on_error, bool reuse_address)
        : udp_point_(std::make_shared<zzz::udp_point>(io_service, endpoint, on_received, on_error, reuse_address))
    {}

    inline udp_point::~udp_point()
    {
        udp_point_->cancel();
    }

    inline void udp_point::send_to(endpoint_type endpoint, range<byte const> packet)
    {
        udp_point_->send_to(endpoint, packet);
    }
    inline void udp_point::send_to(endpoint_type endpoint, range<byte const> packet, boost::system::error_code& ec)
    {
        udp_point_->send_to(endpoint, packet, ec);
    }

    inline void udp_point::broadcast(bool enable)
    {
        udp_point_->broadcast(enable);
    }
    inline void udp_point::send_buffer_size(size<byte> sz)
    {
        udp_point_->send_buffer_size(sz);
    }
    inline void udp_point::receive_buffer_size(size<byte> sz)
    {
        udp_point_->receive_buffer_size(sz);
    }

    inline udp_sender::io_service& udp_point::get_io_service()
    {
        return udp_point_->get_io_service();
    }
}
