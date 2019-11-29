#pragma once

#include <msw/zzz/asio/web_socket.hpp>


namespace msw
{
    typedef zzz::web_socket_tmpl<boost::asio::ip::tcp::socket> web_socket;
}
