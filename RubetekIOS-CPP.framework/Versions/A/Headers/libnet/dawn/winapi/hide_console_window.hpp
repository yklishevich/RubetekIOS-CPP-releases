#pragma once

#include <msw/include/windows.hpp>


namespace dawn {
namespace win  {

    inline void hide_console_window()
    {
        HWND hWnd = ::GetConsoleWindow();
        ::ShowWindow( hWnd, SW_HIDE );
    }

}}
