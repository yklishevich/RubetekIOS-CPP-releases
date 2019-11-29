#pragma once

#include <type_traits>

#include <msw/config.hpp>
#ifdef MSW_NO_RESULT_OF
#   include <boost/utility/result_of.hpp>
#endif


namespace msw         {
namespace entry_point {
namespace zzz         {

    template <typename Fn>
    struct is_procedure
        : std::is_same
        <
#ifdef MSW_NO_RESULT_OF
        typename boost::result_of<Fn>::type
#else
        typename std::result_of<Fn>::type
#endif
        , void
        >
    {};

}}}
