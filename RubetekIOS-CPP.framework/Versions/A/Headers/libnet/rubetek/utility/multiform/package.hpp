#pragma once

#include <rubetek/utility/package_std.hpp>
#include <rubetek/utility/multiform/multiform.hpp>


namespace rubetek
{
    template <>
    struct package<multiform>
    {
        static void pack(multiform const& v, buffer& buf)
        {
            buffer local_buf;
            rubetek::pack(v.ctti_typeid(), local_buf);
            rubetek::pack(v.data_, local_buf);
            rubetek::pack(local_buf, buf);
        }
        static bool can_unpack(buffer::const_iterator position, buffer::const_iterator end)
        {
            return rubetek::can_unpack<buffer>(position, end);
        }
        static buffer::const_iterator unpack(multiform& v, buffer::const_iterator position, buffer::const_iterator end)
        {
            buffer local_buf = rubetek::unpack<buffer>(position, end);
            {
                buffer::const_iterator local_pos = local_buf.begin();
                buffer::const_iterator local_end = local_buf.end();
                v.ctti_typeid_ = rubetek::unpack<decltype(v.ctti_typeid_)>(local_pos, local_end);
                v.data_ = rubetek::unpack<decltype(v.data_)>(local_pos, local_end);
            }
            return position;
        }
    };
}
