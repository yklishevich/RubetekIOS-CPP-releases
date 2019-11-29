#pragma once

#include <sys/types.h>
#include <ifaddrs.h>
#include <arpa/inet.h>
#include <net/if.h>    // IFF_UP

#include <array>
#include <string>

#include <msw/config.hpp>
#include <msw/noncopyable.hpp>
#include <msw/system_error/throw.hpp>


namespace dawn
{
    struct lin_interface_address
        : msw::noncopyable
    {
        lin_interface_address()
            : root_    ( get() )
            , current_ ( root_ )
        {}
        ~lin_interface_address()
        {
            ::freeifaddrs(root_);
        }

        char const* name() const
        {
            return current_->ifa_name;
        }
        bool active() const
        {
            MSW_ASSERT(current_);
            return current_->ifa_flags & IFF_UP;
        }
        bool has_address() const
        {
            MSW_ASSERT(current_);
            return current_->ifa_addr != nullptr;
        }
        int family() const
        {
            MSW_ASSERT(has_address());
            return current_->ifa_addr->sa_family;
        }
        bool is_packet_family() const
        {
#ifdef MSW_APPLE
            return false;
#else
            return family() == AF_PACKET;
#endif
        }
        bool is_ip_v4() const
        {
            return family() == AF_INET;
        }
        bool is_ip_v6() const
        {
            return family() == AF_INET6;
        }
        bool is_ip() const
        {
            return is_ip_v4() || is_ip_v6();
        }
        std::string address() const
        {
            MSW_ASSERT(has_address());
            MSW_ASSERT(is_ip());
            return addr_to_string(current_->ifa_addr);
        }
        std::string mask() const
        {
            MSW_ASSERT(has_address());
            MSW_ASSERT(is_ip());
            return addr_to_string(current_->ifa_netmask);
        }

        lin_interface_address& operator ++ ()
        {
            MSW_ASSERT(current_);
            current_ = current_->ifa_next;
            return *this;
        }

        explicit operator bool () const
        {
            return current_ != nullptr;
        }

    private:

        static ifaddrs* get()
        {
            ifaddrs* ifa;
            if (::getifaddrs(&ifa) != 0) msw::system_error::throw_exception("getifaddrs fail");
            return ifa;
        }
        static std::string addr_to_string(sockaddr* addr)
        {
            std::array<char, 64> buf;
            return ::inet_ntop(addr->sa_family, &(reinterpret_cast<sockaddr_in*>(addr)->sin_addr), buf.data(), buf.size()) ? buf.data() : "";
        }

        ifaddrs* const root_    ;
        ifaddrs*       current_ ;
    };
}
