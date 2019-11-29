#pragma once

#include <map>
#include <cassert>

#include <rubetek/utility/package/package.hpp>


namespace rubetek
{
    template <typename T, typename T2>
    struct package<std::map<T, T2>>
    {
        typedef std::map<T, T2> map;

        static void pack(map const& m, buffer& buf)
        {
            buffer local_buf;
            for (auto const& t : m)
                rubetek::pack(t, local_buf);
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

        static buffer::const_iterator unpack(map& mp, buffer::const_iterator position, buffer::const_iterator end)
        {
            unsigned size = rubetek::unpack<unsigned>(position, end);
            buffer::const_iterator local_end = position + size;
            assert(end >= local_end);
            while (position != local_end)
                mp.insert(rubetek::unpack<std::pair<T, T2>>(position, local_end));
            return position;
        }
    };
}
