#pragma once

#include <rubetek/utility/package_std.hpp>
#include <rubetek/utility/multiform/package.hpp>
#include <rubetek/module/settings/setting.hpp>


namespace rubetek
{
    template <>
    struct package<module::setting>
    {
        static void pack(module::setting const& v, buffer& buf)
        {
            buffer local_buf;
            rubetek::pack(v.name, local_buf);
            rubetek::pack(v.value, local_buf);
            rubetek::pack(local_buf, buf);
        }
        static bool can_unpack(buffer::const_iterator position, buffer::const_iterator end)
        {
            return rubetek::can_unpack<buffer>(position, end);
        }
        static buffer::const_iterator unpack(module::setting& v, buffer::const_iterator position, buffer::const_iterator end)
        {
            buffer local_buf = rubetek::unpack<buffer>(position, end);
            {
                buffer::const_iterator local_pos = local_buf.begin();
                buffer::const_iterator local_end = local_buf.end();
                v.name = rubetek::unpack<std::string>(local_pos, local_end);
                v.value = rubetek::unpack<module::setting::value_type>(local_pos, local_end);
            }
            return position;
        }
    };
}
