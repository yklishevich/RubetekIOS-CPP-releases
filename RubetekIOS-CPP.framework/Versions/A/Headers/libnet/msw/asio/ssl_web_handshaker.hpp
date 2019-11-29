#pragma once

#include <msw/asio/ssl_web_socket.hpp>
#include <msw/asio/web_handshaker.hpp>


namespace msw
{
    typedef web_handshaker_tmpl<ssl_web_socket> ssl_web_handshaker;
}
