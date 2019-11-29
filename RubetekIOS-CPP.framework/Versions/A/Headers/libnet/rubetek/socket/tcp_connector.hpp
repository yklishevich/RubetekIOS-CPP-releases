#pragma once

#include <memory>
#include <chrono>
#include <functional>

#include <boost/asio/steady_timer.hpp>

#include <msw/config.hpp>
#include <msw/buffer.hpp>
#include <msw/noncopyable.hpp>
#include <msw/include/boost/asio.hpp>
#include <msw/lexical_cast/network/query/to_string.hpp>

#include <rubetek/log.hpp>


namespace rubetek
{
	struct tcp_connector
		: msw::noncopyable
		, std::enable_shared_from_this<tcp_connector>
	{
		typedef std::shared_ptr<tcp_connector>						ptr					;
		typedef std::function<void(boost::asio::ip::tcp::socket)>	on_connected		;
		typedef std::function<void(ptr)>							on_close			;
		typedef std::function<void()>								on_connection_fail	;

		static std::chrono::steady_clock::duration const default_reconnect_interval;

				void							close				()																							;
		static	ptr								make				(
																		boost::asio::io_service&
																		, boost::asio::ip::tcp::endpoint
																		, on_connected
																		, on_close
																		, on_connection_fail										= on_connection_fail()
																		, std::chrono::steady_clock::duration reconnect_interval	= default_reconnect_interval
																	)																							;
		static	ptr								make				(
																		boost::asio::io_service&
																		, boost::asio::ip::tcp::resolver::query
																		, on_connected
																		, on_close
																		, on_connection_fail										= on_connection_fail()
																		, std::chrono::steady_clock::duration reconnect_interval	= default_reconnect_interval
																	)																							;
												~tcp_connector		()																							;
				boost::asio::ip::tcp::endpoint	endpoint			()																					const	;

	private:
												tcp_connector		(
																		boost::asio::io_service&
																		, boost::asio::ip::tcp::endpoint
																		, on_connected
																		, on_close
																		, on_connection_fail
																		, std::chrono::steady_clock::duration	reconnect_interval
																	)																							;
												tcp_connector		(
																		boost::asio::io_service&
																		, boost::asio::ip::tcp::resolver::query
																		, on_connected
																		, on_close
																		, on_connection_fail
																		, std::chrono::steady_clock::duration	reconnect_interval
																	)																							;
				void							start				()																							;
				void							connect				()																							;
				void							connect_to_endpoint	()																							;
				void							connect_to_iterator	(boost::asio::ip::tcp::resolver::iterator)													;
				void							resolve				()																							;
				void							on_fail				()																							;

				void							connect_handler		(std::shared_ptr<tcp_connector>, boost::system::error_code const&)							;

		logger									const	logger_				;
		boost::asio::ip::tcp::socket					socket_				;
		boost::asio::ip::tcp::endpoint			const	endpoint_			;
		boost::asio::ip::tcp::resolver::query	const	query_				;
		boost::asio::ip::tcp::resolver					resolver_			;
		on_connected									on_connected_		;
		on_close										on_close_			;
		boost::asio::steady_timer						timer_				;
		on_connection_fail								on_connection_fail_	;
		std::chrono::steady_clock::duration		const	reconnect_interval_	;
	};
}


namespace rubetek
{
	inline void tcp_connector::close()
	{
		if (on_close_)
		{
			timer_.cancel();
			socket_.close();
			logger_.debug("closed");
			on_close(std::move(on_close_))(shared_from_this());
		}
	}

	inline tcp_connector::ptr tcp_connector::make
	(
		boost::asio::io_service&				io_service
		, boost::asio::ip::tcp::endpoint		endpoint
		, on_connected							on_connected
		, on_close								on_close
		, on_connection_fail					on_connection_fail
		, std::chrono::steady_clock::duration	reconnect_interval
	)
	{
		ptr connector(new tcp_connector(io_service, endpoint, on_connected, on_close, on_connection_fail, reconnect_interval));
		connector->start();
		return connector;
	}
	inline tcp_connector::ptr tcp_connector::make
	(
		boost::asio::io_service&				io_service
		, boost::asio::ip::tcp::resolver::query	query
		, on_connected							on_connected
		, on_close								on_close
		, on_connection_fail					on_connection_fail
		, std::chrono::steady_clock::duration	reconnect_interval
	)
	{
		ptr connector(new tcp_connector(io_service, query, on_connected, on_close, on_connection_fail, reconnect_interval));
		connector->start();
		return connector;
	}
	inline tcp_connector::~tcp_connector()
	{}

