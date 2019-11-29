#pragma once

#include <string>

#include <boost/optional.hpp>
#include <boost/filesystem.hpp>

#include <msw/format.hpp>
#include <msw/throw_runtime_error.hpp>


namespace msw  {
namespace file {

    inline void copy(boost::filesystem::path const& src, boost::filesystem::path const& dst, boost::optional<std::string>& err)
    {
        if (!boost::filesystem::exists(src))
        {
            err = msw::format("source '", src, "' does not exist");
            return;
        }
        boost::system::error_code ec;
        if (boost::filesystem::is_regular_file(src))
        {
            boost::filesystem::copy_file(src, dst, ec);
            if (ec)
            {
                err = ec.message();
                return;
            }
        }
        else if (boost::filesystem::is_directory(src))
        {
            boost::filesystem::create_directories(dst, ec);
            if (ec)
            {
                err = ec.message();
                return;
            }
            for (boost::filesystem::directory_iterator i(src); i != boost::filesystem::directory_iterator(); ++i)
            {
                boost::filesystem::path const sub_src(i->path());
                boost::filesystem::path const sub_dst(dst / sub_src.filename());
                copy(sub_src, sub_dst, err);
                if (err) return;
            }
        }
    }

    inline void copy(boost::filesystem::path const& src, boost::filesystem::path const& dst)
    {
        boost::optional<std::string> err;
        copy(src, dst, err);
        if (err) msw::throw_runtime_error(*err);
    }

}}
