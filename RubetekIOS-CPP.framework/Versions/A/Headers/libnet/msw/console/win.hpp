#pragma once

#include <string>

#include <msw/include/windows.hpp>
#include <msw/system_error/throw.hpp>


namespace msw     {
namespace console {
namespace win     {

    template <typename Handler>
    void set_ctrl_handler(Handler h, bool add = true)
    {
        if (!::SetConsoleCtrlHandler(reinterpret_cast<PHANDLER_ROUTINE>(h), add))
            system_error::throw_exception("can't set console ctrl handler");
    }
    inline void set_title(std::string const& title)
    {
        if (!::SetConsoleTitleA(title.c_str())) system_error::throw_exception("can't set console title");
    }
    inline void set_console_size(short width, short height)
    {
        SMALL_RECT rect;
        rect.Top    = 0      ;
        rect.Left   = 0      ;
        rect.Right  = width  ;
        rect.Bottom = height ;
        ::SetConsoleWindowInfo(::GetStdHandle(STD_OUTPUT_HANDLE), true, &rect);
    }

}}}
