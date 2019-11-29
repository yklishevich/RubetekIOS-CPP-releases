#pragma once


namespace msw
{
    template <typename T1, typename T2>
    int percentage_relation(T1 minor, T2 major)
    {
        if (minor == 0) return 0;
        return int(static_cast<double>(minor) / major * 100);
    }
}
