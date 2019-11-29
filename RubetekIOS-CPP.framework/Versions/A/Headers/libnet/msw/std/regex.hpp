#pragma once

#include <msw/config.hpp>

#ifdef MSW_BOOST_REGEX
#   include <boost/regex.hpp>
#else
#   include <regex>
#endif


namespace msw
{
#ifdef MSW_BOOST_REGEX
    using     boost::regex         ;
    using     boost::cmatch        ;
    using     boost::smatch        ;
    using     boost::csub_match    ;
    using     boost::ssub_match    ;
    using     boost::basic_regex   ;
    using     boost::regex_match   ;
    using     boost::regex_search  ;
    using     boost::match_results ;
    namespace regex_constants = boost::regex_constants;
#else
    using     std::regex         ;
    using     std::cmatch        ;
    using     std::smatch        ;
    using     std::csub_match    ;
    using     std::ssub_match    ;
    using     std::basic_regex   ;
    using     std::regex_match   ;
    using     std::regex_search  ;
    using     std::match_results ;
    namespace regex_constants = std::regex_constants;
#endif
}
