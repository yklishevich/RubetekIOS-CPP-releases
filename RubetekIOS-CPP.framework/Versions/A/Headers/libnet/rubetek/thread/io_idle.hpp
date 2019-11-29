#pragma once

#include <memory>

#include <msw/noncopyable.hpp>
#include <msw/dimension/trigger.hpp>
#include <msw/include/boost/asio.hpp>
#include <msw/std/chrono.hpp>
#include <msw/callback/event.hpp>

#include <rubetek/log/log.hpp>
#include <rubetek/thread/idle.hpp>


namespace rubetek
{
	struct io_idle_thread
		: msw::noncopyable
	{
		template <typename Fn>
											io_idle_thread	(boost::asio::io_service& io_service, Fn&&, msw::chrono::steady_clock::duration interval)			;

		msw::chrono::steady_clock::duration	interval		()																							const	;

	private:

		struct hold_info
			: msw::noncopyable
			, std::enable_shared_from_this<hold_info>
		{
			typedef std::shared_ptr<hold_info> ptr;

			msw::chrono::high_resolution_clock::duration hold_time() const
			{
				return msw::chrono::high_resolution_clock::now() - start_time;
			}

			msw::trigger									holds		;
			msw::chrono::high_resolution_clock::time_point	start_time	;
		};

		struct auto_holder
			: msw::noncopyable
			, std::enable_shared_from_this<auto_holder>
		{
			explicit auto_holder(hold_info::ptr hold_info)
				: hold_info_(hold_info)
			{
				hold_info_->holds.set();
				hold_info_->start_time = msw::chrono::high_resolution_clock::now();
			}
			~auto_holder()
			{
				hold_info_->holds.reset();
			}
		private:

			hold_info::ptr hold_info_;
		};

		void								idle_thread_fn	()																									;
		void								idle_event		(std::shared_ptr<auto_holder>)																		;

		boost::asio::io_service&	io_service_	;
		msw::callback::event		idle_event_	;
		hold_info::ptr				hold_info_	;
		idle_thread					idle_thread_;
	};
}


namespace rubetek
{
	template <typename Fn>
	io_idle_thread::io_idle_thread(boost::asio::io_service& io_service, Fn&& fn, msw::chrono::steady_clock::duration interval)
		: io_service_(io_service)
		, idle_event_(std::forward<Fn>(fn))
		, hold_info_(std::make_shared<hold_info>())
		, idle_thread_
		(
#ifdef MSW_NO_LAMBDA
			std::bind(&io_idle_thread::idle_thread_fn, this)
#else
			[this]()
			{
				idle_thread_fn();
			}
#endif
			, interval
		)
	{}
	inline msw::chrono::steady_clock::duration io_idle_thread::interval() const
	{
		return idle_thread_.interval();
	}

	inline void io_idle_thread::idle_thread_fn()
	{
		if (hold_info_->holds)
		{
			log::warning("idle thread: idle function holds for a long time ("
				, msw::chrono::duration_cast<msw::chrono::milliseconds>(hold_info_->hold_time()).count(), " ms)");
			msw::this_thread::sleep_for(msw::chrono::milliseconds(500));
		}
		else
		{
			auto holder(std::make_shared<auto_holder>(hold_info_));
			io_service_.post
			(
#ifdef MSW_NO_LAMBDA
				std::bind(&io_idle_thread::idle_event, this, holder)
#else
				[this, holder]()
				{
					idle_event_();
				}
#endif
			);
		}
	}
	inline void io_idle_thread::idle_event(std::shared_ptr<auto_holder>)
	{
		idle_event_();
	}
}
