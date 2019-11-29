#pragma once

#include <ostream>

#include <msw/assert.hpp>

#include <lama/gumbo/for_each.hpp>
#include <lama/gumbo/node_print_visitor.hpp>


namespace lama  {
namespace gumbo {

    inline void print_node_tree(GumboNode* node, std::ostream& s = std::cout, int max_depth = -1, int level_offset = 0, int tab_width = 2)
    {
        MSW_ASSERT(node);
        if ((max_depth != -1) && (level_offset > max_depth)) return;
        for_each_node_impl(node, node_print_visitor(s, tab_width), level_offset, max_depth);
    }

    template <typename Object>
    inline void print_object(Object const& o, std::ostream& s = std::cout, int level = 0, int tab_width = 2)
    {
        node_print_visitor(s, tab_width)(o, level);
    }
    template <typename Object>
    inline void print_object_tree(Object const& o, std::ostream& s = std::cout, int max_depth = -1, int level_offset = 0, int tab_width = 2)
    {
        if ((max_depth != -1) && (level_offset > max_depth)) return;
        print_object(o, s, level_offset, tab_width);
        level_offset++;
        for_each_child
        (
            o
            , [&s, max_depth, level_offset, tab_width](GumboNode* node)
            {
                print_node_tree(node, s, max_depth, level_offset, tab_width);
            }
        );
    }

}}
