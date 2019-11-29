#pragma once

#include <unordered_set>

#include <msw/config.hpp>
#include <msw/noncopyable.hpp>

#include <rubetek/socket/tcp_session.hpp>


namespace rubetek
{
	template <typename AppSession>
	struct tcp_session_single
		: msw::noncopyable
	{
		typedef tcp_session<AppSession>			tcp_session_type;
		typedef typename tcp_session_type::ptr	tcp_session_ptr	;

								tcp_session_single	()											;
								~tcp_session_single	()											;

		template <typename ...Args>
		void					make				(boost::asio::ip::tcp::socket, Args&&...)	;

		tcp_session_ptr const&	session				()	const									;

	private:
		void					on_close			(typename tcp_session_type::ptr)			;

		tcp_session_ptr	tcp_session_;
	};
}


namespace rubetek
{
	template <typename AppSession>
	tcp_session_single<AppSession>::tcp_session_single()
	{}
	template <typename AppSession>
	tcp_session_single<AppSession>::~tcp_session_single()
	{
		if (tcp_session_) tcp_session_->close();
	}
	template <typename AppSession>
	template <typename ...Args>
	void tcp_session_single<AppSession>::make(boost::asio::ip::tcp::socket socket, Args&&... args)
	{
		if (tcp_session_) throw std::runtime_error("tcp session already exists");
		tcp_session_ = tcp_session_type::make
			(
				std::move(socket)
#ifdef MSW_NO_LAMBDA
				std::bind(&tcp_session_single<AppSession>::on_close, this, std::placehoders::_1)
#else
				, [this](typename tcp_session_type::ptr)
				{
					tcp_session_.reset();
				}
#endif
				, std::forward<Args>(args)...
			);
	}
	template <typename AppSession>
	typename tcp_session_single<AppSession>::tcp_session_ptr const& tcp_session_single<AppSession>::session() const
	{
		return tcp_session_;
	}
	template <typename AppSession>
	void tcp_session_single<AppSession>::on_close(typename tcp_session_type::ptr)
	{
		tcp_session_.reset();
	}
}
