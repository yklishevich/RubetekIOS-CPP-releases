#pragma once

#include <boost/optional.hpp>

#include <msw/decode_filename.hpp>


namespace msw
{
    inline boost::optional<std::string> decode_filename_optional(boost::optional<std::string> filename, char symbol = '*', std::string replacer = self_path())
    {
        if (filename) filename = decode_filename(*filename, symbol, replacer);
        return filename;
    }
}
