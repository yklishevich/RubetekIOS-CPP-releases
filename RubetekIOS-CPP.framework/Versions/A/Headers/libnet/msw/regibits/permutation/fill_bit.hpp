#pragma once

#include <msw/regibits/permutation/set_bit.hpp>
#include <msw/regibits/permutation/reset_bit.hpp>


namespace msw      {
namespace regibits {

    template <typename T>
    void fill_bit(T& val, int pos, bool x)
    {
        if (x) set_bit(val, pos);
        else reset_bit(val, pos);
    }

}}
