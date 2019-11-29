#pragma once

#include <map>
#include <cassert>

#include <rubetek/utility/package/package.hpp>


namespace rubetek
{
    template <typename T, typename T2>
    struct package<std::pair<T, T2>>
    {
        typedef std::pair<T, T2> pair;

        static void pack(pair const& p, buffer& buf)
        {
            buffer local_buf;
            rubetek::pack(p.first, local_buf);
            rubetek::pack(p.second, local_buf);
            unsigned size = static_cast<unsigned>(local_buf.size());
            rubetek::pack(size, buf);
            buf.insert(buf.end(), local_buf.begin(), local_buf.end());
        }

        static bool can_unpack(buffer::const_iterator position, buffer::const_iterator end)
        {
            bool result = false;
            if (rubetek::can_unpack<unsigned>(position, end))
            {
                unsigned size = rubetek::unpack<unsigned>(position, end);
                result = (end - position) >= static_cast<decltype((end - position))>(size);
            }
            return result;
        }

        static buffer::const_iterator unpack(pair& p, buffer::const_iterator position, buffer::const_iterator end)
        {
            unsigned size = rubetek::unpack<unsigned>(position, end);
            buffer::const_iterator local_end = position + size;
            assert(end >= local_end);
            p.first = rubetek::unpack<T>(position, local_end);
            p.second = rubetek::unpack<T2>(position, local_end);
            return position;
        }
    };
}
