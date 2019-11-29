#pragma once

#include <msw/zzz/entry_point/call_fwd.hpp>
#include <msw/include/windows.hpp>


int __stdcall WinMain(HINSTANCE instance, HINSTANCE prev_instance, char* cmd_line, int cmd_show)
{
    return msw::entry_point::zzz::call(instance, prev_instance, cmd_line, cmd_show);
}
