#pragma once 

#include <cmath>
#include <string>
#include <type_traits>


namespace msw
{
    template <typename T>
    std::string precise_format_count(T x)
    {
        static_assert(std::is_integral<T>::value, "T must be integral");
        std::string s = std::to_string(x);
        std::string r;
        r.reserve(s.size() + s.size() / 3);
        int n = static_cast<int>(std::fmod(3 - (s.size() % 3), 3));
        for (char c : s)
        {
            if (n == 3)
            {
                r.push_back(' ');
                n = 0;
            }
            r.push_back(c);
            n++;
        }
        return r;
    }
}
