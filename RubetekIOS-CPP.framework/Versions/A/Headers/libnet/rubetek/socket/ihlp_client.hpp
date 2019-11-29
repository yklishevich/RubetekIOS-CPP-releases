#pragma once

#include <memory>
#include <functional>

#include <msw/range.hpp>
#include <msw/config.hpp>
#include <msw/noncopyable.hpp>
#include <msw/dimension/trigger.hpp>
#include <msw/callback/byte_range.hpp>
#include <msw/callback/event.hpp>
#include <msw/lexical_cast/network/endpoint/to_string.hpp>

#include <rubetek/log.hpp>
#include <rubetek/socket/ihlp_session_single.hpp>
#include <rubetek/socket/tcp_connector_single.hpp>


namespace rubetek
{
	template <typename IHLP_SizeType>
	struct ihlp_client_base
		: msw::noncopyable
	{
		typedef msw::callback::byte_const_range		on_packet_received	;
		typedef msw::callback::event				on_connected		;
		typedef msw::callback::event				on_disconnected		;
		typedef msw::callback::event				on_connection_fail	;

								ihlp_client_base		(
															boost::asio::io_service&
															, boost::asio::ip::tcp::endpoint
															, ihlp::timeouts
															, on_packet_received
															, on_connected
															, on_disconnected
															, on_connection_fail
														)												;
								ihlp_client_base		(
															boost::asio::io_service&
															, boost::asio::ip::tcp::resolver::query
															, ihlp::timeouts
															, on_packet_received
															, on_connected
															, on_disconnected
															, on_connection_fail
														)												;

								~ihlp_client_base		()												;

		void					idle					()												;
		void					send					(msw::range<msw::byte const>)					;
		bool					connected				()										const	;
		ihlp::timeouts const&	ihlp_timeouts			()										const	;

	private:

		void					connect					()												;
		void					on_connected_handler	(boost::asio::ip::tcp::socket)					;
		void					on_disconnected_handler	()												;

		struct session
		{
			typedef std::function<void(msw::range<msw::byte const>)> fn_send;

			template <typename FnClose>
			session
			(
				fn_send fsend
				, FnClose
				, typename ihlp_client_base<IHLP_SizeType>::on_disconnected on_disconnected
				, typename ihlp_client_base<IHLP_SizeType>::on_packet_received packet_received
			)
				: send_					(fsend)
				, on_disconnected_		(on_disconnected)
				, on_packet_received_	(packet_received)
			{}
			void on_packet_received(msw::range<msw::byte> packet)
			{
				on_packet_received_(packet);
			}
			~session()
			{
				on_disconnected_();
			}
			void send(msw::range<msw::byte const> packet)
			{
				send_(packet);
			}
		private:
			fn_send                                                      send_               ;
			typename ihlp_client_base<IHLP_SizeType>::on_disconnected    on_disconnected_    ;
			typename ihlp_client_base<IHLP_SizeType>::on_packet_received on_packet_received_ ;
		};

		logger                                      const logger_              ;
		boost::asio::io_service&                          io_service_          ;
		boost::asio::ip::tcp::endpoint              const endpoint_            ;
		boost::asio::ip::tcp::resolver::query       const query_               ;
		ihlp::timeouts                              const timeouts_            ;
		on_packet_received                                on_packet_received_  ;
		on_connected                                      on_connected_        ;
		on_disconnected                                   on_disconnected_     ;
		on_connection_fail                                on_connection_fail_  ;
		tcp_connector_single                              tcp_connector_       ;
		ihlp_session_single<session, IHLP_SizeType>       ihlp_session_single_ ;
		msw::trigger                                      do_it_               ;
	};

	typedef ihlp_client_base<msw::wbyte> ihlp_client       ;
	typedef ihlp_client_base<msw::qbyte> ihlp_large_client ;
}


