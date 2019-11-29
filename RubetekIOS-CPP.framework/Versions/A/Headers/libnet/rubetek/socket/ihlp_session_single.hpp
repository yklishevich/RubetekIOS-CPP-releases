#pragma once

#include <unordered_set>

#include <msw/noncopyable.hpp>
#include <msw/lexical_cast/network/endpoint/to_string.hpp>

#include <rubetek/log.hpp>
#include <rubetek/socket/ihlp_session.hpp>
#include <rubetek/socket/tcp_session_single.hpp>


namespace rubetek
{
	template <typename AppSession, typename IHLP_SizeType = msw::wbyte>
	struct ihlp_session_single
		: msw::noncopyable
	{
		typedef tcp_session_single<ihlp_session<AppSession, IHLP_SizeType>>	tcp_session_single_type	;
		typedef typename tcp_session_single_type::tcp_session_ptr			tcp_session_ptr			;

								ihlp_session_single	()																	;
								~ihlp_session_single()																	;

		template <typename ...Args>
		void					make				(boost::asio::ip::tcp::socket, ihlp::timeouts, Args&&...)			;

		void					idle				()																	;

		tcp_session_ptr const&	session				()															const	;

	private:
		logger                  const logger_             ;
		tcp_session_single_type       tcp_session_single_ ;
	};
}


namespace rubetek
{
	template <typename AppSession, typename IHLP_SizeType>
	ihlp_session_single<AppSession, IHLP_SizeType>::ihlp_session_single()
		: logger_("ihlp session")
	{}
	template <typename AppSession, typename IHLP_SizeType>
	ihlp_session_single<AppSession, IHLP_SizeType>::~ihlp_session_single()
	{}

	template <typename AppSession, typename IHLP_SizeType>
	template <typename ...Args>
	void ihlp_session_single<AppSession, IHLP_SizeType>::make(boost::asio::ip::tcp::socket socket, ihlp::timeouts timeouts, Args&&... args)
	{
		tcp_session_single_.make(std::move(socket), timeouts, std::forward<Args>(args)...);
	}

	template <typename AppSession, typename IHLP_SizeType>
	void ihlp_session_single<AppSession, IHLP_SizeType>::idle()
	{
		if (session() && session()->has_application() && !session()->application().check_timeout())
		{
			try
			{

			logger_.info("close session [" + msw::network_endpoint_to_string(session()->remote_endpoint()) + "] by timeout");

			}
			catch (std::exception const& e)
			{
				msw::throw_runtime_error("########## ihlp_session_single fail: ", e.what());
			}

			session()->close();
		}
	}
	template <typename AppSession, typename IHLP_SizeType>
	typename ihlp_session_single<AppSession, IHLP_SizeType>::tcp_session_ptr const& ihlp_session_single<AppSession, IHLP_SizeType>::session() const
	{
		return tcp_session_single_.session();
	}
}
