#pragma once

#include <array>
#include <regex>
#include <string>
#include <vector>

#include <msw/buffer.hpp>
#include <msw/throw_runtime_error.hpp>
#include <msw/lexical_cast/bytes_as_string.hpp>

#include <lama/boost/include/algorithm/string.hpp>


namespace dawn {
namespace win  {

    inline std::string cpu_name()
    {
        std::array<int, 4> cpui;
        __cpuid(cpui.data(), 0x80000000);
        int const count = cpui[0];  
        std::vector<std::array<int, 4>> info;
        for (int i = 0x80000000; i < count; ++i)
        {
            __cpuidex(cpui.data(), i, 0);
            info.push_back(cpui);
        }
        msw::buffer<char> brand;
        if (count > 0x80000004)
        {
            for (int i = 2; i < 5; ++i)
                brand.push_back(msw::make_range<char>(info[i]));
            std::string s = brand.data().native();
            static std::regex e(" +");
            s = std::regex_replace(s, e, " ");
            return s;
        }
        msw::throw_runtime_error("CPU name fail");
#include <msw/throw_hide_warning.hpp>
    }

    inline std::string cpu_name_short()
    {
        std::string s = cpu_name();
        boost::algorithm::erase_all(s, "CPU ");
        boost::algorithm::erase_all(s, "@ ");
        boost::algorithm::erase_all(s, "(R)");
        boost::algorithm::erase_all(s, "(TM)");
        //boost::algorithm::erase_all(s, "Intel(R) ");
        //boost::algorithm::erase_all(s, "Core(TM) ");
        //boost::algorithm::erase_all(s, "Pentium(R) ");
        return s;
    }

}}
