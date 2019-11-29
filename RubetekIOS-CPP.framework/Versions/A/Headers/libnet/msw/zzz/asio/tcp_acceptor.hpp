#pragma once

#include <memory>
#include <functional>

#include <msw/assert.hpp>
#include <msw/noncopyable.hpp>
#include <msw/asio/tcp_socket.hpp>
#include <msw/include/boost/asio.hpp>
#include <msw/zzz/asio/error_handler.hpp>

#ifdef MSW_LINUX
#   include <fcntl.h>
#endif


namespace msw {
namespace zzz {

    struct tcp_acceptor
        : noncopyable
        , std::enable_shared_from_this<tcp_acceptor>
    {
        typedef std::function<void(tcp_socket&&)> on_accept  ;
        typedef boost::asio::io_service           io_service ;
        typedef boost::asio::ip::tcp::endpoint    endpoint   ;

             tcp_acceptor (io_service&, endpoint, on_accept, on_error) ;

        void start        ()                                           ;
        void cancel       ()                                           ;

    private:

        void do_accept    ()                                           ;

        boost::asio::ip::tcp::acceptor acceptor_      ;
        tcp_socket                     socket_        ;
        on_accept                      on_accept_     ;
        error_handler                  error_handler_ ;
    };

}}


namespace msw {
namespace zzz {

    inline tcp_acceptor::tcp_acceptor(io_service& io_service, endpoint endpoint, on_accept on_accept, on_error on_error)
        : acceptor_      ( io_service, endpoint )
        , socket_        ( io_service           )
        , on_accept_     ( on_accept            )
        , error_handler_ ( on_error             )
    {
#ifdef MSW_LINUX
        fcntl(acceptor_.native_handle(), F_SETFD, fcntl(acceptor_.native_handle(), F_GETFD) | FD_CLOEXEC);
#endif
    }

    inline void tcp_acceptor::start()
    {
        do_accept();
    }
    inline void tcp_acceptor::cancel()
    {
        on_accept_ = nullptr;
        acceptor_.close();
    }

    inline void tcp_acceptor::do_accept()
    {
        auto self(shared_from_this());
        acceptor_.async_accept
        (
            socket_
            , [this, self](boost::system::error_code ec)
            {
                if (on_accept_)
                {
                    if (ec) error_handler_(ec);
                    else
                    {
                        (on_accept(on_accept_))(std::move(socket_));
                        if (on_accept_) do_accept();
                    }
                }
            }
        );
    }

}}
