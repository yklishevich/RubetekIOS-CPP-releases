#pragma once

#include <iphlpapi.h>

#include <msw/noncopyable.hpp>

#ifdef MSW_MSVC
#   pragma comment(lib, "IPHLPAPI.lib")
#endif


namespace dawn
{
    struct win_ip_address
    {
        explicit win_ip_address(IP_ADDR_STRING const* addr)
            : addr_(addr)
        {}
        win_ip_address(win_ip_address const& other)
            : addr_(other.addr_)
        {}

        char const* address() const
        {
            MSW_ASSERT(addr_);
            return addr_->IpAddress.String;
        }
        char const* mask() const
        {
            MSW_ASSERT(addr_);
            return addr_->IpMask.String;
        }

        win_ip_address& operator ++ ()
        {
            MSW_ASSERT(addr_);
            addr_ = addr_->Next;
            return *this;
        }

        explicit operator bool () const
        {
            return addr_ != nullptr;
        }

    private:
        IP_ADDR_STRING const* addr_;
    };

    struct win_adapter_info
        : msw::noncopyable
    {
        win_adapter_info()
            : mem_(sizeof(IP_ADAPTER_INFO))
            , current_(nullptr)
        {
            unsigned long len = static_cast<unsigned long>(mem_.size().count());
            if (get_adapeters_info(len) == ERROR_BUFFER_OVERFLOW) mem_.resize(len);
            unsigned const ret = get_adapeters_info(len);
            if (ret == NO_ERROR)
                current_ = adapeters_info_ptr();
            else if (ret != ERROR_NO_DATA)
                throw std::runtime_error("GetAdaptersInfo failed with error: " + std::to_string(ret));
        }

        unsigned combo_index() const
        {
            return current().ComboIndex;
        }
        char const* name() const
        {
            return current().AdapterName;
        }
        char const* description() const
        {
            return current().Description;
        }
        msw::range<msw::byte const> address() const
        {
            return msw::make_range(msw::make_ptr(current().Address), msw::size<msw::byte>(current().AddressLength));
        }
        unsigned index() const
        {
            return current().Index;
        }
        unsigned type() const
        {
            return current().Type;
        }
        win_ip_address ip_address() const
        {
            return win_ip_address(&current().IpAddressList);
        }
        win_ip_address gateway() const
        {
            return win_ip_address(&current().GatewayList);
        }
        win_ip_address dhcp_server() const
        {
            return win_ip_address(&current().DhcpServer);
        }
        bool dhcp_enabled() const
        {
            return current().DhcpEnabled != 0;
        }

        bool have_wins() const
        {
            return current().HaveWins == TRUE;
        }
        win_ip_address primary_wins_server() const
        {
            return win_ip_address(&current().PrimaryWinsServer);
        }
        win_ip_address secondary_wins_server() const
        {
            return win_ip_address(&current().SecondaryWinsServer);
        }

        win_adapter_info& operator ++ ()
        {
            MSW_ASSERT(current_);
            current_ = current_->Next;
            return *this;
        }

        explicit operator bool () const
        {
            return current_ != nullptr;
        }

    private:

        IP_ADAPTER_INFO* adapeters_info_ptr()
        {
            return mem_.front_max<IP_ADAPTER_INFO>().data().native();
        }
        unsigned get_adapeters_info(unsigned long& len)
        {
            return ::GetAdaptersInfo(adapeters_info_ptr(), &len);
        }

        IP_ADAPTER_INFO const& current() const
        {
            MSW_ASSERT(current_);
            return *current_;
        }

        msw::buffer<msw::byte> mem_     ;
        IP_ADAPTER_INFO const* current_ ;
    };
}
