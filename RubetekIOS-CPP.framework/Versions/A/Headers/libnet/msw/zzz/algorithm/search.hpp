#pragma once

#include <msw/algorithm/equal.hpp>


namespace msw {
namespace zzz {
namespace alg {

    template <typename What, typename Where>
    typename Where::position search_value(What const& what, Where where)
    {
        while (where.has_items() && (where.front() != what)) where.pop_front();
        return where.begin();
    }

    template <typename What, typename Where>
    typename Where::position search_range(What what, Where where)
    {
        MSW_ASSERT(what.has_items());
        if (where.size() > what.size())
        {
            auto const end = where.end(what.size().count() - 1);
            for (; where.begin() != end; where.pop_front())
                if (msw::partial_equal(where, what)) break;
        }
        return where.begin();
    }

}}}
