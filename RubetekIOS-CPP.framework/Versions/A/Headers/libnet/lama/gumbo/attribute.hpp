#pragma once

#include <msw/assert.hpp>

#include <lama/gumbo/include/gumbo.hpp>
#include <lama/gumbo/impl/make_range_string.hpp>


namespace lama  {
namespace gumbo {

    struct attribute
    {
        explicit attribute(GumboAttribute* a)
            : a_(a)
        {
            MSW_ASSERT(a_);
        }

        char const* name() const
        {
            return a_->name;
        }
        msw::range<char const> original_name() const
        {
            return impl::make_range_string(a_->original_name);
        }
        char const* value() const
        {
            return a_->value;
        }
        msw::range<char const> original_value() const
        {
            return impl::make_range_string(a_->original_value);
        }
    private:
        GumboAttribute* a_;
    };

}}
