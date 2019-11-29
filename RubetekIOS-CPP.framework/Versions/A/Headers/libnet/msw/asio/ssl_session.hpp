#pragma once

#include <msw/asio/session.hpp>
#include <msw/asio/ssl_socket.hpp>


namespace msw
{
    typedef session<ssl_socket> ssl_session;
}
