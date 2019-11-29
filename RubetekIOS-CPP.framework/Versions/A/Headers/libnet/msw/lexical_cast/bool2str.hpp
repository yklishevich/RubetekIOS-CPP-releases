#pragma once 


namespace msw
{
#define MSW_BOOL2STR(V1, V2)\
    inline char const* V1##_##V2(bool v)\
    {\
        static char const v1[] = #V1;\
        static char const v0[] = #V2;\
        return v ? v1 : v0;\
    }
    MSW_BOOL2STR( yes     , no      )
    MSW_BOOL2STR( on      , off     )
    MSW_BOOL2STR( ok      , bad     )
    MSW_BOOL2STR( good    , bad     )
    MSW_BOOL2STR( true    , false   )
    MSW_BOOL2STR( enable  , disable )
    MSW_BOOL2STR( success , failure )
#undef MSW_BOOL2STR
    inline char const* plus_minus(bool v)
    {
        static char const v1[] = "+";
        static char const v0[] = "-";
        return v ? v1 : v0;
    }
}
