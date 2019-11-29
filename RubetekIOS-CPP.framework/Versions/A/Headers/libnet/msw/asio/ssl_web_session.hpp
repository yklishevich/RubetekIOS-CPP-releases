#pragma once

#include <msw/asio/session.hpp>
#include <msw/asio/ssl_web_socket.hpp>
#include <msw/zzz/asio/ssl_web_session.hpp>


namespace msw
{
    typedef session<ssl_web_socket> ssl_web_session;
}
