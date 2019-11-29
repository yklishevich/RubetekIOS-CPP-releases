#pragma once

#include <msw/asio/session.hpp>
#include <msw/asio/web_socket.hpp>
#include <msw/zzz/asio/web_session.hpp>


namespace msw
{
    typedef session<web_socket> web_session;
}
