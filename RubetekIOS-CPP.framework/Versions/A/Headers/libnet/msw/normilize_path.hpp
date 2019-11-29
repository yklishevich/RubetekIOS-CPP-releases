#pragma once

#include <string>

#include <msw/return_move.hpp>


namespace msw
{
    inline std::string normilize_path(std::string path)
    {
        if (!path.empty())
        {
            char const last_symbol = *--path.end();
            if (last_symbol != '/' || last_symbol != '\\') path += '/';
        }
        MSW_RETURN_MOVE(path);
    }
}
