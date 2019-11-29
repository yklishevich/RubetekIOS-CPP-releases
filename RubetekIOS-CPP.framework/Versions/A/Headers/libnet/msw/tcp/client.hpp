#pragma once

/*
DEPRECATED
*/

#include <string>
#include <functional>

#include <boost/bind.hpp>

#include <msw/buffer.hpp>
#include <msw/plain_types.hpp>
#include <msw/noncopyable.hpp>
#include <msw/include/boost/asio.hpp>


namespace msw {
namespace tcp {

	struct client
		: noncopyable
	{
		typedef std::function<void(boost::system::error_code)>	on_connection	;
		typedef std::function<void(range<byte>)>				on_packet_ready	;
		typedef std::function<void(boost::system::error_code)>	on_receive_error;

		explicit	client			(boost::asio::io_service&)																							;

		void		connect			(boost::asio::ip::tcp::endpoint, on_connection, on_packet_ready, on_receive_error fn = on_receive_error())			;
		void		disconnect		()																													;

		void		send			(range<byte const> packet)																							;
		void		send			(range<byte const> packet, boost::system::error_code&)																;

		void		receive			(range<byte>)																										;

	private:

		void		handle_receive	(boost::system::error_code const&, size_t bytes_transferred, range<byte>)											;

		boost::asio::ip::tcp::socket	socket_				;
		on_connection					on_connection_		;
		on_packet_ready					on_packet_ready_	;
		on_receive_error				on_receive_error_	;
	};

}}


namespace msw {
namespace tcp {

	inline client::client(boost::asio::io_service& io_service)
		: socket_(io_service)
	{}

	inline void client::connect(boost::asio::ip::tcp::endpoint endpoint, on_connection on_connection, on_packet_ready on_packet_ready, on_receive_error on_receive_error)
	{
		on_connection_ = on_connection;
		on_packet_ready_ = on_packet_ready;
		on_receive_error_ = on_receive_error;

		socket_.async_connect(endpoint
			, [this](boost::system::error_code const& ec)
			{
				on_connection_(ec);
			});
	}
	inline void client::disconnect()
	{
		socket_.close();
	}

	inline void client::send(range<byte const> packet)
	{
		socket_.send(boost::asio::buffer(packet.data().native(), packet.size().count()));
	}
	inline void client::send(range<byte const> packet, boost::system::error_code& ec)
	{
		socket_.send(boost::asio::buffer(packet.data().native(), packet.size().count()), 0, ec);
	}

	inline void client::receive(range<byte> buf)
	{
		socket_.async_receive(boost::asio::buffer(buf.data().native(), buf.size().count())
			, boost::bind(&client::handle_receive, this
			, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred, buf));
	}

	inline void client::handle_receive(boost::system::error_code const& ec, size_t bytes_transferred, range<byte> buf)
	{
		if (ec)
		{
			if (on_receive_error_) on_receive_error_(ec);
			else throw boost::system::system_error(ec);
		}
		else on_packet_ready_(buf.front(bytes_transferred));
	}

}}
