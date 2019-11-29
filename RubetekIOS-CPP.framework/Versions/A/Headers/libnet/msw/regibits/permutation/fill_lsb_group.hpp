#pragma once

#include <msw/regibits/permutation/set_lsb_group.hpp>
#include <msw/regibits/permutation/reset_lsb_group.hpp>


namespace msw      {
namespace regibits {

    template <typename T>
    void fill_lsb_group(T& val, int count, bool x)
    {
        if (x) set_lsb_group(val, count);
        else reset_lsb_group(val, count);
    }

}}
