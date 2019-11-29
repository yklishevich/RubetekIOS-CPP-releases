#pragma once

#include <memory>

#include <msw/asio/tcp_socket.hpp>
#include <msw/zzz/asio/ssl_handshaker.hpp>


namespace msw
{
    struct ssl_handshaker
        : noncopyable
    {
        typedef zzz::ssl_handshaker::on_handshake_completed on_handshake_completed ;
        typedef zzz::ssl_handshaker::on_verify              on_verify              ;

        ssl_handshaker  (tcp_socket&&, ssl_context&, on_handshake_completed, on_verify = nullptr, on_error = nullptr) ;
        ~ssl_handshaker ()                                                                                            ;
    private:
        std::shared_ptr<zzz::ssl_handshaker> handshaker_;
    };
}


namespace msw
{
    inline ssl_handshaker::ssl_handshaker(tcp_socket&& tcp_socket, ssl_context& context, on_handshake_completed on_handshake_completed, on_verify on_verify, on_error on_error)
        : handshaker_(std::make_shared<zzz::ssl_handshaker>(std::move(tcp_socket), context, on_handshake_completed, on_verify, on_error))
    {
        handshaker_->start();
    }
    inline ssl_handshaker::~ssl_handshaker()
    {
        handshaker_->cancel();
    }
}
