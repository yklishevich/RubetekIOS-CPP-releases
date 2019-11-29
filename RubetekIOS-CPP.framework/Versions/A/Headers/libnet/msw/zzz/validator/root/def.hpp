#pragma once

#include <msw/zzz/validator/root/decl.hpp>
#include <msw/zzz/validator/link/def.hpp>


namespace msw {
namespace zzz {

    inline validator_root::validator_root()
    {}
    inline validator_root::validator_root(init)
        : valid_(new bool(true))
    {}
    inline validator_root::~validator_root()
    {
        if (valid_) *valid_ = false;
    }

    inline validator_root::validator_root(validator_root&& other)
        : valid_(std::move(other.valid_))
    {}
    inline validator_root& validator_root::operator = (validator_root&& other)
    {
        valid_ = std::move(other.valid_);
        return *this;
    }

    inline void validator_root::change()
    {
        if (valid_) *valid_ = false;
        valid_.reset(new bool(true));
    }

    inline validator_link validator_root::make_link() const
    {
        return validator_link(valid_);
    }

    inline void validator_root::swap(validator_root& other)
    {
        valid_.swap(other.valid_);
    }
}}
