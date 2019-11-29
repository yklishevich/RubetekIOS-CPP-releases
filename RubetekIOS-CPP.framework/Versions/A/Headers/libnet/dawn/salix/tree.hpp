#pragma once

#include <list>
#include <iostream>
#include <unordered_map>

#include <boost/optional.hpp>

#include <msw/format.hpp>
#include <msw/throw_runtime_error.hpp>

#include <dawn/tree.hpp>
#include <dawn/salix/node.hpp>


namespace dawn  {
namespace salix {

    typedef tree<node> tree;

}}


namespace dawn  {
namespace salix {

    inline void print_each_node(tree const& t, std::ostream& s = std::cout)
    {
        for_each_tree_value
        (
            t
            , [&](dawn::salix::node const& node, msw::uint level)
            {
                for (msw::uint j = 0; j != level; ++j) msw::format_s(s, "  ");
                    msw::format_s(s, node.key(), ": ", node.value(), msw::nl);
            }
        );
    }
    template <typename Tree>
    bool has_child(Tree&& tree, std::string const& key)
    {
        for (auto& t : tree.child)
            if (t.value.key() == key) return true;
        return false;
    }
    template <typename Tree>
    Tree& get_child(Tree&& tree, std::string const& key)
    {
        for (auto& t : tree.child)
            if (t.value.key() == key) return t;
        msw::throw_runtime_error("key '", key, "' not found");
#include <msw/throw_hide_warning.hpp>
    }
    template <typename Tree>
    std::string get_child_value(Tree&& tree, std::string const& key)
    {
        return get_child(std::forward<Tree>(tree), key).value.value();
    }
    template <typename Tree>
    boost::optional<std::string> get_child_optional_value(Tree&& tree, std::string const& key)
    {
        if (has_child(std::forward<Tree>(tree), key))
        {
            auto const& sub_tree = get_child(std::forward<Tree>(tree), key);
            return sub_tree.value.value();
        }
        return boost::none;
    }
    template <typename Tree>
    std::list<std::string> get_child_key_list(Tree&& tree)
    {
        std::list<std::string> lst;
        dawn::flat_for_each_tree_value
        (
            std::forward<Tree>(tree)
            , [&lst](dawn::salix::node const& node)
            {
                lst.push_back(node.key());
            }
        );
        return std::move(lst);
    }
    template <typename Tree>
    std::unordered_map<std::string, std::string> get_child_key_value_map(Tree&& tree)
    {
        std::unordered_map<std::string, std::string> map;
        msw::flat_for_each_tree_value
        (
            std::forward<Tree>(tree)
            , [&map](dawn::salix::node const& node)
            {
                map[node.key()] = node.value();
            }
        );
        return std::move(map);
    }

}}
