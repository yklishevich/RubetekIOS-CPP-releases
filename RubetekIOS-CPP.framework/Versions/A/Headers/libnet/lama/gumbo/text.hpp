#pragma once

#include <msw/range.hpp>
#include <msw/assert.hpp>

#include <lama/gumbo/include/gumbo.hpp>
#include <lama/gumbo/impl/make_range_string.hpp>


namespace lama  {
namespace gumbo {

    struct text
    {
        explicit text(GumboText* t)
            : t_(t)
        {
            MSW_ASSERT(t_);
        }

        char const* value() const
        {
            return t_->text;
        }
        msw::range<char const> origianl() const
        {
            return impl::make_range_string(t_->original_text);
        }

    private:
        GumboText* t_;
    };

}}
