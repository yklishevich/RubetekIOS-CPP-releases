#pragma once

#include <msw/config.hpp>
#include <msw/noncopyable.hpp>

#include <rubetek/socket/tcp_connector.hpp>


namespace rubetek
{
	struct tcp_connector_single
		: msw::noncopyable
	{
		typedef tcp_connector::on_connected			on_connected		;
		typedef tcp_connector::on_connection_fail	on_connection_fail	;

		explicit	tcp_connector_single	(boost::asio::io_service&)													;
					~tcp_connector_single	()																			;

		void		make					(boost::asio::ip::tcp::endpoint, on_connected, on_connection_fail)			;
		void		make					(boost::asio::ip::tcp::resolver::query, on_connected, on_connection_fail)	;

	private:

		boost::asio::io_service&	io_service_		;
		tcp_connector::ptr			tcp_connector_	;
	};
}


namespace rubetek
{
	inline tcp_connector_single::tcp_connector_single(boost::asio::io_service& io_service)
		: io_service_(io_service)
	{}
	inline tcp_connector_single::~tcp_connector_single()
	{
		if (tcp_connector_) tcp_connector_->close();
	}

	inline void tcp_connector_single::make(boost::asio::ip::tcp::endpoint endpoint, on_connected on_connected, on_connection_fail on_connection_fail)
	{
		if (tcp_connector_) throw std::runtime_error("tcp connector already exists");
		tcp_connector_ = tcp_connector::make
			(
				io_service_
				, endpoint
				, on_connected
				, [this](tcp_connector::ptr)
				{
					tcp_connector_.reset();
				}
				, on_connection_fail
			);
	}
	inline void tcp_connector_single::make(boost::asio::ip::tcp::resolver::query query, on_connected on_connected, on_connection_fail on_connection_fail)
	{
		if (tcp_connector_) throw std::runtime_error("tcp connector already exists");
		tcp_connector_ = tcp_connector::make
			(
				io_service_
				, query
				, on_connected
				, [this](tcp_connector::ptr)
				{
					tcp_connector_.reset();
				}
				, on_connection_fail
			);
	}
}
