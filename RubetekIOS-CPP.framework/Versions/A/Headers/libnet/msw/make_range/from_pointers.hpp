#pragma once

#include <type_traits>                          // std::enable_if, std::remove_const

#include <msw/zzz/validator/link/make_fake.hpp>
#include <msw/zzz/range/def.hpp>
#include <msw/zzz/block/make.hpp>


namespace msw
{
    template <typename X1, typename X2>
    auto make_range(X1 x1, X2 x2) -> decltype(zzz::construct_range(zzz::make_block(x1, x2) MSW_ON_DEBUG(, zzz::make_fake_validator())))
    {
        return zzz::construct_range(zzz::make_block(x1, x2) MSW_ON_DEBUG(, zzz::make_fake_validator()));
    }
}
