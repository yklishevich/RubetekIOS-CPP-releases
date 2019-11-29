#pragma once

#include <type_traits>              // std::remove_reference

#include <msw/on_debug.hpp>
#include <msw/zzz/range/fwd.hpp>


namespace msw {
namespace zzz {

    struct construct_range_tag{};

    template <typename Block MSW_ON_DEBUG(, typename Validator)> range<typename std::remove_reference<Block>::type::value_type> construct_range(Block&& MSW_ON_DEBUG(, Validator&&));

}}
