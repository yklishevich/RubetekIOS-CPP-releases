#pragma once

#include <string>

#include <msw/assert.hpp>
#include <msw/include/boost/asio.hpp>

#include <lama/beast/include/websocket.hpp>


namespace msw {
namespace zzz {

    template <typename TransportSocket>
    struct web_socket_tmpl
        : noncopyable
    {
        typedef TransportSocket                             transport_socket ;
        typedef beast::websocket::stream<transport_socket>  native_socket    ;
        typedef boost::asio::ip::tcp::socket::endpoint_type endpoint_type    ;
        typedef beast::websocket::opcode                    opcode           ;
        typedef boost::asio::io_service                     io_service       ;

        explicit         web_socket_tmpl (transport_socket&&)                                                    ;
                         web_socket_tmpl (web_socket_tmpl&&)                                                     ;

        web_socket_tmpl& operator =      (web_socket_tmpl&&)                                                     ;

        template <typename Handler>
        void             async_handshake (std::string const& host, std::string const& resource, Handler&&)       ;
        template <typename Buffer, typename Handler>
        void             async_receive   (opcode&, Buffer&&, Handler&&)                                          ;
        template <typename Buffer, typename Handler>
        void             async_send      (Buffer&&, Handler&&)                                                   ;

                         //async_ping, async_close

        io_service&      get_io_service  ()                                                                      ;

        void             close           ()                                                                      ;

        endpoint_type    local_endpoint  ()                                                                const ;
        endpoint_type    remote_endpoint ()                                                                const ;

    private:

        native_socket socket_;
    };

}}


namespace msw {
namespace zzz {

    template <typename TransportSocket>
    web_socket_tmpl<TransportSocket>::web_socket_tmpl(transport_socket&& transport_socket)
        : socket_(std::move(transport_socket))
    {}
    template <typename TransportSocket>
    web_socket_tmpl<TransportSocket>::web_socket_tmpl(web_socket_tmpl&& other)
        : socket_(std::move(other.socket_))
    {}

    template <typename TransportSocket>
    web_socket_tmpl<TransportSocket>& web_socket_tmpl<TransportSocket>::operator = (web_socket_tmpl&& other)
    {
        socket_ = std::move(other.socket_);
        return *this;
    }

    template <typename TransportSocket>
    template <typename Handler>
    void web_socket_tmpl<TransportSocket>::async_handshake(std::string const& host, std::string const& resource, Handler&& handler)
    {
        socket_.async_handshake(host, resource, std::forward<Handler>(handler));
    }
    template <typename TransportSocket>
    template <typename Buffer, typename Handler>
    void web_socket_tmpl<TransportSocket>::async_receive(opcode& oc, Buffer&& buf, Handler&& handler)
    {
        socket_.async_read(oc, std::forward<Buffer>(buf), std::forward<Handler>(handler));
    }
    template <typename TransportSocket>
    template <typename Buffer, typename Handler>
    void web_socket_tmpl<TransportSocket>::async_send(Buffer&& buf, Handler&& handler)
    {
        socket_.async_write(std::forward<Buffer>(buf), std::forward<Handler>(handler));
    }

    template <typename TransportSocket>
    typename web_socket_tmpl<TransportSocket>::io_service& web_socket_tmpl<TransportSocket>::get_io_service()
    {
        return socket_.get_io_service();
    }

    template <typename TransportSocket>
    void web_socket_tmpl<TransportSocket>::close()
    {
        socket_.lowest_layer().close();
    }

    template <typename TransportSocket>
    typename web_socket_tmpl<TransportSocket>::endpoint_type web_socket_tmpl<TransportSocket>::local_endpoint() const
    {
        return socket_.lowest_layer().local_endpoint();
    }
    template <typename TransportSocket>
    typename web_socket_tmpl<TransportSocket>::endpoint_type web_socket_tmpl<TransportSocket>::remote_endpoint() const
    {
        return socket_.lowest_layer().remote_endpoint();
    }

}}
