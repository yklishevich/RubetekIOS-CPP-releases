#pragma once

#include <string>
#include <fstream>

#include <msw/throw_runtime_error.hpp>


namespace rubetek    {
namespace controller {

    inline std::string read_password(std::string const& filename)
    {
        std::ifstream unique_id_file(filename);
        if (!unique_id_file.good()) msw::throw_runtime_error("can't open file with password: ", filename);
        std::string password;
        unique_id_file >> password;
        return std::move(password);
    }

}}
