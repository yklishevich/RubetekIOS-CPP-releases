#pragma once

#include <boost/asio/ssl.hpp>

#include <msw/assert.hpp>
#include <msw/std/memory.hpp>
#include <msw/asio/tcp_socket.hpp>
#include <msw/asio/ssl_context.hpp>

#include <lama/openssl/include/autolink.hpp>


namespace msw
{
    struct ssl_socket
        : noncopyable
    {
        typedef boost::asio::ssl::stream<boost::asio::ip::tcp::socket> native_socket     ;
        typedef std::unique_ptr<native_socket>                         native_socket_ptr ;
        typedef tcp_socket::endpoint_type                              endpoint_type     ;
        typedef boost::asio::ssl::stream_base::handshake_type          handshake_type    ;
        typedef boost::asio::io_service                                io_service        ;

                             ssl_socket          (tcp_socket&&, ssl_context&)          ;
                             ssl_socket          (ssl_socket&&)                        ;

        ssl_socket&          operator =          (ssl_socket&&)                        ;

        template <typename Handler>
        void                 async_handshake     (handshake_type, Handler&&)           ;
        template <typename Buffer, typename Handler>
        void                 async_receive       (Buffer&&, Handler&&)                 ;
        template <typename Buffer, typename Handler>
        void                 async_send          (Buffer&&, Handler&&)                 ;

        io_service&          get_io_service      ()                                    ;

        void                 close               ()                                    ;

        endpoint_type        local_endpoint      ()                              const ;
        endpoint_type        remote_endpoint     ()                              const ;

        void                 set_verify_mode     (boost::asio::ssl::verify_mode)       ;
        template <typename VerifyCallback>
        void                 set_verify_callback (VerifyCallback&&)                    ;

        native_socket_ptr    eject_native_socket ()                                    ;

    private:

        native_socket&       socket              ()                                    ;
        native_socket const& socket              ()                              const ;

        native_socket_ptr socket_;
    };
}


namespace msw
{
    inline ssl_socket::ssl_socket(tcp_socket&& tcp_socket, ssl_context& context)
        : socket_(msw::make_unique<native_socket>(tcp_socket.get_io_service(), context))
    {
        socket().lowest_layer() = std::move(tcp_socket);
        //socket().next_layer() = std::move(tcp_socket);
    }
    inline ssl_socket::ssl_socket(ssl_socket&& other)
        : socket_(std::move(other.socket_))
    {}

    inline ssl_socket& ssl_socket::operator = (ssl_socket&& other)
    {
        socket_ = std::move(other.socket_);
        return *this;
    }

    template <typename Handler>
    void ssl_socket::async_handshake(handshake_type type, Handler&& handler)
    {
        socket().async_handshake(type, std::forward<Handler>(handler));
    }
    template <typename Buffer, typename Handler>
    void ssl_socket::async_receive(Buffer&& buf, Handler&& handler)
    {
        socket().async_read_some(std::forward<Buffer>(buf), std::forward<Handler>(handler));
    }
    template <typename Buffer, typename Handler>
    void ssl_socket::async_send(Buffer&& buf, Handler&& handler)
    {
        socket().async_write_some(std::forward<Buffer>(buf), std::forward<Handler>(handler));
    }

    inline ssl_socket::io_service& ssl_socket::get_io_service()
    {
        return socket().get_io_service();
    }

    inline void ssl_socket::close()
    {
        if (socket_) socket().lowest_layer().close();//shutdown();
    }

    inline ssl_socket::endpoint_type ssl_socket::local_endpoint() const
    {
        return socket().lowest_layer().local_endpoint();
    }
    inline ssl_socket::endpoint_type ssl_socket::remote_endpoint() const
    {
        return socket().lowest_layer().remote_endpoint();
    }

    inline void ssl_socket::set_verify_mode(boost::asio::ssl::verify_mode mode)
    {
        socket().set_verify_mode(mode);
    }
    template <typename VerifyCallback>
    void ssl_socket::set_verify_callback(VerifyCallback&& callback)
    {
        socket().set_verify_callback(std::forward<VerifyCallback>(callback));
    }

    inline ssl_socket::native_socket_ptr ssl_socket::eject_native_socket()
    {
        return std::move(socket_);
    }

    inline ssl_socket::native_socket& ssl_socket::socket()
    {
        MSW_ASSERT(socket_);
        return *socket_;
    }
    inline ssl_socket::native_socket const& ssl_socket::socket() const
    {
        MSW_ASSERT(socket_);
        return *socket_;
    }
}
