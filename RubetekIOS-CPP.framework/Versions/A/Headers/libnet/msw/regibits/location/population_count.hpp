#pragma once


namespace msw      {
namespace regibits {

    template <typename T>
    int population_count(T val)
    {
        int result = 0;
        int const size = sizeof(T) * 8;
        for (int n = 0; n < size; n++, val >>= 1)
            if (val & 1) result++;
        return result;
    }

}}
