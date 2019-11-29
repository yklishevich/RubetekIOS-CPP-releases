#pragma once

#include <boost/optional.hpp>

#include <msw/include/windows.hpp>
#include <msw/include/boost/asio.hpp>

namespace dawn {
namespace net  {

    inline boost::optional<boost::asio::ip::address> ip_address_from_sockaddr(sockaddr const* addr)
    {
        MSW_ASSERT(addr);
        if (addr->sa_family == AF_INET) return boost::asio::ip::address_v4(reinterpret_cast<sockaddr_in const*>(addr)->sin_addr.S_un.S_addr);
        else if (addr->sa_family == AF_INET6) return boost::asio::ip::address_v6(reinterpret_cast<std::array<msw::byte, 16> const&>(reinterpret_cast<sockaddr_in6 const*>(addr)->sin6_addr.u.Byte));
        return boost::none;
    }

}}
