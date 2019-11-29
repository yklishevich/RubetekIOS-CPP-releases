#pragma once

#include <string>

#include <msw/format.hpp>

#include <dawn/winapi/cpu_info.hpp>
#include <dawn/winapi/cpu_name.hpp>


namespace dawn {
namespace win  {

    inline std::string cpu_description()
    {
        std::string s = cpu_name_short();
        cpu_info info;
        if (info.processor_count() > 1) s += msw::format(" x", info.processor_count());
        std::string logical_cores;
        if (info.logical_processor_count() > info.core_count())
            logical_cores = msw::format("/", info.logical_processor_count());
        s += msw::format(" ", info.core_count(), logical_cores,"-cores");
        return s;
    }

}}
