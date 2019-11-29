#pragma once

#include <rubetek/utility/ctti.hpp>
#include <rubetek/utility/package_std.hpp>
#include <rubetek/utility/multiform/multiform.hpp>


namespace rubetek
{
    template <typename Visitor>
    void apply_multiform_visitor(multiform const& mf, Visitor& v)
    {
        if (mf.ctti_typeid() == ctti::global_typeid<bool>()) v(mf.unpack<bool>());
#define APPLY_MULTIFORM_VISITOR_ITEM(T)\
        else if (mf.ctti_typeid() == ctti::global_typeid<T>()) v(mf.unpack<T>());
        APPLY_MULTIFORM_VISITOR_ITEM( char               )
        APPLY_MULTIFORM_VISITOR_ITEM( signed char        )
        APPLY_MULTIFORM_VISITOR_ITEM( unsigned char      )
        APPLY_MULTIFORM_VISITOR_ITEM( short              )
        APPLY_MULTIFORM_VISITOR_ITEM( unsigned short     )
        APPLY_MULTIFORM_VISITOR_ITEM( int                )
        APPLY_MULTIFORM_VISITOR_ITEM( unsigned int       )
        APPLY_MULTIFORM_VISITOR_ITEM( long               )
        APPLY_MULTIFORM_VISITOR_ITEM( unsigned long      )
        APPLY_MULTIFORM_VISITOR_ITEM( long long          )
        APPLY_MULTIFORM_VISITOR_ITEM( unsigned long long )
        APPLY_MULTIFORM_VISITOR_ITEM( float              )
        APPLY_MULTIFORM_VISITOR_ITEM( std::string        )
        else throw std::runtime_error("can't apply multiform visitor");
#undef APPLY_MULTIFORM_VISITOR_ITEM
    }
}
