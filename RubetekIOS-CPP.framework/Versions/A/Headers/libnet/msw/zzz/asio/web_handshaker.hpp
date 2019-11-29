#pragma once

#include <string>
#include <memory>
#include <functional>

#include <msw/assert.hpp>
#include <msw/std/memory.hpp>
#include <msw/noncopyable.hpp>
#include <msw/asio/web_socket.hpp>
#include <msw/include/boost/asio.hpp>


namespace msw {
namespace zzz {

    template <typename WebSocket>
    struct web_handshaker_tmpl
        : noncopyable
        , std::enable_shared_from_this<web_handshaker_tmpl<WebSocket>>
    {
        typedef WebSocket                            web_socket_type        ;
        typedef std::function<void(web_socket_type)> on_handshake_completed ;

                web_handshaker_tmpl (web_socket_type&&, std::string const& host, std::string const& resource, on_handshake_completed, on_error) ;
        void    start               ()                                                                                                          ;
        void    cancel              ()                                                                                                          ;

    private:

        web_socket_type              web_socket_             ;
        std::string            const host_                   ;
        std::string            const resource_               ;
        on_handshake_completed       on_handshake_completed_ ;
        error_handler                error_handler_          ;
    };

}}


namespace msw {
namespace zzz {

    template <typename WebSocket>
    web_handshaker_tmpl<WebSocket>::web_handshaker_tmpl
    (
          web_socket_type&&      web_socket
        , std::string const&     host
        , std::string const&     resource
        , on_handshake_completed on_handshake_completed
        , on_error               on_error
    )
        : web_socket_             ( std::move(web_socket)  )
        , host_                   ( host                   )
        , resource_               ( resource               )
        , on_handshake_completed_ ( on_handshake_completed )
        , error_handler_          ( on_error               )
    {}
    template <typename WebSocket>
    void web_handshaker_tmpl<WebSocket>::start()
    {
        auto self(MSW_SHARED_FROM_THIS);
        web_socket_.async_handshake
        (
            host_
            , resource_
            , [this, self](boost::system::error_code ec)
            {
                if (on_handshake_completed_)
                {
                    if (ec) error_handler_(ec);
                    else (on_handshake_completed(on_handshake_completed_))(std::move(web_socket_));
                }
            }
        );
    }
    template <typename WebSocket>
    void web_handshaker_tmpl<WebSocket>::cancel()
    {
        on_handshake_completed_ = nullptr;
        web_socket_.close();
    }

}}
