#pragma once

#include <memory>
#include <functional>

#include <msw/assert.hpp>
#include <msw/noncopyable.hpp>
#include <msw/asio/tcp_socket.hpp>
#include <msw/asio/ssl_socket.hpp>
#include <msw/include/boost/asio.hpp>
#include <msw/asio/ssl_verify_context.hpp>

#include <lama/openssl/include/autolink.hpp>


namespace msw {
namespace zzz {

    struct ssl_handshaker
        : noncopyable
        , std::enable_shared_from_this<ssl_handshaker>
    {
        typedef std::function<void(ssl_socket)>                            on_handshake_completed ;
        typedef std::function<bool(bool preverified, ssl_verify_context&)> on_verify              ;

             ssl_handshaker (tcp_socket&&, ssl_context&, on_handshake_completed, on_verify, on_error) ;
        void start          ()                                                                        ;
        void cancel         ()                                                                        ;

    private:

        ssl_socket             ssl_socket_             ;
        on_handshake_completed on_handshake_completed_ ;
        error_handler          error_handler_          ;
    };

}}


namespace msw {
namespace zzz {

    inline ssl_handshaker::ssl_handshaker (tcp_socket&& tcp_socket, ssl_context& context, on_handshake_completed on_handshake_completed, on_verify on_verify, on_error on_error)
        : ssl_socket_             ( std::move(tcp_socket), context )
        , on_handshake_completed_ ( on_handshake_completed         )
        , error_handler_          ( on_error                       )
    {
        //ssl_socket_.set_verify_mode(boost::asio::ssl::verify_peer);
        if (on_verify) ssl_socket_.set_verify_callback(on_verify);
    }
    inline void ssl_handshaker::start()
    {
        auto self(shared_from_this());
        ssl_socket_.async_handshake
        (
            boost::asio::ssl::stream_base::client
            , [this, self](boost::system::error_code ec)
            {
                if (on_handshake_completed_)
                {
                    if (ec) error_handler_(ec);
                    else (on_handshake_completed(on_handshake_completed_))(std::move(ssl_socket_));
                }
            }
        );
    }
    inline void ssl_handshaker::cancel()
    {
        on_handshake_completed_ = nullptr;
        ssl_socket_.close();
    }

}}
