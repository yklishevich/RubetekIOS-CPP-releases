#pragma once

#include <list>
#include <string>
#include <utility>

#include <msw/config.hpp>

#ifdef MSW_WINDOWS
#   include <dawn/net/win_adapter_info.hpp>
#endif
#ifdef MSW_LINUX
#   include <dawn/net/lin_interface_address.hpp>
#endif


namespace dawn
{
    inline std::list<std::pair<std::string, std::string>> make_ip_list()
    {
        std::list<std::pair<std::string, std::string>> list;
#ifdef MSW_WINDOWS
        for (win_adapter_info adapter_info; adapter_info; ++adapter_info)
            for (win_ip_address ip = adapter_info.ip_address(); ip; ++ip)
                list.push_back(std::make_pair(ip.address(), ip.mask()));
#endif
#ifdef MSW_LINUX
        #ifndef MSW_ANDROID
        for (lin_interface_address ifa; ifa; ++ifa)
        {
            if (ifa.has_address() && ifa.is_ip_v4())
            {
                list.push_back(std::make_pair(ifa.address(), ifa.mask()));
            }
        }
        #endif
#endif
        list.unique();
        return std::move(list);
    }
}
