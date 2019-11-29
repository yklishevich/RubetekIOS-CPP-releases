#pragma once

#include <string>

#include <msw/buffer.hpp>
#include <msw/file/ifile.hpp>

#include <lama/boost/include/crc.hpp>


namespace msw  {
namespace file {

    template <typename CRC_Computer = boost::crc_32_type>
    typename CRC_Computer::value_type crc(std::string const& file_name, CRC_Computer&& computer = CRC_Computer())
    {
        buffer<byte> file = read_file(file_name);
        computer.process_bytes(file.data().native(), file.size().count());
        return computer.checksum();
    }

}}
