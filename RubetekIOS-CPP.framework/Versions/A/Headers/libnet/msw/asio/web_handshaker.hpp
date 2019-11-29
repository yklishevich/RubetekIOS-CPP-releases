#pragma once

#include <memory>
#include <string>

#include <msw/asio/web_socket.hpp>
#include <msw/zzz/asio/web_handshaker.hpp>


namespace msw
{
    template <typename WebSocket>
    struct web_handshaker_tmpl
        : noncopyable
    {
        typedef          WebSocket                                 web_socket_type        ;
        typedef          zzz::web_handshaker_tmpl<web_socket_type> handshaker             ;
        typedef typename handshaker::on_handshake_completed        on_handshake_completed ;

        web_handshaker_tmpl  (web_socket_type&&, std::string const& host, std::string const& resource, on_handshake_completed, on_error = nullptr) ;
        ~web_handshaker_tmpl ()                                                                                                                    ;
    private:
        std::shared_ptr<handshaker> handshaker_;
    };

    typedef web_handshaker_tmpl<web_socket> web_handshaker;
}


namespace msw
{
    template <typename WebSocket>
    web_handshaker_tmpl<WebSocket>::web_handshaker_tmpl
    (
          web_socket_type&&      web_socket
        , std::string const&     host
        , std::string const&     resource
        , on_handshake_completed on_handshake_completed
        , on_error               on_error
    )
        : handshaker_(std::make_shared<handshaker>(std::move(web_socket), host, resource, on_handshake_completed, on_error))
    {
        handshaker_->start();
    }
    template <typename WebSocket>
    web_handshaker_tmpl<WebSocket>::~web_handshaker_tmpl()
    {
        handshaker_->cancel();
    }
}
