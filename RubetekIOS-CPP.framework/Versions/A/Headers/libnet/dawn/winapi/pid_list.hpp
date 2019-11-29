#pragma once

#include <msw/include/psapi.hpp>

#include <array>
#include <vector>

#include <msw/plain_types.hpp>
#include <msw/system_error/throw.hpp>


namespace dawn {
namespace win  {

    inline std::vector<msw::int32> pid_list()
    {
        std::array<DWORD, 1024> pid_array;
        DWORD size;
        if (!::EnumProcesses(pid_array.data(), sizeof(pid_array), &size))
            msw::system_error::throw_exception("pid list fail");
        std::vector<msw::int32> lst(size / sizeof(DWORD));
        return std::move(lst);
    }

}}
