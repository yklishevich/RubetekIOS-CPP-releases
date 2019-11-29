#pragma once

#include <string>

#include <boost/format.hpp>


namespace rubetek {
namespace format {

							std::string ip_port	(std::string const& ip, unsigned short port)	;
	template <typename T>	std::string hex		(T)												;

}}


namespace rubetek {
namespace format {

	inline std::string ip_port(std::string const& ip, unsigned short port)
	{
		return ip + ":" + std::to_string(port);
	}

	template <typename T>
	std::string hex(T v)
	{
		return (boost::format("%1%") % boost::io::group(std::hex, std::showbase, v)).str();
	}


}}