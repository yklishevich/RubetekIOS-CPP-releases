#pragma once

#include <string>

#include <boost/asio/ssl.hpp>

#include <beast/websocket/ssl.hpp>

#include <msw/asio/ssl_socket.hpp>
#include <msw/zzz/asio/web_socket.hpp>


namespace msw
{
    struct ssl_web_socket
        : noncopyable
    {
        typedef ssl_socket::native_socket                    ssl_native_socket     ;
        typedef ssl_socket::native_socket_ptr                ssl_native_socket_ptr ;
        typedef beast::websocket::stream<ssl_native_socket&> native_socket         ;

        typedef std::unique_ptr<native_socket>               native_socket_ptr     ;

        typedef boost::asio::ip::tcp::socket::endpoint_type  endpoint_type         ;
        typedef beast::websocket::opcode                     opcode                ;
        typedef boost::asio::io_service                      io_service            ;

        explicit      ssl_web_socket  (ssl_native_socket_ptr&&)                                               ;
                      ssl_web_socket  (ssl_web_socket&&)                                                      ;

        template <typename Handler>
        void          async_handshake (std::string const& host, std::string const& resource, Handler&&)       ;
        template <typename Buffer, typename Handler>
        void          async_receive   (opcode&, Buffer&&, Handler&&)                                          ;
        template <typename Buffer, typename Handler>
        void          async_send      (Buffer&&, Handler&&)                                                   ;

                    //async_ping, async_close

        io_service&   get_io_service  ()                                                                      ;

        void          close           ()                                                                      ;

        endpoint_type local_endpoint  ()                                                                const ;
        endpoint_type remote_endpoint ()                                                                const ;

    private:

        ssl_native_socket_ptr ssl_socket_ ;
        native_socket_ptr     socket_     ;
    };
}


namespace msw
{
    inline ssl_web_socket::ssl_web_socket(ssl_native_socket_ptr&& ptr)
        : ssl_socket_ ( std::move(ptr)                                )
        , socket_     ( msw::make_unique<native_socket>(*ssl_socket_) )
    {
        MSW_ASSERT(ssl_socket_);
    }
    inline ssl_web_socket::ssl_web_socket(ssl_web_socket&& other)
        : ssl_socket_ ( std::move(other.ssl_socket_) )
        , socket_     ( std::move(other.socket_)     )
    {
        MSW_ASSERT( ssl_socket_ );
        MSW_ASSERT( socket_     );
    }

    template <typename Handler>
    void ssl_web_socket::async_handshake(std::string const& host, std::string const& resource, Handler&& handler)
    {
        MSW_ASSERT(socket_);
        socket_->async_handshake(host, resource, std::forward<Handler>(handler));
    }
    template <typename Buffer, typename Handler>
    void ssl_web_socket::async_receive(opcode& oc, Buffer&& buf, Handler&& handler)
    {
        MSW_ASSERT(socket_);
        socket_->async_read(oc, std::forward<Buffer>(buf), std::forward<Handler>(handler));
    }
    template <typename Buffer, typename Handler>
    void ssl_web_socket::async_send(Buffer&& buf, Handler&& handler)
    {
        MSW_ASSERT(socket_);
        socket_->async_write(std::forward<Buffer>(buf), std::forward<Handler>(handler));
    }

    inline ssl_web_socket::io_service& ssl_web_socket::get_io_service()
    {
        MSW_ASSERT(socket_);
        return socket_->get_io_service();
    }

    inline void ssl_web_socket::close()
    {
        if (socket_) socket_->lowest_layer().close();
    }

    inline ssl_web_socket::endpoint_type ssl_web_socket::local_endpoint() const
    {
        MSW_ASSERT(socket_);
        return socket_->lowest_layer().local_endpoint();
    }
    inline ssl_web_socket::endpoint_type ssl_web_socket::remote_endpoint() const
    {
        MSW_ASSERT(socket_);
        return socket_->lowest_layer().remote_endpoint();
    }
}
