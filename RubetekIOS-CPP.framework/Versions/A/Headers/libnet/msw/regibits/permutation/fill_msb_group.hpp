#pragma once

#include <msw/regibits/permutation/set_msb_group.hpp>
#include <msw/regibits/permutation/reset_msb_group.hpp>


namespace msw      {
namespace regibits {

    template <typename T>
    void fill_msb_group(T& val, int count, bool x)
    {
        if (x) set_msb_group(val, count);
        else reset_msb_group(val, count);
    }

}}
