#pragma once

#include <msw/asio/udp_point.hpp>
#include <msw/lexical_cast/bool2str.hpp>

#include <rubetek/log.hpp>


namespace rubetek
{
	struct udp_socket
		: msw::noncopyable
	{
		typedef msw::udp_point::on_received   on_received   ;
		typedef msw::udp_point::endpoint_type endpoint_type ;

				udp_socket		(boost::asio::io_service&, msw::wbyte port, on_received, bool reuse_address = false)	;

		void	enable_broadcast(bool = true)																			;
		void	send_to			(endpoint_type, msw::range<msw::byte const>)											;

	private:
		logger                      const logger_      ;
		logger_io<log_io_tags::udp> const logger_io_   ;
		on_received                 const on_received_ ;
		msw::udp_point                    sock_        ;
	};
}


namespace rubetek
{
	inline udp_socket::udp_socket(boost::asio::io_service& io_service, msw::wbyte port, on_received on_received, bool reuse_address)
		: logger_      ("udp socket", std::string("0.0.0.0:") + std::to_string(port))
		, logger_io_   (logger_)
		, on_received_ (on_received)
		, sock_
		(
			io_service
			, port
			, [this](endpoint_type remote_endpoint, msw::range<msw::byte> packet)
			{
				logger_io_.received(packet, remote_endpoint);
				try
				{
					on_received_(remote_endpoint, packet);
				}
				catch (std::exception const& e)
				{
					logger_.error("process packet: ", e.what());
				}
			}
			, [this](boost::system::error_code ec)
			{
				if (ec == boost::system::errc::connection_reset) logger_.debug("connection reset: ", ec.message());
				else logger_.error(ec.message());
			}
			, reuse_address
		)
	{
		logger_.debug("reuse address: ", msw::yes_no(reuse_address));
	}

	inline void udp_socket::enable_broadcast(bool enable)
	{
		sock_.broadcast(enable);
	}
	inline void udp_socket::send_to(endpoint_type endpoint, msw::range<msw::byte const> packet)
	{
		boost::system::error_code ec;
		sock_.send_to(endpoint, packet, ec);
		if (ec) logger_.error("sent failed: ", ec.message());
		else logger_io_.sent(packet, endpoint);
	}
}