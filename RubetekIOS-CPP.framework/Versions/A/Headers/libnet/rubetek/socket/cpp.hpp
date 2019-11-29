#pragma once

#include <rubetek/socket/tcp_connector.hpp>


namespace rubetek
{
	std::chrono::steady_clock::duration const tcp_connector::default_reconnect_interval = std::chrono::seconds(5);
}
