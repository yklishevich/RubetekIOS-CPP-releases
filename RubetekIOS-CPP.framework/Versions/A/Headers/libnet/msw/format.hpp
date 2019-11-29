#pragma once

#include <string>
#include <sstream>
#include <iostream>

#include <msw/ptr.hpp>
#include <msw/bit_ptr.hpp>
#include <msw/size.hpp>
#include <msw/range.hpp>
#include <msw/buffer.hpp>
#include <msw/assert.hpp>
#include <msw/std/thread.hpp>
#include <msw/algorithm/search.hpp>
#include <msw/lexical_cast/ptr_to_str.hpp>
#include <msw/lexical_cast/bytes_as_string.hpp>
#include <msw/dump/line.hpp>


namespace msw
{
    struct new_line{};
    static new_line const nl = new_line();

    template <typename T> void format_s(std::ostream&, T&&);

    inline void format_s(std::ostream& ostream, new_line)
    {
        ostream << '\n';
    }

    inline void format_s(std::ostream& ostream, char const* s)
    {
        ostream << s;
    }
    inline void format_s(std::ostream& ostream, std::string&& s)
    {
        format_s(ostream, s.c_str());
    }
    inline void format_s(std::ostream& ostream, std::string& s)
    {
        format_s(ostream, s.c_str());
    }
    inline void format_s(std::ostream& ostream, std::string const& s)
    {
        format_s(ostream, s.c_str());
    }

    inline void format_s(std::ostream& ostream, msw::thread::id const& id)
    {
        ostream << id;
    }

    template <typename T>
    void format_s(std::ostream& ostream, size<T> sz)
    {
        format_s(ostream, sz.count());
    }

    template <typename T>
    void format_s(std::ostream& ostream, ptr<T> p)
    {
        format_s(ostream, ptr_to_str(p));
    }
    template <typename T>
    void format_s(std::ostream& ostream, bit_ptr_t<T> p)
    {
        format_s(ostream, ptr_to_str(p));
    }
    inline void format_s(std::ostream& ostream, byte b)
    {
        format_s(ostream, msw::uint32(b));
    }
    inline void format_s(std::ostream& ostream, void* p)
    {
        format_s(ostream, make_ptr(p));
    }
    inline void format_s(std::ostream& ostream, void const* p)
    {
        format_s(ostream, make_ptr(p));
    }

    inline void format_s(std::ostream& ostream, range<char const> r)
    {
        if (r.has_items())
        {
            if (search(char(0), r) == r.end()) format_s(ostream, bytes_as_string(r));
            else format_s(ostream, r.data().native());
        }
    }
    inline void format_s(std::ostream& ostream, range<char> r)
    {
        format_s(ostream, r.all<char const>());
    }
    inline void format_s(std::ostream& ostream, buffer<char> const& buf)
    {
        format_s(ostream, buf.all());
    }
    inline void format_s(std::ostream& ostream, buffer<char>&& buf)
    {
        format_s(ostream, buf.all());
    }

    template <typename T>
    void format_s(std::ostream& ostream, range<T> r)
    {
        format_s(ostream, line_dump(r));
    }
    template <typename T>
    void format_s(std::ostream& ostream, buffer<T>& buf)
    {
        format_s(ostream, buf.all());
    }
    template <typename T>
    void format_s(std::ostream& ostream, buffer<T> const& buf)
    {
        format_s(ostream, buf.all());
    }
}


namespace msw {
namespace zzz {

    template <typename T>
    struct generic_format_s
    {
        template <typename U>
        void operator()(std::ostream& ostream, U&& v)
        {
            ostream << std::forward<U>(v);
        }
    };

    template <typename T>
    struct generic_format_s<T*>
    {
        void operator()(std::ostream& ostream, T* v)
        {
            format_s(ostream, make_ptr(v));
        }
    };
    template <typename T>
    struct generic_format_s<T const*>
    {
        void operator()(std::ostream& ostream, T const* v)
        {
            format_s(ostream, make_ptr(v));
        }
    };

}}


namespace msw
{
    template <typename T>
    void format_s(std::ostream& ostream, T&& v)
    {
        zzz::generic_format_s
        <
            typename std::remove_const
            <
                typename std::remove_reference<T>::type
            >::type
        >()(ostream, std::forward<T>(v));
    }

    template <typename T, typename ...Args>
    void format_s(std::ostream& ostream, T&& v, Args&& ...args)
    {
        format_s(ostream, std::forward<T>(v));
        format_s(ostream, std::forward<Args>(args)...);
    }

    template <typename ...Args>
    std::string format(Args&& ...args)
    {
        std::stringstream ss;
        format_s(ss, std::forward<Args>(args)...);
        return ss.str();
    }
}
