#pragma once

#include <boost/filesystem.hpp>


namespace msw  {
namespace file {

    inline void remove(boost::filesystem::path const& src, boost::system::error_code& ec)
    {
        boost::filesystem::remove_all(src, ec);
    }

    inline void remove(boost::filesystem::path const& src)
    {
        boost::filesystem::remove_all(src);
    }

}}
