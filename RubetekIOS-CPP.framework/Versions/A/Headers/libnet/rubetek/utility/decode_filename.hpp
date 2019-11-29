#pragma once

#include <string>

#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string/replace.hpp>

#include <msw/self/pid.hpp>
#include <msw/decode_filename.hpp>


namespace rubetek
{
	inline std::string decode_filename(std::string filename, std::string what = "$(PID)", std::string replacer = boost::lexical_cast<std::string>(msw::self_pid()))
	{
		filename = msw::decode_filename(filename);
		boost::algorithm::replace_all(filename, what, replacer);
		return filename;
	}
}
