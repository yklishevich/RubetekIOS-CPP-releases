#pragma once

#include <msw/make_range_native.hpp>

#include <lama/gumbo/include/gumbo.hpp>


namespace lama  {
namespace gumbo {
namespace impl  {

    inline msw::range<char const> make_range_string(GumboStringPiece s)
    {
        return (s.length && (s.data != nullptr)) ? msw::make_range_native(s.data, s.length) : msw::range<char const>();
    }

}}}
