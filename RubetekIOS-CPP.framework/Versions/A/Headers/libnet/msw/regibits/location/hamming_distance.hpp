#pragma once

#include <msw/regibits/location/population_count.hpp>


namespace msw      {
namespace regibits {

    template <typename T>
    int hamming_distance(T val1, T val2)
    {
        T const val = val1 ^ val2;
        return population_count(val);
    }

}}
