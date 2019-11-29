#pragma once

#include <msw/generator/counter.hpp>
#include <msw/algorithm/generate.hpp>


namespace msw
{
    template <typename Dest>
    void generate_counter(Dest&& dest)
    {
        typedef typename std::remove_reference<Dest>::type dest_type;
        typedef typename dest_type::value_type value_type;
        generate(std::forward<Dest>(dest), generator::counter<value_type>());
    }
}
