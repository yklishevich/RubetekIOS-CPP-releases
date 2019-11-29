#pragma once

#include <boost/optional.hpp>

#include <msw/std/memory.hpp>
#include <msw/noncopyable.hpp>
#include <msw/asio/udp_point.hpp>
#include <msw/lexical_cast/bool2str.hpp>
#include <msw/lexical_cast/network/endpoint/to_string.hpp>

#include <rubetek/log.hpp>


namespace rubetek
{
    struct udp_socket
        : msw::noncopyable
    {
        typedef msw::udp_point::io_service    io_service    ;
        typedef msw::udp_point::endpoint_type endpoint_type ;
        typedef msw::udp_point::on_received   on_received   ;

                    udp_socket       (io_service&, msw::wbyte port, on_received)  ;

        void        enable_broadcast (bool = true)                                ;
        void        send_to          (endpoint_type, msw::range<msw::byte const>) ;
        io_service& get_io_service   () const                                     ;

    private:
        void        bind             ()                                           ;

        logger                          const logger_           ;
        logger_io<log_io_tags::udp>     const logger_io_        ;
        msw::wbyte                      const port_             ;
        on_received                     const on_received_      ;
        io_service&                           io_service_       ;
        boost::optional<bool>                 enable_broadcast_ ;
        std::unique_ptr<msw::udp_point>       udp_point_        ;
    };
}


namespace rubetek
{
    inline udp_socket::udp_socket(io_service& io_service, msw::wbyte port, on_received on_received)
        : logger_      ( "udp socket", msw::network_endpoint_to_string("0.0.0.0", port) )
        , logger_io_   ( logger_                                                        )
        , port_        ( port                                                           )
        , on_received_ ( on_received                                                    )
        , io_service_  ( io_service                                                     )
    {
        bind();
    }

    inline void udp_socket::enable_broadcast(bool enable)
    {
        logger_.info("enable broadcast: ", msw::yes_no(enable));
        enable_broadcast_ = enable;
        udp_point_->broadcast(enable);
    }
    inline void udp_socket::send_to(endpoint_type endpoint, msw::range<msw::byte const> packet)
    {
        boost::system::error_code ec;
        udp_point_->send_to(endpoint, packet, ec);
        if (ec)
        {
            logger_.error("send fail: ", ec.message());
            if (ec != boost::asio::error::broken_pipe) return;
            logger_.info("rebind");
            bind();
            udp_point_->send_to(endpoint, packet, ec);
            if (ec) return;
        }
        logger_io_.send(packet, endpoint);
    }
    inline udp_socket::io_service& udp_socket::get_io_service() const
    {
        return udp_point_->get_io_service();
    }

    inline void udp_socket::bind()
    {
        udp_point_.reset();
        udp_point_ = msw::make_unique<msw::udp_point>
        (
            io_service_
            , port_
            , [this](endpoint_type remote_endpoint, msw::range<msw::byte> packet)
            {
                logger_io_.received(packet, remote_endpoint);
                on_received_(remote_endpoint, packet);
            }
            , [this](boost::system::error_code ec)
            {
                logger_.error(ec.message());
            }
        );
        if (enable_broadcast_) udp_point_->broadcast(*enable_broadcast_);
    }
}
