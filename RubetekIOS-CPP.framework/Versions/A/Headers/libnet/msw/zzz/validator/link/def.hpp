#pragma once

#include <msw/zzz/validator/link/decl.hpp>


namespace msw {
namespace zzz {

    inline validator_link::validator_link()
    {}

    inline validator_link::validator_link(validator_link const& other)
        : valid_(other.valid_)
    {}
    inline validator_link& validator_link::operator = (validator_link const& other)
    {
        valid_ = other.valid_;
        return *this;
    }

    inline validator_link::validator_link(validator_link&& other)
        : valid_(std::move(other.valid_))
    {}
    inline validator_link& validator_link::operator = (validator_link&& other)
    {
        valid_ = std::move(other.valid_);
        return *this;
    }

    inline bool validator_link::check() const
    {
        return valid_ ? *valid_ : true;
    }
    inline bool validator_link::same_root(validator_link const& other) const
    {
        return valid_ == other.valid_;
    }

    inline validator_link::validator_link(valid_type const& valid)
        : valid_(valid)
    {}
    inline validator_link::validator_link(fake)
        : valid_(new bool(true))
    {}

}}
