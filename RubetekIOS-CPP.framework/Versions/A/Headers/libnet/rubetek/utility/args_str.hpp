#pragma once

#include <string>


namespace rubetek {
namespace utility {

	struct args_str
	{
		template <typename Arg>
		static std::string make(Arg const& arg)
		{
			return arg;
		}
		template <typename Arg, typename ...Args>
		static std::string make(Arg const& arg, Args const&... args)
		{
			std::string const right = make(args...);
			return make(arg) + (right.empty() ? std::string() : (" " + right));
		}
	};

}}
