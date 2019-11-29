#pragma once

#include <string>
#include <iostream>


namespace msw     {
namespace console {
namespace lin     {

    inline void set_title(std::string const& title)
    {
        char esc_start[] = { 0x1b, ']', '0', ';', 0 };
        char esc_end[] = { 0x07, 0 };
        std::cout << esc_start << title << esc_end;
        std::cout.flush();
    }

}}}
