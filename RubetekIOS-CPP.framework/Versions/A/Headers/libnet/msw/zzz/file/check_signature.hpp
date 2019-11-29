#pragma once

#include <string>

#include <msw/range.hpp>
#include <msw/plain_types.hpp>        // byte
#include <msw/algorithm/equal.hpp>
#include <msw/file/optimum_ifile.hpp>


namespace msw  {
namespace zzz  {
namespace file {

    inline bool check_signature(std::string const& file_name, msw::range<msw::byte const> signature)
    {
        msw::optimum_ifile ifile_(file_name);
        return equal(ifile_.read(signature.size()), signature);
    }

}}}
