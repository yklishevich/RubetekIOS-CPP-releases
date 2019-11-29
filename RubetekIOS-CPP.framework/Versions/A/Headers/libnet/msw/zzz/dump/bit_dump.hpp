#pragma once

#include <string>

#include <boost/format.hpp>

#include <msw/assert.hpp>
#include <msw/dimension/counter.hpp>
#include <msw/plain_types.hpp>       // int32


namespace msw {
namespace zzz {

    template <typename Range>
    std::string bit_dump(Range r)
    {
        bit_size const max_size = 0xffff;
        MSW_ASSERT(r.size() <= max_size);
        r.crop_front_max(max_size);
        int32 const line_size = 64;
        bit_size const elem_size = size<bit::base_type>(1);
        std::string res;
        msw::counter<msw::qbyte, line_size> cntr;

        while (r.has_items())
        {
            if (!res.empty()) res.push_back('\n');
            Range r_line = r.pop_front_max(line_size);
            res += boost::str(boost::format("%04X:") % cntr.value());

            while (r_line.has_items())
            {
                res.push_back(' ');
                Range r_elem = r_line.pop_front_max(elem_size);
                while (r_elem.has_items())
                    res.push_back(r_elem.pop_front() ? '*' : '-');
            };
            ++cntr;
        };
        return res;
    }

}}
