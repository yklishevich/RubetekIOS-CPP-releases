#pragma once

#include <string>

#include <rubetek/utility/package/package.hpp>


namespace rubetek
{
    template <>
    struct package<std::string>
    {
        static void pack(std::string const& v, buffer& buf)
        {
            rubetek::pack(buffer(v.begin(), v.end()), buf);
        }
        static bool can_unpack(buffer::const_iterator position, buffer::const_iterator end)
        {
            return rubetek::can_unpack<buffer>(position, end);
        }
        static buffer::const_iterator unpack(std::string& v, buffer::const_iterator position, buffer::const_iterator end)
        {
            buffer local_buf = rubetek::unpack<buffer>(position, end);
            v = std::string(local_buf.begin(), local_buf.end());
            return position;
        }
    };
}
