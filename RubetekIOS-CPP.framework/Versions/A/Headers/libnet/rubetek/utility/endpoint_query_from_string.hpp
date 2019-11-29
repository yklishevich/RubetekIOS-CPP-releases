#pragma once

#include <string>

#include <boost/lexical_cast.hpp>

#include <msw/std/regex.hpp>
#include <msw/include/boost/asio.hpp>
#include <msw/throw_runtime_error.hpp>


namespace rubetek
{
	inline boost::asio::ip::tcp::resolver::query endpoint_query_from_string(std::string const& address, msw::wbyte default_port)
	{
		static msw::regex const regex_address("(.*?)(?::(\\d+))?");
		msw::smatch what;
		if (!msw::regex_match(address, what, regex_address)) msw::throw_runtime_error("address incorrect: ", address);
		return boost::asio::ip::tcp::resolver::query
		(
			what[1]
			, std::to_string(std::string(what[2]).empty() ? default_port : boost::lexical_cast<msw::wbyte>(what[2]))
		);
	}
}
