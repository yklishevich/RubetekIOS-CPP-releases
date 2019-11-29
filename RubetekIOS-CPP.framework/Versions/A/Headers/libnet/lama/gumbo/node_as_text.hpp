#pragma once

#include <boost/optional.hpp>

#include <lama/gumbo/text.hpp>
#include <lama/gumbo/include/gumbo.hpp>


namespace lama  {
namespace gumbo {

    inline boost::optional<text> node_as_text(GumboNode* node)
    {
        if (node->type == GUMBO_NODE_TEXT) return text(&node->v.text);
        return boost::none;
    }

}}
