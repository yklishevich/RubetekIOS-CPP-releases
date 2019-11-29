#pragma once

#include <deque>
#include <memory>
#include <functional>

#include <msw/config.hpp>
#include <msw/buffer.hpp>
#include <msw/noncopyable.hpp>
#include <msw/dimension/trigger.hpp>
#include <msw/include/boost/asio.hpp>
#include <msw/throw_runtime_error.hpp>
#include <msw/lexical_cast/network/endpoint/to_string.hpp>

#include <rubetek/log.hpp>


namespace rubetek
{
	template <typename AppSession>
	struct tcp_session
		: msw::noncopyable
		, std::enable_shared_from_this<tcp_session<AppSession>>
	{
		typedef tcp_session<AppSession>		self_type	;
		typedef std::shared_ptr<self_type>	ptr			;
		typedef std::function<void(ptr)>	on_close_fn	;

				void							send				(msw::range<msw::byte const>)									;
				void							close				()																;
				bool							has_remote_endpoint	()														const	;
				boost::asio::ip::tcp::endpoint	remote_endpoint		()														const	;

		template <typename ...Args>
		static	ptr								make				(boost::asio::ip::tcp::socket, on_close_fn, Args&&...)			;
												~tcp_session		()																;

				bool							has_application		()														const	;
				AppSession&						application			()																;
				AppSession const&				application			()														const	;

	private:
						tcp_session				(boost::asio::ip::tcp::socket, on_close_fn)											;
		template <typename ...Args>
				void	start					(Args&&...)																			;
				void	do_receive				()																					;
				void	do_send					()																					;

				void	pre_send_handler		(msw::buffer<msw::byte>)															;
				void	send_handler			(std::shared_ptr<self_type>, boost::system::error_code, std::size_t)				;
				void	receive_handler			(std::shared_ptr<self_type>, boost::system::error_code, std::size_t)				;

				void	close_from_application	()																					;
				void	close_application		()																					;

		logger								const	logger_				;
		logger_io<log_io_tags::tcp>			const	logger_io_			;
		msw::trigger								process_received_	;
		msw::trigger								must_close_			;
		boost::asio::ip::tcp::socket				socket_				;
		msw::buffer<msw::byte>						received_stream_	;
		std::deque<msw::buffer<msw::byte>>			send_packets_		;
		on_close_fn									on_close_			;
		std::unique_ptr<AppSession>					application_session_;
	};
}


namespace rubetek
{
	template <typename AppSession>
	void tcp_session<AppSession>::send(msw::range<msw::byte const> packet)
	{
		if (!socket_.is_open())
		{
			logger_.warning("send fail: socket closed");
			return;
		}
		logger_io_.send(packet);
		msw::buffer<msw::byte> pkt(packet);
		socket_.get_io_service().post
		(
#ifdef MSW_NO_LAMBDA
			std::bind(&tcp_session<AppSession>::pre_send_handler, this, pkt)
#else
			[this, pkt]()
			{
				bool send_in_progress = !send_packets_.empty();
				send_packets_.push_back(/*std::move(*/pkt/*)*/);
				if (!send_in_progress) do_send();
			}
#endif
		);
	}

	template <typename AppSession>
	void tcp_session<AppSession>::close()
	{
		if (application_session_)
		{
#ifdef SMART_HOUSE_TCP_SESSION_DETAIL_LOG
			logger_.debug("closing");
#endif
			socket_.close();
			if (!process_received_) close_application();
			logger_.debug("closed");
			on_close_(std::enable_shared_from_this<tcp_session<AppSession>>::shared_from_this());
#ifdef SMART_HOUSE_TCP_SESSION_DETAIL_LOG
			logger_.debug("on_close is called");
#endif
		}
	}

	template <typename AppSession>
	bool tcp_session<AppSession>::has_remote_endpoint() const
	{
		boost::system::error_code ec;
		socket_.remote_endpoint(ec);
		return !ec;
	}
	template <typename AppSession>
	boost::asio::ip::tcp::endpoint tcp_session<AppSession>::remote_endpoint() const
	{
		//logger_.debug("call remote_endpoint");
		boost::asio::ip::tcp::endpoint ep;
		try
		{
			boost::system::error_code ec;
			//logger_.debug("0");
			ep = socket_.remote_endpoint(ec);
			//logger_.debug("ep");
			if (ec) logger_.error(ec.message());
		}
		catch (std::exception const& e)
		{
			//msw::throw_runtime_error("tcp session: ", e.what());
			logger_.critical("tcp session: ", e.what());
		}
		//logger_.debug("call remote_endpoint return");
		return ep;
	}

	template <typename AppSession>
	template <typename ...Args>
	typename tcp_session<AppSession>::ptr tcp_session<AppSession>::make(boost::asio::ip::tcp::socket socket, on_close_fn on_close, Args&&... args)
	{
		ptr session(new self_type(std::move(socket), on_close));
		session->start(std::forward<Args>(args)...);
		return session;
	}

	template <typename AppSession>
	tcp_session<AppSession>::~tcp_session()
	{}

