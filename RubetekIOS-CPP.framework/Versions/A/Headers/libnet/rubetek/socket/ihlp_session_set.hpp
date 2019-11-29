#pragma once

#include <msw/config.hpp>
#include <msw/asio/timer.hpp>
#include <msw/noncopyable.hpp>
#include <msw/throw_runtime_error.hpp>
#include <msw/lexical_cast/network/endpoint/to_string.hpp>

#include <rubetek/log.hpp>
#include <rubetek/socket/ihlp_session.hpp>
#include <rubetek/socket/tcp_session_set.hpp>


namespace rubetek
{
	template <typename AppSession, typename IHLP_SizeType = msw::wbyte>
	struct ihlp_session_set
		: msw::noncopyable
	{
		typedef tcp_session_set<ihlp_session<AppSession, IHLP_SizeType>> tcp_session_set_type ;
		typedef typename tcp_session_set_type::iterator                  iterator             ;
		typedef typename tcp_session_set_type::const_iterator            const_iterator       ;

						ihlp_session_set	(boost::asio::io_service&)												;

		template <typename ...Args>
		void			make				(boost::asio::ip::tcp::socket, ihlp::timeouts, Args&&...)				;

		iterator		begin				()																		;
		const_iterator	begin				()																const	;
		iterator		end					()																		;
		const_iterator	end					()																const	;

		void			close_early			(msw::chrono::steady_clock::duration, boost::asio::ip::address)			;

	private:
		void			idle				()																		;

		logger               const logger_          ;
		tcp_session_set_type       tcp_session_set_ ;
		msw::timer                 timer_           ;
	};
}


namespace rubetek
{
	template <typename AppSession, typename IHLP_SizeType>
	ihlp_session_set<AppSession, IHLP_SizeType>::ihlp_session_set(boost::asio::io_service& io_service)
		: logger_("ihlp sessions", this)
		, timer_
		(
			io_service
			, msw::chrono::milliseconds(200)
			, [this]()
			{
				idle();
			}
		)
	{}

	template <typename AppSession, typename IHLP_SizeType>
	template <typename ...Args>
	void ihlp_session_set<AppSession, IHLP_SizeType>::make(boost::asio::ip::tcp::socket socket, ihlp::timeouts timeouts, Args&&... args)
	{
		tcp_session_set_.make(std::move(socket), timeouts, std::forward<Args>(args)...);
	}

	template <typename AppSession, typename IHLP_SizeType>
	typename ihlp_session_set<AppSession, IHLP_SizeType>::iterator ihlp_session_set<AppSession, IHLP_SizeType>::begin()
	{
		return tcp_session_set_.begin();
	}
	template <typename AppSession, typename IHLP_SizeType>
	typename ihlp_session_set<AppSession, IHLP_SizeType>::const_iterator ihlp_session_set<AppSession, IHLP_SizeType>::begin() const
	{
		return tcp_session_set_.begin();
	}
	template <typename AppSession, typename IHLP_SizeType>
	typename ihlp_session_set<AppSession, IHLP_SizeType>::iterator ihlp_session_set<AppSession, IHLP_SizeType>::end()
	{
		return tcp_session_set_.end();
	}
	template <typename AppSession, typename IHLP_SizeType>
	typename ihlp_session_set<AppSession, IHLP_SizeType>::const_iterator ihlp_session_set<AppSession, IHLP_SizeType>::end() const
	{
		return tcp_session_set_.end();
	}

	template <typename AppSession, typename IHLP_SizeType>
	void ihlp_session_set<AppSession, IHLP_SizeType>::close_early(msw::chrono::steady_clock::duration early_duration, boost::asio::ip::address ip)
	{
		logger_.info("closing early sessions with a lifetime of more ", msw::chrono::duration_cast<msw::chrono::seconds>(early_duration).count(), " s for ip: ", ip.to_string());
		std::list<typename tcp_session<ihlp_session<AppSession, IHLP_SizeType>>::ptr> closing_sessions;
#ifdef MSW_NO_FOR
		for (auto i = tcp_session_set_.begin(); i != tcp_session_set_.end(); ++i)
		{
			auto& session = *i;
#else
		for (auto& session : tcp_session_set_)
#endif
		{
			if (session->has_remote_endpoint())
			{
				if
				(
					(session->remote_endpoint().address() == ip)
					&& session->has_application()
					&& session->application().is_early(early_duration)
				) closing_sessions.push_back(session);
			}
			else logger_.error("session missing remote endpoint");
		}
#ifdef MSW_NO_FOR
		}
#endif
		if (closing_sessions.empty()) logger_.debug("early sessions not found");
		if (!closing_sessions.empty()) logger_.debug("close early session(s)");
#ifdef MSW_NO_FOR
		for (auto i = closing_sessions.begin(); i != closing_sessions.end(); ++i)
		{
			auto& session = *i;
#else
		for (auto& session : closing_sessions)
#endif
		{
			std::string const name = session->has_remote_endpoint() ? msw::network_endpoint_to_string(session->remote_endpoint()) : std::string("no endpoint");
			logger_.info("close early session [" + name + "]");
			session->close();
		}
#ifdef MSW_NO_FOR
		}
#endif
	}

	template <typename AppSession, typename IHLP_SizeType>
	void ihlp_session_set<AppSession, IHLP_SizeType>::idle()
	{
		std::list<typename tcp_session<ihlp_session<AppSession, IHLP_SizeType>>::ptr> hung_sessions;
#ifdef MSW_NO_FOR
		for (auto i = tcp_session_set_.begin(); i != tcp_session_set_.end(); ++i)
		{
			auto& session = *i;
#else
		for (auto& session : tcp_session_set_)
#endif
			if (session->has_application() && !session->application().check_timeout()) hung_sessions.push_back(session);
#ifdef MSW_NO_FOR
		}
#endif
		if (!hung_sessions.empty()) logger_.debug("close session(s) by timeout");
#ifdef MSW_NO_FOR
		for (auto i = hung_sessions.begin(); i != hung_sessions.end(); ++i)
		{
			auto& session = *i;
#else
		for (auto& session : hung_sessions)
#endif
		{
			try
			{
			std::string const name = session->has_remote_endpoint() ? msw::network_endpoint_to_string(session->remote_endpoint()) : std::string("no endpoint");
			logger_.info("close session [" + name + "] by timeout");
			session->close();

			}
			catch (std::exception const& e)
			{
				msw::throw_runtime_error("########## ihlp_session_set fail: ", e.what());
			}
		}
#ifdef MSW_NO_FOR
		}
#endif
	}
}
