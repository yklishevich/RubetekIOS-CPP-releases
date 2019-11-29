#pragma once

#include <array>
#include <string>

#include <rubetek/config.hpp>

#ifdef SMART_HOUSE_WINDOWS
	#include <windows.h>
#else
	#include <unistd.h>
#endif


namespace rubetek {
namespace utility {

	std::string self_exe_name(bool& error)	;
	std::string self_exe_name()				;

}}


namespace rubetek {
namespace utility {

	inline std::string self_exe_name(bool& error)
	{
		std::array<char, 4096> filename;
		int error_code =
		#ifdef SMART_HOUSE_WINDOWS
			::GetModuleFileNameA(0
		#else
			readlink("/proc/self/exe"
		#endif
			, filename.data(), static_cast<unsigned>(filename.size()));
		error =
		#ifdef SMART_HOUSE_WINDOWS
			false;
			error_code;
		#else
			error_code == -1;
		#endif
	    return error ? std::string() : std::string(filename.data());
	}

	inline std::string self_exe_name()
	{
		bool error;
		std::string result = self_exe_name(error);
		if (error) throw std::runtime_error("can't get self exe name");
		return result;
	}

}}
