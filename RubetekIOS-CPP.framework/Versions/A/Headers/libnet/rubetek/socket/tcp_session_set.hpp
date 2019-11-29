#pragma once

#include <functional>

#include <msw/config.hpp>

#ifdef MSW_NO_UNORDERED_CONTAINERS
#	include <set>
#else
#	include <unordered_set>
#endif

#include <msw/noncopyable.hpp>
#include <msw/dimension/trigger.hpp>

#include <rubetek/socket/tcp_session.hpp>


namespace rubetek
{
	template <typename AppSession>
	struct tcp_session_set
		: msw::noncopyable
	{
		typedef tcp_session<AppSession>								tcp_session_type;
		typedef std::
#ifdef MSW_NO_UNORDERED_CONTAINERS
				set
#else
				unordered_set
#endif
						<typename tcp_session_type::ptr>			tcp_sessions	;
		typedef typename tcp_sessions::iterator						iterator		;
		typedef typename tcp_sessions::const_iterator				const_iterator	;

						tcp_session_set	()													;
						~tcp_session_set()													;

		template <typename ...Args>
		void			make			(boost::asio::ip::tcp::socket, Args&&...)			;

		iterator		begin			()													;
		const_iterator	begin			()											const	;
		iterator		end				()													;
		const_iterator	end				()											const	;

	private:
#ifdef MSW_NO_LAMBDA
		void			on_close		(typename tcp_session_type::ptr)					;
#endif

		msw::trigger	destructurization_	;
		tcp_sessions	tcp_sessions_		;
	};
}


namespace rubetek
{
	template <typename AppSession>
	tcp_session_set<AppSession>::tcp_session_set()
		: destructurization_(false)
	{}
	template <typename AppSession>
	tcp_session_set<AppSession>::~tcp_session_set()
	{
		destructurization_.set();
#ifdef MSW_NO_FOR
		for (auto i = tcp_sessions_.begin(); i != tcp_sessions_.end(); ++i)
			(*i)->close();
#else
		for (typename tcp_session_type::ptr const& ptr : tcp_sessions_)
			ptr->close();
#endif
	}
	template <typename AppSession>
	template <typename ...Args>
	void tcp_session_set<AppSession>::make(boost::asio::ip::tcp::socket socket, Args&&... args)
	{
		tcp_sessions_.insert(tcp_session_type::make
			(
				std::move(socket)
#ifdef MSW_NO_LAMBDA
				, std::bind(&tcp_session_set<AppSession>::on_close, this, std::placeholders::_1)
#else
				, [this](typename tcp_session_type::ptr ptr)
				{
					if (!destructurization_) tcp_sessions_.erase(ptr);
				}
#endif
				, std::forward<Args>(args)...
			)
		);
	}

	template <typename AppSession>
	typename tcp_session_set<AppSession>::iterator tcp_session_set<AppSession>::begin()
	{
		return tcp_sessions_.begin();
	}
	template <typename AppSession>
	typename tcp_session_set<AppSession>::const_iterator tcp_session_set<AppSession>::begin() const
	{
		return tcp_sessions_.begin();
	}
	template <typename AppSession>
	typename tcp_session_set<AppSession>::iterator tcp_session_set<AppSession>::end()
	{
		return tcp_sessions_.end();
	}
	template <typename AppSession>
	typename tcp_session_set<AppSession>::const_iterator tcp_session_set<AppSession>::end() const
	{
		return tcp_sessions_.end();
	}

#ifdef MSW_NO_LAMBDA
	template <typename AppSession>
	void tcp_session_set<AppSession>::on_close(typename tcp_session_type::ptr ptr)
	{
		if (!destructurization_) tcp_sessions_.erase(ptr);
	}
#endif
}
