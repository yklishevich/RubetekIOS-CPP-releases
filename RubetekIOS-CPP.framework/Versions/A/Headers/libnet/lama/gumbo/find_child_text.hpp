#pragma once

#include <boost/optional.hpp>

#include <lama/gumbo/for_each.hpp>


namespace lama  {
namespace gumbo {

    template <typename Object>
    boost::optional<text> find_child_text(Object obj)
    {
        boost::optional<text> result;
        conditional_for_each_child_text
        (
            obj
            , [&](GumboNodeType, text txt)
            {
                result = txt;
                return false;
            }
        );
        return result;
    }
    template <typename Object>
    boost::optional<text> find_child_text(Object obj, GumboNodeType type)
    {
        boost::optional<text> result;
        conditional_for_each_child_text
        (
            obj
            , [&](GumboNodeType node_type, text txt)
            {
                if (node_type == type)
                {
                    result = txt;
                    return false;
                }
                return true;
            }
        );
        return result;
    }

}}
