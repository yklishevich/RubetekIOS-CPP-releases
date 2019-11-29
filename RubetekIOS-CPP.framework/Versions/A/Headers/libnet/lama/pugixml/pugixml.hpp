#pragma once

#include <string>
#include <functional>

#include <boost/optional.hpp>

#include <pugixml/pugixml.hpp>


namespace msw     {
namespace pugixml {

    template <typename Fn>
    void for_each(pugi::xml_node const& root_node, char const* child_name, Fn fn)
    {
        for (pugi::xml_node node = root_node.child(child_name); node; node = node.next_sibling(child_name))
            fn(node);
    }
    template <typename Fn>
    void for_each(pugi::xml_node const& root_node, std::string const& child_name, Fn fn)
    {
        for_each(root_node, child_name.c_str(), fn);
    }

    template <typename Fn>
    void for_each(pugi::xml_node const& root_node, std::string const& child_name, std::string const& attribute_name, boost::optional<std::string> const& attribute_value, Fn fn)
    {
        for_each
        (
            root_node
            , child_name
            , [&](pugi::xml_node const& node)
            {
                auto attr = node.attribute(attribute_name.c_str());
                if (attr)
                {
                    if (attribute_value)
                    {
                        std::string const val = attr.value();
                        if (val != attribute_value) return;
                    }
                    fn(node);
                }
            }
        );
    }
    template <typename Fn>
    void for_each(pugi::xml_node const& root_node, std::string const& child_name, std::string const& attribute_name, std::string const& attribute_value, Fn fn)
    {
        for_each(root_node, child_name, attribute_name, boost::optional<std::string>(attribute_value), fn);
    }

    namespace zzz
    {
        struct deep_walker
            : pugi::xml_tree_walker
        {
            typedef std::function<bool(pugi::xml_node&, unsigned depth)> node_ready;
            explicit deep_walker(node_ready node_ready)
                : node_ready_(node_ready)
            {}
            virtual bool for_each(pugi::xml_node& node)
            {
                return node_ready_(node, depth());
            }
        private:
            node_ready node_ready_;
        };
    }

    template <typename Fn>
    void for_each_deep(pugi::xml_node& root_node, Fn fn)
    {
        zzz::deep_walker walker(fn);
        root_node.traverse(walker);
    }
    template <typename Fn>
    void for_each_deep(pugi::xml_node& root_node, std::string const& node_name, std::string const& attribute_name, std::string const& attribute_value, Fn fn)
    {
        for_each_deep
        (
            root_node
            , [&](pugi::xml_node& node, unsigned depth)
            {
                if (node.name() == node_name)
                {
                    auto attribute = node.attribute(attribute_name.c_str());
                    if (attribute)
                    {
                        std::string const value = attribute.value();
                        if (value == attribute_value) fn(node, depth);
                    }
                }
                return true;
            }
        );
    }

    inline pugi::xml_node find_deep(pugi::xml_node& root_node, std::string const& node_name, std::string const& attribute_name, std::string const& attribute_value)
    {
        pugi::xml_node result;
        for_each_deep
        (
            root_node
            , [&](pugi::xml_node& node, unsigned)
            {
                if (node.name() == node_name)
                {
                    auto attribute = node.attribute(attribute_name.c_str());
                    if (attribute)
                    {
                        std::string const value = attribute.value();
                        if (value == attribute_value)
                        {
                            result = node;
                            return false;
                        }
                    }
                }
                return true;
            }
        );
        return result;
    }

}}
