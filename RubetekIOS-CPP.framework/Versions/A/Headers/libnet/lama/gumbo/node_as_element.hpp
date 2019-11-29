#pragma once

#include <boost/optional.hpp>

#include <lama/gumbo/element.hpp>
#include <lama/gumbo/include/gumbo.hpp>


namespace lama  {
namespace gumbo {

    inline boost::optional<element> node_as_element(GumboNode* node)
    {
        if (node->type == GUMBO_NODE_ELEMENT) return element(&node->v.element);
        return boost::none;
    }

}}
