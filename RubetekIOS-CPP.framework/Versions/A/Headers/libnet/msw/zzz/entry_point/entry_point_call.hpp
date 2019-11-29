#pragma once

#include <msw/config.hpp>
#include <msw/zzz/entry_point/entry_point_main.hpp>


#ifdef MSW_NO_FORWARD_AS_TUPLE
#   define MSW_ENTRY_POINT(Type, ...)\
        template <typename ...Args>\
        int msw::entry_point::zzz::call(Args... args)\
        {\
            entry_point_main<Type>(std::make_tuple(args...), __VA_ARGS__);\
            return 0;\
        }
#else
#   define MSW_ENTRY_POINT(Type, ...)\
        template <typename ...Args>\
        int msw::entry_point::zzz::call(Args... args)\
        {\
            entry_point_main<Type>(std::forward_as_tuple(args...), __VA_ARGS__);\
            return 0;\
        }
#endif
