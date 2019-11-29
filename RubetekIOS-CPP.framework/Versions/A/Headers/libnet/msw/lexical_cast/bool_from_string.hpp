#pragma once 

#include <string>

#include <msw/throw_runtime_error.hpp>


namespace msw
{
    inline bool bool_from_string(std::string const& s)
    {
             if (s == "1") return true  ;
        else if (s == "0") return false ;
#define MSW_BOOL_FROM_STRING(T, F)\
        else if (s == #T) return true  ;\
        else if (s == #F) return false ;
        MSW_BOOL_FROM_STRING( yes     , no      )
        MSW_BOOL_FROM_STRING( on      , off     )
        MSW_BOOL_FROM_STRING( ok      , bad     )
        MSW_BOOL_FROM_STRING( good    , bad     )
        MSW_BOOL_FROM_STRING( true    , false   )
        MSW_BOOL_FROM_STRING( enable  , disable )
        MSW_BOOL_FROM_STRING( success , failure )
        MSW_BOOL_FROM_STRING( +       , -       )
#undef MSW_BOOL_FROM_STRING
        throw_runtime_error("bad cast<bool>: '", s, "'");
        return false;
    }
}
