#pragma once

#include <msw/on_debug.hpp>
#include <msw/zzz/position/fwd.hpp>


namespace msw {
namespace zzz {

    template <typename Pointer MSW_ON_DEBUG(, typename Validator)> position<typename Pointer::value_type> construct_position(Pointer MSW_ON_DEBUG(, Validator&&));

}}
