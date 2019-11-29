#pragma once

#include <string>

#include <rubetek/essence/device_properties.hpp>
#include <rubetek/utility/package/property.hpp>
#include <rubetek/utility/package/package.hpp>
#include <rubetek/utility/package/pod.hpp>


namespace rubetek
{
    template <>
    struct package<device_properties>
    {
        static void pack(device_properties const& v, buffer& buf)
        {
            buffer local_buf;
            rubetek::pack(v.address, local_buf);
            rubetek::pack(v.properties, local_buf);
            rubetek::pack(local_buf, buf);
        }
        static bool can_unpack(buffer::const_iterator position, buffer::const_iterator end)
        {
            return rubetek::can_unpack<buffer>(position, end);
        }
        static buffer::const_iterator unpack(device_properties& v, buffer::const_iterator position, buffer::const_iterator end)
        {
            buffer local_buf = rubetek::unpack<buffer>(position, end);
            {
                buffer::const_iterator local_pos = local_buf.begin();
                buffer::const_iterator local_end = local_buf.end();
                v.address = rubetek::unpack<property_address>(local_pos, local_end);
                v.properties = rubetek::unpack<property_list>(local_pos, local_end);
            }
            return position;
        }
    };
}
