#pragma once


namespace msw
{
    template <typename ClassType>
    struct safe_bool
    {
        typedef void (safe_bool::*type)();
        void true_() {}
    };

    template <typename ClassType>
    typename safe_bool<ClassType>::type make_safe_bool(bool value)
    {
        return value ? &safe_bool<ClassType>::true_ : false;
    }
}
