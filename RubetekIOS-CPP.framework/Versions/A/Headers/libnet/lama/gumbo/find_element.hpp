#pragma once

#include <boost/optional.hpp>

#include <msw/std/regex.hpp>
#include <msw/equal_strings.hpp>

#include <lama/gumbo/element.hpp>
#include <lama/gumbo/for_each.hpp>


namespace lama  {
namespace gumbo {

    template <typename NodeObject>
    boost::optional<element> find_element(NodeObject node, GumboTag tag, int max_depth = -1)
    {
        boost::optional<element> elem;
        conditional_for_each_element
        (
            node
            , [&](element e, int)
            {
                if (e.tag() == tag)
                {
                    elem = e;
                    return false;
                }
                return true;
            }
            , max_depth
        );
        return elem;
    }
    template <typename NodeObject>
    boost::optional<element> find_element(NodeObject node, GumboTag tag, char const* attr_name, char const* attr_value = nullptr, int max_depth = -1)
    {
        boost::optional<element> elem;
        conditional_for_each_element
        (
            node
            , [&](element e, int)
            {
                if (e.tag() == tag)
                {
                    auto attr = e.get_attribute(attr_name);
                    if (attr)
                    {
                        if (attr_value && !(msw::equal_strings(attr->value(), attr_value))) return true;
                        elem = e;
                        return false;
                    }
                }
                return true;
            }
            , max_depth
        );
        return elem;
    }
    template <typename NodeObject>
    boost::optional<element> find_element(NodeObject node, GumboTag tag, char const* attr_name, msw::regex const& attr_expr, int max_depth = -1)
    {
        boost::optional<element> elem;
        conditional_for_each_element
        (
            node
            , [&](element e, int)
            {
                if (e.tag() == tag)
                {
                    auto attr = e.get_attribute(attr_name);
                    if (attr)
                    {
                        msw::cmatch w;
                        if (!regex_match(attr->value(), w, attr_expr)) return true;
                        elem = e;
                        return false;
                    }
                }
                return true;
            }
            , max_depth
        );
        return elem;
    }

}}
