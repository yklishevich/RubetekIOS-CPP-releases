#pragma once

#include <fstream>

#include <msw/throw_runtime_error.hpp>

#include <dawn/salix/read_stream.hpp>


namespace dawn  {
namespace salix {

    inline tree read_file(std::string const& filename)
    {
        std::ifstream file(filename);
        if (!file.good()) msw::throw_runtime_error("can't open file: '", filename, "'");
        return read_stream(file);
    }

}}
