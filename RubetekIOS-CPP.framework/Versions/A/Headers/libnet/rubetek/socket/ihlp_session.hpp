#pragma once

#include <functional>

#include <msw/range.hpp>
#include <msw/config.hpp>
#include <msw/noncopyable.hpp>
#include <msw/callback/byte_range.hpp>
#include <msw/callback/event.hpp>

#include <rubetek/proto/ihlp/transceiver.hpp>


namespace rubetek
{
	template <typename AppSession, typename IHLP_SizeType = msw::wbyte>
	struct ihlp_session
		: msw::noncopyable
	{
		typedef msw::callback::byte_range	ihlp_packet_ready	;
		typedef msw::callback::event		fn_close			;

		template <typename ...Args>
					ihlp_session			(ihlp_packet_ready, fn_close, ihlp::timeouts, Args&&...)	;
					~ihlp_session			()															;

		void		on_packet_received		(msw::range<msw::byte>)										;
		bool		check_timeout			()															;
		bool		is_early				(msw::chrono::steady_clock::duration)						;
		void		set_keep_alive_time		(msw::chrono::steady_clock::duration)						;

		AppSession&	application				()															;

	private:

		void		payload_ready			(msw::range<msw::byte>)										;

		std::unique_ptr<AppSession>           app_session_      ;
		ihlp::transceiver_base<IHLP_SizeType> ihlp_transceiver_ ;

	};
}


namespace rubetek
{
	template <typename AppSession, typename IHLP_SizeType>
	template <typename ...Args>
	ihlp_session<AppSession, IHLP_SizeType>::ihlp_session(ihlp_packet_ready ihlp_packet_ready, fn_close close, ihlp::timeouts timeouts, Args&&... args)
		: ihlp_transceiver_
		(
#ifdef MSW_NO_LAMBDA
			std::bind(&ihlp_session<AppSession>::payload_ready, this, std::placeholders::_1)
#else
			[this](msw::range<msw::byte> payload)
			{
				payload_ready(payload);
			}
#endif
			, ihlp_packet_ready
			, timeouts
		)
	{
		app_session_.reset(new AppSession(std::bind(&ihlp::transceiver_base<IHLP_SizeType>::send, &ihlp_transceiver_, std::placeholders::_1), close, std::forward<Args>(args)...));
	}
	template <typename AppSession, typename IHLP_SizeType>
	ihlp_session<AppSession, IHLP_SizeType>::~ihlp_session()
	{
		app_session_.reset();
	}

	template <typename AppSession, typename IHLP_SizeType>
	void ihlp_session<AppSession, IHLP_SizeType>::on_packet_received(msw::range<msw::byte> packet)
	{
		ihlp_transceiver_.process_tcp_payload(packet);
	}
	template <typename AppSession, typename IHLP_SizeType>
	bool ihlp_session<AppSession, IHLP_SizeType>::check_timeout()
	{
		return ihlp_transceiver_.check_timeout();
	}
	template <typename AppSession, typename IHLP_SizeType>
	void ihlp_session<AppSession, IHLP_SizeType>::set_keep_alive_time(msw::chrono::steady_clock::duration keep_alive_time)
	{
		ihlp_transceiver_.set_keep_alive_time(keep_alive_time);
	}
	template <typename AppSession, typename IHLP_SizeType>
	bool ihlp_session<AppSession, IHLP_SizeType>::is_early(msw::chrono::steady_clock::duration duration)
	{
		return ihlp_transceiver_.is_early(duration);
	}

	template <typename AppSession, typename IHLP_SizeType>
	AppSession&	ihlp_session<AppSession, IHLP_SizeType>::application()
	{
		MSW_ASSERT(app_session_);
		return *app_session_;
	}

	template <typename AppSession, typename IHLP_SizeType>
	void ihlp_session<AppSession, IHLP_SizeType>::payload_ready(msw::range<msw::byte> payload)
	{
		if (app_session_) app_session_->on_packet_received(payload);
	}
}
