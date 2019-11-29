#pragma once

#include <list>

#include <msw/int.hpp>


namespace dawn
{
    template <typename T>
    struct tree
    {
        typedef T                    value_type ;
        typedef tree<value_type>     self_type  ;
        typedef std::list<self_type> child_type ;

        tree()
        {}
        explicit tree(value_type const& v)
            : value(v)
        {}
        explicit tree(value_type&& v)
            : value(std::move(v))
        {}

        bool has_child() const
        {
            return !child.empty();
        }

        value_type value;
        child_type child;
    };
}


namespace dawn {
namespace zzz  {

    template <typename Tree, typename Fn>
    void for_each_tree(Tree&& tree, Fn&& fn, msw::uint level = 0)
    {
        msw::uint const next_level = level + 1;
        for (auto& i : tree.child)
        {
            fn(i, level);
            zzz::for_each_tree(i, std::forward<Fn>(fn), next_level);
        }
    }

}}


namespace dawn
{
    template <typename Tree, typename Fn>
    void for_each_tree(Tree&& tree, Fn&& fn)
    {
        zzz::for_each_tree(std::forward<Tree>(tree), std::forward<Fn>(fn));
    }
    template <typename Tree, typename Fn>
    void for_each_tree_value(Tree&& tree, Fn&& fn)
    {
        zzz::for_each_tree
        (
            std::forward<Tree>(tree)
            , [fn](Tree&& tree, msw::uint level)
            {
                fn(tree.value, level);
            }
        );
    }
    template <typename Tree, typename Fn>
    void flat_for_each_tree(Tree&& tree, Fn&& fn)
    {
        for (auto& i : tree.child) fn(i);
    }
    template <typename Tree, typename Fn>
    void flat_for_each_tree_value(Tree&& tree, Fn&& fn)
    {
        flat_for_each_tree
        (
            std::forward<Tree>(tree)
            , [fn](Tree&& tree)
            {
                fn(tree.value);
            }
        );
    }
}