namespace rubetek
{
	template <typename IHLP_SizeType>
	ihlp_client_base<IHLP_SizeType>::ihlp_client_base
	(
		boost::asio::io_service&         io_service
		, boost::asio::ip::tcp::endpoint endpoint
		, ihlp::timeouts                 timeouts
		, on_packet_received             on_packet_received
		, on_connected                   on_connected
		, on_disconnected                on_disconnected
		, on_connection_fail             on_connection_fail
	)
		: logger_             ("ihlp client", msw::network_endpoint_to_string(endpoint), log::level::info)
		, io_service_         (io_service)
		, endpoint_           (endpoint)
		, query_              ("")
		, timeouts_           (timeouts)
		, on_packet_received_ (on_packet_received)
		, on_connected_       (on_connected)
		, on_disconnected_    (on_disconnected)
		, on_connection_fail_ (on_connection_fail)
		, tcp_connector_      (io_service)
		, do_it_              (true)
	{
		connect();
	}
	template <typename IHLP_SizeType>
	ihlp_client_base<IHLP_SizeType>::ihlp_client_base
	(
		boost::asio::io_service&                io_service
		, boost::asio::ip::tcp::resolver::query query
		, ihlp::timeouts                        timeouts
		, on_packet_received                    on_packet_received
		, on_connected                          on_connected
		, on_disconnected                       on_disconnected
		, on_connection_fail                    on_connection_fail
	)
		: logger_             ("ihlp client", query.host_name() + ":" + query.service_name(), log::level::info)
		, io_service_         (io_service)
		, query_              (query)
		, timeouts_           (timeouts)
		, on_packet_received_ (on_packet_received)
		, on_connected_       (on_connected)
		, on_disconnected_    (on_disconnected)
		, on_connection_fail_ (on_connection_fail)
		, tcp_connector_      (io_service)
		, do_it_              (true)
	{
		connect();
	}
	template <typename IHLP_SizeType>
	ihlp_client_base<IHLP_SizeType>::~ihlp_client_base()
	{
		do_it_.reset();
	}

	template <typename IHLP_SizeType>
	void ihlp_client_base<IHLP_SizeType>::idle()
	{
		ihlp_session_single_.idle();
	}
	template <typename IHLP_SizeType>
	void ihlp_client_base<IHLP_SizeType>::send(msw::range<msw::byte const> packet)
	{
		MSW_ASSERT(connected());
		if (connected()) ihlp_session_single_.session()->application().application().send(packet);
	}
	template <typename IHLP_SizeType>
	bool ihlp_client_base<IHLP_SizeType>::connected() const
	{
		return static_cast<bool>(ihlp_session_single_.session());
	}
	template <typename IHLP_SizeType>
	ihlp::timeouts const& ihlp_client_base<IHLP_SizeType>::ihlp_timeouts() const
	{
		return timeouts_;
	}

	template <typename IHLP_SizeType>
	void ihlp_client_base<IHLP_SizeType>::connect()
	{
		if (endpoint_ == boost::asio::ip::tcp::endpoint())
		{
			tcp_connector_.make
			(
				query_
				, [this](boost::asio::ip::tcp::socket socket)
				{
					on_connected_handler(std::move(socket));
				}
				, on_connection_fail_
			);
		}
		else
		{
			tcp_connector_.make
			(
				endpoint_
				, [this](boost::asio::ip::tcp::socket socket)
				{
					on_connected_handler(std::move(socket));
				}
				, on_connection_fail_
			);
		}
	}
	template <typename IHLP_SizeType>
	void ihlp_client_base<IHLP_SizeType>::on_connected_handler(boost::asio::ip::tcp::socket socket)
	{
		ihlp_session_single_.make
		(
			std::move(socket)
			, timeouts_
#ifdef MSW_NO_LAMBDA
			, std::bind(&ihlp_client_base::on_disconnected_handler, this)
#else
			, [this]()
			{
				on_disconnected_();
				if (do_it_) connect();
			}
#endif
			, on_packet_received_
		);
		on_connected_();
	}
	template <typename IHLP_SizeType>
	void ihlp_client_base<IHLP_SizeType>::on_disconnected_handler()
	{
		on_disconnected_();
		if (do_it_) connect();
	}
}
