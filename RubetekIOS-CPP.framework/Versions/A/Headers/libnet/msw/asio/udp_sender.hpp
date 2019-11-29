#pragma once

#include <msw/range.hpp>
#include <msw/plain_types.hpp>          // byte
#include <msw/noncopyable.hpp>
#include <msw/include/boost/asio.hpp>
#include <msw/asio/socket_options.hpp>

namespace msw
{
    struct udp_sender
        : noncopyable
    {
        typedef boost::asio::io_service        io_service    ;
        typedef boost::asio::ip::udp::endpoint endpoint_type ;

        explicit      udp_sender       (io_service&)                                                         ;
                      udp_sender       (io_service&, endpoint_type local_endpoint)                           ;
                      udp_sender       (io_service&, endpoint_type local_endpoint, bool reuse_address)       ;

        void          send_to          (endpoint_type, range<byte const>)                                    ;
        void          send_to          (endpoint_type, range<byte const>, boost::system::error_code&)        ;

        void          close            ()                                                                    ;

        endpoint_type local_endpoint   ()                                                              const ;

        void          broadcast        (bool)                                                                ;
        void          send_buffer_size (size<byte>)                                                          ;

        io_service&   get_io_service   ()                                                                    ;

    protected:
        boost::asio::ip::udp::socket socket_;
    };
}


namespace msw
{
    inline udp_sender::udp_sender(io_service& io_service)
        : udp_sender(io_service, endpoint_type(boost::asio::ip::udp::v4(), 0))
    {}
    inline udp_sender::udp_sender(io_service& io_service, endpoint_type endpoint)
        : socket_(io_service, endpoint)
    {
        set_socket_option(socket_).cloexec();
    }
    inline udp_sender::udp_sender(io_service& io_service, endpoint_type endpoint, bool reuse_address)
        : socket_(io_service, endpoint.protocol())
    {
        set_socket_option(socket_).reuse_address(reuse_address);
        socket_.bind(endpoint);
        set_socket_option(socket_).cloexec();
    }

    inline void udp_sender::send_to(endpoint_type endpoint, range<byte const> packet)
    {
        socket_.send_to(boost::asio::buffer(packet.data().native(), packet.size().count()), endpoint);
    }
    inline void udp_sender::send_to(endpoint_type endpoint, range<byte const> packet, boost::system::error_code& ec)
    {
        socket_.send_to(boost::asio::buffer(packet.data().native(), packet.size().count()), endpoint, 0, ec);
    }

    inline void udp_sender::close()
    {
        socket_.close();
    }

    inline udp_sender::endpoint_type udp_sender::local_endpoint() const
    {
        return socket_.local_endpoint();
    }

    inline void udp_sender::broadcast(bool enable)
    {
        set_socket_option(socket_).broadcast(enable);
    }
    inline void udp_sender::send_buffer_size(size<byte> s)
    {
        set_socket_option(socket_).send_buffer_size(s);
    }

    inline udp_sender::io_service& udp_sender::get_io_service()
    {
        return socket_.get_io_service();
    }
}
