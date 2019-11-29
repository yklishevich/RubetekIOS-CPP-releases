#pragma once

#include <boost/optional.hpp>

#include <lama/gumbo/node_as_text.hpp>
#include <lama/gumbo/node_as_element.hpp>


namespace lama  {
namespace gumbo {

    inline boost::optional<text> extract_html_mono_text(GumboNode* node)
    {
        auto e = node_as_element(node);
        if (e && e->children_count())
        {
            e = node_as_element(e->back_child());
            if (e && e->children_count()) return node_as_text(e->back_child());
        }
        return boost::none;
    }

}}