	template <typename AppSession>
	bool tcp_session<AppSession>::has_application() const
	{
		return static_cast<bool>(application_session_);
	}
	template <typename AppSession>
	AppSession& tcp_session<AppSession>::application()
	{
		return *application_session_;
	}
	template <typename AppSession>
	AppSession const& tcp_session<AppSession>::application() const
	{
		return *application_session_;
	}

	template <typename AppSession>
	tcp_session<AppSession>::tcp_session(boost::asio::ip::tcp::socket socket, on_close_fn on_close)
		: logger_			("tcp session", msw::network_endpoint_to_string(socket.local_endpoint()) + " <- " + msw::network_endpoint_to_string(socket.remote_endpoint()))
		, logger_io_		(logger_)
		, socket_			(std::move(socket))
		, received_stream_	(msw::KB * 64)
		, on_close_			(on_close)
	{
		boost::asio::socket_base::linger option(true, 0);
		socket_.set_option(option);
	}

	template <typename AppSession>
	template <typename ...Args>
	void tcp_session<AppSession>::start(Args&&... args)
	{
		application_session_.reset(new AppSession(std::bind(&self_type::send, this, std::placeholders::_1), std::bind(&self_type::close_from_application, this), std::forward<Args>(args)...));
		logger_.info("started");
		do_receive();
	}

	template <typename AppSession>
	void tcp_session<AppSession>::do_receive()
	{
		auto self(std::enable_shared_from_this<tcp_session<AppSession>>::shared_from_this());
		socket_.async_receive
		(
			boost::asio::buffer(received_stream_.data().native(), received_stream_.size().count())
#ifdef MSW_NO_LAMBDA
			, std::bind(&tcp_session<AppSession>::receive_handler, this, self, std::placeholders::_1, std::placeholders::_2)
#else
			, [this, self](boost::system::error_code ec, std::size_t bytes_received)
			{
				receive_handler(self, ec, bytes_received);
			}
#endif
		);
	}
	template <typename AppSession>
	void tcp_session<AppSession>::do_send()
	{
		auto self(std::enable_shared_from_this<tcp_session<AppSession>>::shared_from_this());
		socket_.async_send
		(
			boost::asio::buffer(send_packets_.front().data().native(), send_packets_.front().size().count())
#ifdef MSW_NO_LAMBDA
			, std::bind(&tcp_session<AppSession>::send_handler, this, self, std::placeholders::_1, std::placeholders::_2)
#else
			, [this, self](boost::system::error_code ec, std::size_t bytes_sent)
			{
				send_handler(self, ec, bytes_sent);
			}
#endif
		);
	}

	template <typename AppSession>
	void tcp_session<AppSession>::pre_send_handler(msw::buffer<msw::byte> pkt)
	{
		bool send_in_progress = !send_packets_.empty();
		send_packets_.push_back(std::move(pkt));
		if (!send_in_progress) do_send();
	}
	template <typename AppSession>
	void tcp_session<AppSession>::send_handler(std::shared_ptr<self_type>, boost::system::error_code ec, std::size_t bytes_sent)
	{
		if (!ec)
		{
			msw::size<msw::byte> const sent_len = bytes_sent;
			logger_io_.sent(send_packets_.front().front(sent_len));
			if (send_packets_.front().size() == sent_len) send_packets_.pop_front();
			else send_packets_.front().erase_from_to_front(sent_len);
			if (!send_packets_.empty()) do_send();
		}
		else
		{
#ifdef SMART_HOUSE_TCP_SESSION_DETAIL_LOG
			logger_.debug("send error");
#endif
			close();
		}
	}
	template <typename AppSession>
	void tcp_session<AppSession>::receive_handler(std::shared_ptr<self_type>, boost::system::error_code ec, std::size_t bytes_received)
	{
		if (!ec)
		{
			logger_io_.received(received_stream_.front(bytes_received));
			if (socket_.is_open())
			{
				process_received_.set();
				try
				{
					application_session_->on_packet_received(received_stream_.front(bytes_received));
				}
				catch (std::exception const& e)
				{
					logger_.error("process packet: ", e.what());
				}
				process_received_.reset();
				if (must_close_) close_application();
				else do_receive();
			}
		}
		else
		{
#ifdef SMART_HOUSE_TCP_SESSION_DETAIL_LOG
			logger_.debug("receive error");
#endif
			close();
		}
	}

	template <typename AppSession>
	void tcp_session<AppSession>::close_from_application()
	{
#ifdef SMART_HOUSE_TCP_SESSION_DETAIL_LOG
		logger_.debug("close from application");
#endif
		if (process_received_) must_close_.set();
		else close();
	}
	template <typename AppSession>
	void tcp_session<AppSession>::close_application()
	{
		if (application_session_)
		{
#ifdef SMART_HOUSE_TCP_SESSION_DETAIL_LOG
			logger_.debug("application closing");
#endif
			application_session_.reset();
#ifdef SMART_HOUSE_TCP_SESSION_DETAIL_LOG
			logger_.debug("application closed");
#endif
		}
	}
}
