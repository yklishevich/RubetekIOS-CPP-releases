#pragma once

#include <msw/size.hpp>
#include <msw/config.hpp>
#include <msw/nonassignable.hpp>
#include <msw/include/boost/asio.hpp>

#ifdef MSW_LINUX
#include <fcntl.h>
#endif

namespace msw
{
    enum class ip_dscp : int
    {
          cs0 = 0
        , cs6 = 192
    };

    template <typename Socket>
    struct socket_options
        : nonassignable
    {
        typedef Socket socket;

        explicit socket_options      (socket&)    ;
        void     receive_buffer_size (size<byte>) ;
        void     send_buffer_size    (size<byte>) ;
        void     broadcast           (bool)       ;
        void     linger              (bool)       ;
        void     reuse_address       (bool)       ;
        void     dscp                (ip_dscp)    ;
        void     cloexec             ()           ;
    private:
        socket& socket_;
    };

    template <typename Socket>
    socket_options<Socket> set_socket_option(Socket&);
}


namespace msw
{
    template <typename Socket>
    socket_options<Socket>::socket_options(socket& socket)
        : socket_(socket)
    {}
    template <typename Socket>
    void socket_options<Socket>::receive_buffer_size(size<byte> value)
    {
        socket_.set_option(boost::asio::socket_base::receive_buffer_size(static_cast<unsigned>(value.count())));
    }
    template <typename Socket>
    void socket_options<Socket>::send_buffer_size(size<byte> value)
    {
        socket_.set_option(boost::asio::socket_base::send_buffer_size(static_cast<unsigned>(value.count())));
    }
    template <typename Socket>
    void socket_options<Socket>::broadcast(bool enable)
    {
        socket_.set_option(boost::asio::socket_base::broadcast(enable));
    }
    template <typename Socket>
    void socket_options<Socket>::linger(bool enable)
    {
#if !defined(MSW_APPLE) && !defined(MSW_CLANG)
        socket_.set_option(boost::asio::socket_base::linger(enable));
#endif
    }
    template <typename Socket>
    void socket_options<Socket>::reuse_address(bool enable)
    {
        socket_.set_option(boost::asio::socket_base::reuse_address(enable));
    }
    template <typename Socket>
    void socket_options<Socket>::dscp(ip_dscp iptos)
    {
#ifdef MSW_LINUX
        setsockopt(socket_.native_handle(), IPPROTO_IP, IP_TOS,  &iptos, sizeof(iptos));
#else
        iptos;
#endif
    }

    template <typename Socket>
    void socket_options<Socket>::cloexec()
    {
#ifdef MSW_LINUX
        fcntl(socket_.native_handle(), F_SETFD, fcntl(socket_.native_handle(), F_GETFD) | FD_CLOEXEC);
#endif
    }

    template <typename Socket>
    socket_options<Socket> set_socket_option(Socket& socket)
    {
        return socket_options<Socket>(socket);
    }
}
