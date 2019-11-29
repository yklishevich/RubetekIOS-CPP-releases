#pragma once

#include <boost/variant/static_visitor.hpp>

#include <rubetek/utility/multiform/multiform.hpp>


namespace rubetek
{
    struct multiform_from_variant_visitor
        : boost::static_visitor<multiform>
    {
        template <typename T>
        multiform operator () (T const&);
    };

    template <typename Variant>
    multiform multiform_from_variant(Variant const&);
}


namespace rubetek
{
    template <typename T>
    multiform multiform_from_variant_visitor::operator () (T const& v)
    {
        return multiform(v);
    }
    template <typename Variant>
    multiform multiform_from_variant(Variant const& v)
    {
        multiform_from_variant_visitor visitor;
        return v.apply_visitor(visitor);
    }
}
