#pragma once

#include <boost/variant.hpp>

#include <rubetek/utility/package/package.hpp>


namespace rubetek
{
#define SMART_HOUSE_PACK_VARIANT(N)\
    if (typeid(T ## N) == v.type())\
    {\
        rubetek::pack(static_cast<unsigned char>(N), local_buf);\
        rubetek::pack(boost::get<T ## N>(v), local_buf);\
    }

    template <typename T1, typename T2>
    struct package<boost::variant<T1, T2>>
    {
        typedef boost::variant<T1, T2> variant;

        static void pack(variant const& v, buffer& buf)
        {
            buffer local_buf;
            if (v.empty())
                rubetek::pack(static_cast<unsigned char>(0), local_buf);
            else
            {
                SMART_HOUSE_PACK_VARIANT(1)
                else
                SMART_HOUSE_PACK_VARIANT(2)
            }
            rubetek::pack(local_buf, buf);
        }
        static bool can_unpack(buffer::const_iterator position, buffer::const_iterator end)
        {
            return rubetek::can_unpack<buffer>(position, end);
        }
        static buffer::const_iterator unpack(variant& v, buffer::const_iterator position, buffer::const_iterator end)
        {
            buffer local_buf = rubetek::unpack<buffer>(position, end);
            {
                buffer::const_iterator position = local_buf.begin();
                buffer::const_iterator end = local_buf.end();
                switch (rubetek::unpack<unsigned char>(position, end))
                {
                    case 0: v = variant(); break;
                    case 1: v = rubetek::unpack<T1>(position, end); break;
                    case 2: v = rubetek::unpack<T2>(position, end); break;
                    default: assert(0);
                };
            }
            return position;
        }
    };

    template <typename T1, typename T2, typename T3>
    struct package<boost::variant<T1, T2, T3>>
    {
        typedef boost::variant<T1, T2, T3> variant;

        static void pack(variant const& v, buffer& buf)
        {
            buffer local_buf;
            if (v.empty())
                rubetek::pack(static_cast<unsigned char>(0), local_buf);
            else
            {
                SMART_HOUSE_PACK_VARIANT(1)
                else
                SMART_HOUSE_PACK_VARIANT(2)
                else
                SMART_HOUSE_PACK_VARIANT(3)
            }
            rubetek::pack(local_buf, buf);
        }
        static bool can_unpack(buffer::const_iterator position, buffer::const_iterator end)
        {
            return rubetek::can_unpack<buffer>(position, end);
        }
        static buffer::const_iterator unpack(variant& v, buffer::const_iterator position, buffer::const_iterator end)
        {
            buffer local_buf = rubetek::unpack<buffer>(position, end);
            {
                buffer::const_iterator position = local_buf.begin();
                buffer::const_iterator end = local_buf.end();
                switch (rubetek::unpack<unsigned char>(position, end))
                {
                    case 0: v = variant(); break;
                    case 1: v = rubetek::unpack<T1>(position, end); break;
                    case 2: v = rubetek::unpack<T2>(position, end); break;
                    case 3: v = rubetek::unpack<T3>(position, end); break;
                    default: assert(0);
                };
            }
            return position;
        }
    };

    template <typename T1, typename T2, typename T3, typename T4>
    struct package<boost::variant<T1, T2, T3, T4>>
    {
        typedef boost::variant<T1, T2, T3, T4> variant;

        static void pack(variant const& v, buffer& buf)
        {
            buffer local_buf;
            if (v.empty())
                rubetek::pack(static_cast<unsigned char>(0), local_buf);
            else
            {
                SMART_HOUSE_PACK_VARIANT(1)
                else
                SMART_HOUSE_PACK_VARIANT(2)
                else
                SMART_HOUSE_PACK_VARIANT(3)
                else
                SMART_HOUSE_PACK_VARIANT(4)
            }
            rubetek::pack(local_buf, buf);
        }
        static bool can_unpack(buffer::const_iterator position, buffer::const_iterator end)
        {
            return rubetek::can_unpack<buffer>(position, end);
        }
        static buffer::const_iterator unpack(variant& v, buffer::const_iterator position, buffer::const_iterator end)
        {
            buffer local_buf = rubetek::unpack<buffer>(position, end);
            {
                buffer::const_iterator local_pos = local_buf.begin();
                buffer::const_iterator local_end = local_buf.end();
                switch (rubetek::unpack<unsigned char>(local_pos, local_end))
                {
                    case 0: v = variant(); break;
                    case 1: v = rubetek::unpack<T1>(local_pos, local_end); break;
                    case 2: v = rubetek::unpack<T2>(local_pos, local_end); break;
                    case 3: v = rubetek::unpack<T3>(local_pos, local_end); break;
                    case 4: v = rubetek::unpack<T4>(local_pos, local_end); break;
                    default: assert(0);
                };
            }
            return position;
        }
    };

#undef SMART_HOUSE_PACK_VARIANT
}
