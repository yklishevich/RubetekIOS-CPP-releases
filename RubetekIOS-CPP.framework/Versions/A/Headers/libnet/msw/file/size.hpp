#pragma once

#include <string>

#include <msw/file/ifile.hpp>
#include <msw/plain_types.hpp>


namespace msw
{
    inline uint64 file_size(std::string const& file_name)
    {
        return ifile(file_name).size();
    }
}
