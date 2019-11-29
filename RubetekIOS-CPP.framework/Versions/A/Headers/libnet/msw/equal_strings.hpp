#pragma once

#include <string>


namespace msw
{
    template <typename Fst, typename Snd>
    bool equal_strings(Fst&& fst, Snd&& snd)
    {
        return std::string(std::forward<Fst>(fst)) == std::string(std::forward<Snd>(snd));
    }
}
