#pragma once

#include <string>

#include <msw/range.hpp>
#include <msw/plain_types.hpp>                               // byte, wbyte
#include <msw/asio/udp_sender.hpp>
#include <msw/lexical_cast/network/endpoint/from_string.hpp>


namespace msw
{
    struct udp_linked_sender
        : udp_sender
    {
        typedef boost::asio::ip::udp::endpoint endpoint_type;

                          udp_linked_sender (boost::asio::io_service&, endpoint_type)                           ;
                          udp_linked_sender (boost::asio::io_service&, std::string const& ip, wbyte port)       ;

        void              send              (range<byte const>)                                                 ;
        void              send              (range<byte const>, boost::system::error_code&)                     ;

        using udp_sender::send_to                                                                               ;

        using udp_sender::close                                                                                 ;

        using udp_sender::broadcast                                                                             ;
        using udp_sender::send_buffer_size                                                                      ;

        endpoint_type     endpoint          ()                                                            const ;

    private:
        endpoint_type endpoint_;
    };
}


namespace msw
{
    inline udp_linked_sender::udp_linked_sender(boost::asio::io_service& io_service, boost::asio::ip::udp::endpoint endpoint)
        : udp_sender ( io_service )
        , endpoint_  ( endpoint   )
    {}
    inline udp_linked_sender::udp_linked_sender(boost::asio::io_service& io_service, std::string const& ip, wbyte port)
        : udp_linked_sender(io_service, udp_endpoint_from_string(ip, port))
    {}

    inline void udp_linked_sender::send(range<byte const> packet)
    {
        send_to(endpoint_, packet);
    }
    inline void udp_linked_sender::send(range<byte const> packet, boost::system::error_code& ec)
    {
        send_to(endpoint_, packet, ec);
    }

    inline udp_linked_sender::endpoint_type udp_linked_sender::endpoint() const
    {
        return endpoint_;
    }
}
