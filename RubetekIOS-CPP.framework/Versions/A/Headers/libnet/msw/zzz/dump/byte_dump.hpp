#pragma once

#include <string>
#include <type_traits>               // std::enable_if

#include <boost/format.hpp>

#include <msw/dimension/counter.hpp>


namespace msw {
namespace zzz {

    template <typename Range>
    typename std::enable_if<sizeof(typename Range::value_type) == 1, std::string>::type
    byte_dump(Range r)
    {
        size<byte> const max_size = 0x0fffffff;
        MSW_ASSERT(r.size() <= max_size);
        r.crop_front_max(max_size);
        uint32 const line_size = 16;
        counter<uint32, line_size> cntr;

        std::string result;
        while (r.has_items())
        {
            if (!result.empty()) result.push_back('\n');
            auto r_line = r.pop_front_max(line_size);

            uint32 remind = line_size - static_cast<uint32>(r_line.size().count());
            result += boost::str(boost::format("%08X: ") % cntr.value());
            std::string text = "| ";
            while (r_line.has_items())
            {
                auto b = r_line.pop_front();
                result += boost::str(boost::format("%02X ") % static_cast<uint32>(b));
                text.push_back((b < 0x20) || (b > 0x7E) ? '.' : static_cast<char>(b));
            }
            for (; remind; --remind)
            {
                result += "   ";
                text.push_back(' ');
            }
            result += text;
            result += " |";
            ++cntr;
        };
        if (r.size() > max_size) result += "\n...";
        return result;
    }

    template <typename Range>
    typename std::enable_if<sizeof(typename Range::value_type) != 1, std::string>::type
    byte_dump(Range r)
    {
        static_assert((sizeof(typename Range::value_type) == 2) || (sizeof(typename Range::value_type) == 4), "size of value_type must be 2 or 4 bytes");

        size<typename Range::value_type> const max_size = 0x0fffffff;
        r.crop_front_max(max_size);
        uint32 const line_size = 24 / sizeof(typename Range::value_type);
        counter<msw::qbyte, line_size> cntr;

        std::string const digit_format = sizeof(typename Range::value_type) == 2 ? "%04X " : "%08X ";
        std::string result;
        while (r.has_items())
        {
            if (!result.empty()) result.push_back('\n');
            auto r_line = r.pop_front_max(line_size);

            result += boost::str(boost::format("%08X: ") % cntr.value());
            while (r_line.has_items())
            {
                auto b = r_line.pop_front();
                result += boost::str(boost::format(digit_format) % static_cast<uint32>(b));
            }
            ++cntr;
        };
        if (r.size() > max_size) result += "\n...";
        return result;
    }

}}