	inline boost::asio::ip::tcp::endpoint tcp_connector::endpoint() const
	{
		return endpoint_;
	}

	inline tcp_connector::tcp_connector
	(
		boost::asio::io_service&				io_service
		, boost::asio::ip::tcp::resolver::query	query
		, on_connected							on_connected
		, on_close								on_close
		, on_connection_fail					on_connection_fail
		, std::chrono::steady_clock::duration	reconnect_interval
	)
		: logger_				("tcp connector", msw::network_query_to_string(query))
		, socket_				(io_service)
		, query_				(query)
		, resolver_				(io_service)
		, on_connected_			(on_connected)
		, on_close_				(on_close)
		, timer_				(io_service)
		, on_connection_fail_	(on_connection_fail)
		, reconnect_interval_	(reconnect_interval)
	{}
	inline tcp_connector::tcp_connector
	(
		boost::asio::io_service&				io_service
		, boost::asio::ip::tcp::endpoint		endpoint
		, on_connected							on_connected
		, on_close								on_close
		, on_connection_fail					on_connection_fail
		, std::chrono::steady_clock::duration	reconnect_interval
	)
		: logger_				("tcp connector", msw::network_endpoint_to_string(endpoint))
		, socket_				(io_service)
		, endpoint_				(endpoint)
		, query_				("")
		, resolver_				(io_service)
		, on_connected_			(on_connected)
		, on_close_				(on_close)
		, timer_				(io_service)
		, on_connection_fail_	(on_connection_fail)
		, reconnect_interval_	(reconnect_interval)
	{}

	inline void tcp_connector::start()
	{
		logger_.info("started");
		connect();
	}
	inline void tcp_connector::connect()
	{
		if (endpoint_ == boost::asio::ip::tcp::endpoint()) resolve();
		else connect_to_endpoint();
	}
	inline void tcp_connector::resolve()
	{
		try
		{
			auto self(shared_from_this());
			resolver_.async_resolve
			(
				query_
				, [this](const boost::system::error_code& ec, boost::asio::ip::tcp::resolver::iterator endpoint_iterator)
				{
					if (!ec)
					{
						connect_to_iterator(endpoint_iterator);
					}
					else
					{
						logger_.info("failed resolve");
						on_fail();
					}
				}
			);
		}
		catch (std::exception const& e)
		{
			logger_.error("resolve fail: ", e.what());
			on_fail();
		}
	}
	inline void tcp_connector::connect_to_endpoint()
	{
		try
		{
			auto self(shared_from_this());
			socket_.async_connect
			(
				endpoint_
#ifdef MSW_NO_LAMBDA
				, std::bind(&tcp_connector_set::connect_handler, this, self, std::placeholders::_1)
#else
				, [this, self](boost::system::error_code const& ec)
				{
					connect_handler(self, ec);
				}
#endif
			);
		}
		catch (std::exception const& e)
		{
			logger_.error("connect fail: ", e.what());
			on_fail();
		}
	}
	inline void tcp_connector::connect_to_iterator(boost::asio::ip::tcp::resolver::iterator iterator)
	{
		try
		{
			auto self(shared_from_this());
			boost::asio::async_connect
			(
				socket_
				, iterator
				, [this, self](boost::system::error_code const& ec, boost::asio::ip::tcp::resolver::iterator)
				{
					connect_handler(self, ec);
				}
			);
		}
		catch (std::exception const& e)
		{
			logger_.error("connect fail: ", e.what());
			on_fail();
		}
	}
	inline void tcp_connector::on_fail()
	{
		if (on_connection_fail_)
		{
			on_connection_fail_();
			timer_.expires_from_now(reconnect_interval_);
			auto self(shared_from_this());
			timer_.async_wait([this, self](boost::system::error_code const& ec)
			{
				if (ec) logger_.debug("timer fail: ", ec.message());
				else connect();
			});
		}
		else close();
	}

	inline void tcp_connector::connect_handler(std::shared_ptr<tcp_connector>, boost::system::error_code const& ec)
	{
		bool connected = !ec;
#ifdef MSW_LINUX
		if (connected)
		{
			boost::system::error_code ec;
			//logger_.debug("call remote_endpoint");
			socket_.remote_endpoint(ec);
			connected = !ec;
		}
#endif
		if (connected)
		{
			logger_.info("connected");
			on_connected_(std::move(socket_));
			close();
		}
		else
		{
			logger_.info("failed to connect");
			on_fail();
		}
	}
}
