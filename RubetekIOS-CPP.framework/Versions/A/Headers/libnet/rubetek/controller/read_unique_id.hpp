#pragma once

#include <string>
#include <fstream>

#include <boost/algorithm/string/case_conv.hpp>

#include <msw/throw_runtime_error.hpp>

#include <rubetek/essence/unique_id/controller.hpp>


namespace rubetek    {
namespace controller {

    inline unique_controller_id::type read_unique_id(std::string const& filename)
    {
        std::ifstream unique_id_file(filename);
        if (!unique_id_file.good()) msw::throw_runtime_error("can't open file with unique id: '", filename, "'");
        unique_controller_id::type unique_id;
        unique_id_file >> unique_id;
        boost::to_lower(unique_id);
        return std::move(unique_id);
    }

}}
