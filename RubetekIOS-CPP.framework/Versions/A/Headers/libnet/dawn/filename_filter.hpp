#pragma once

#include <string>

#include <dawn/string_method.hpp>
#include <dawn/tokenize_string.hpp>



namespace dawn
{
    inline bool filename_filter(std::string filename, std::string filter)
    {
        if (filter.empty()) return true;
        std::list<std::string> filter_tokens = tokenize_string(filter, '*');
        if (filter.front() != '*')
        {
            if (!string_pop_front_equal(filename, filter_tokens.front())) return false;
            filter_tokens.pop_front();
        }
        while (!filter_tokens.empty())
        {
            std::string const& token = filter_tokens.front();
            bool const ok = filter_tokens.size() == 1 ? string_pop_front_if_find_backward(filename, token) : string_pop_front_if_find(filename, token);
            if (!ok) return false;
            filter_tokens.pop_front();
        }
        if (filename.empty()) return true;
        return filter.back() == '*';
    }
}
