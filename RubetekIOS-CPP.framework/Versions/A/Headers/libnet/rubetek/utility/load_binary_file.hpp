#pragma once

#include <string>
#include <fstream>

#include <rubetek/essence/buffer.hpp>


namespace rubetek {
namespace utility {

	buffer load_binary_file(std::string const& file_name);

}}


namespace rubetek {
namespace utility {

	inline buffer load_binary_file(std::string const& file_name)
	{
		buffer buf;
		buf.reserve(10 * 1024 * 1024);
		std::ifstream ifile(file_name.c_str(), std::ios::binary);
		if (!ifile.good()) throw std::runtime_error("can't open file: " + file_name);
		buffer block(1024 * 1024);
		do
		{
			ifile.read(reinterpret_cast<char*>(block.data()), block.size());
			buf.insert(buf.end(), block.begin(), block.begin() + static_cast<std::size_t>(ifile.gcount()));
		}
		while (static_cast<buffer::size_type>(ifile.gcount()) == block.size());
		return std::move(buf);
	}

}}
