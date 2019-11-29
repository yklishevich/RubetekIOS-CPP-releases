#pragma once

#include <string>


namespace dawn
{
    inline void string_pop_front(std::string& s, size_t n)
    {
        s = s.substr(n);
    }
    inline bool string_pop_front_if_find(std::string& s, std::string const& w)
    {
        size_t const pos = s.find(w);
        if (pos == std::string::npos) return false;
        string_pop_front(s, pos + w.size());
        return true;
    }
    inline bool string_pop_front_if_find_backward(std::string& s, std::string const& w)
    {
        size_t const pos = s.rfind(w);
        if (pos == std::string::npos) return false;
        string_pop_front(s, pos + w.size());
        return true;
    }
    inline bool string_pop_front_equal(std::string& s, std::string const& w)
    {
        if (w.empty()) return true;
        if (s.size() >= w.size() && s.compare(0, w.size() - 1, w))
        {
            s = s.substr(w.size());
            return true;
        }
        return false;
    }
}
