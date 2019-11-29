#pragma once

#include <msw/zzz/validator/link/def.hpp>


namespace msw {
namespace zzz {

    inline validator_link make_fake_validator()
    {
        return validator_link(validator_link::fake());
    }

}}
