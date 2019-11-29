#pragma once

#include <string>


namespace rubetek {
namespace utility {

	std::string normilize_path(std::string);

}}


namespace rubetek {
namespace utility {

	inline std::string normilize_path(std::string path)
	{
		if (path.empty()) throw std::runtime_error("can't normilize path: it is empty");
		char const last_symbol = *--path.end();
		if (last_symbol != '/' || last_symbol != '\\') path += '/';
		return path;
	}

}}