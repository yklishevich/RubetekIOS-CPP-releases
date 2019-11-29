#pragma once

#include <map>
#include <string>

#include <msw/self/path.hpp>


namespace msw
{
    inline std::string decode_filename(std::string filename, char symbol = '*', std::string replacer = self_path())
    {
        if (!filename.empty() && (filename.front() == symbol))
        {
            filename.erase(filename.begin());
            filename = replacer + filename;
        }
        return filename;
    }
}
