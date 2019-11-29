#pragma once

#include <boost/filesystem.hpp>

#include <msw/buffer.hpp>
#include <msw/file/optimum_ofile.hpp>

#include <lama/libzip/unpacker.hpp>


namespace lama {
namespace zipp {

    inline void unpack_all(std::string const& file_name, std::string const& out_path)
    {
        unpacker unpkr(file_name);
        int const files_count = unpkr.files_count();
        msw::buffer<msw::byte> buf;
        for (int i = 0; i != files_count; ++i)
        {
            file_info const fi = unpkr.get_file_info(i);
            msw::size<msw::byte> const file_size(static_cast<msw::size<msw::byte>::count_type>(fi.size));
            buf.resize(file_size);
            std::string const full_file_name = out_path + fi.name;
            if (!full_file_name.empty() && (full_file_name.back() != '/') && (full_file_name.back() != '\\'))
            {
                boost::filesystem::create_directories(boost::filesystem::path(full_file_name).parent_path().string());
                write_to_file(full_file_name, unpkr.extract(i, buf.all()));
            }
        }
    }

}}
