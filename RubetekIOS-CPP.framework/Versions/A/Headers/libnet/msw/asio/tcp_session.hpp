#pragma once

#include <msw/asio/session.hpp>
#include <msw/asio/tcp_socket.hpp>


namespace msw
{
    typedef session<tcp_socket> tcp_session;
}
