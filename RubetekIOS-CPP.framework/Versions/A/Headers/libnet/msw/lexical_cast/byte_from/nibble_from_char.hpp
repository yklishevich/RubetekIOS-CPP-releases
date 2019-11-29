#pragma once 

#include <string>

#include <msw/plain_types.hpp> // byte


namespace msw
{
    inline byte nibble_from_char(char c, bool& error)
    {
        byte b = static_cast<byte>(c);
        error = false;
        if ((c >= 0x30) && (c <= 0x39)) return b - 0x30;
        if ((c >= 0x41) && (c <= 0x46)) return b - 0x37;
        if ((c >= 0x61) && (c <= 0x66)) return b - 0x57;
        error = true;
        return 0;
    }
    inline byte nibble_from_char(char c)
    {
        bool error;
        byte b = nibble_from_char(c, error);
        if (error) throw std::runtime_error(std::string("'") + c + "' is not hex char");
        return b;
    }
}
