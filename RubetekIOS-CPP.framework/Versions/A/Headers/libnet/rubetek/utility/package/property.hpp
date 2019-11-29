#pragma once

#include <string>

#include <rubetek/essence/property.hpp>
#include <rubetek/utility/package/package.hpp>
#include <rubetek/utility/package/pod.hpp>
#include <rubetek/utility/package/vector.hpp>
#include <rubetek/utility/package/string.hpp>
#include <rubetek/utility/package/variant.hpp>


namespace rubetek
{
    template <>
    struct package<property>
    {
        static void pack(property const& v, buffer& buf)
        {
            buffer local_buf;
            rubetek::pack(v.id, local_buf);
            rubetek::pack(v.value, local_buf);
            rubetek::pack(local_buf, buf);
        }
        static bool can_unpack(buffer::const_iterator position, buffer::const_iterator end)
        {
            return rubetek::can_unpack<buffer>(position, end);
        }
        static buffer::const_iterator unpack(property& v, buffer::const_iterator position, buffer::const_iterator end)
        {
            buffer local_buf = rubetek::unpack<buffer>(position, end);
            {
                buffer::const_iterator local_pos = local_buf.begin();
                buffer::const_iterator local_end = local_buf.end();
                v.id = rubetek::unpack<property::id_type>(local_pos, local_end);
                v.value = rubetek::unpack<property::value_type>(local_pos, local_end);
            }
            return position;
        }
    };
}
