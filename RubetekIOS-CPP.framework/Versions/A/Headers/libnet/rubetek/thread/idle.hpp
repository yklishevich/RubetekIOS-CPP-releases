#pragma once

#include <msw/config.hpp>
#include <msw/noncopyable.hpp>
#include <msw/dimension/trigger.hpp>
#include <msw/std/chrono.hpp>
#include <msw/std/thread.hpp>
#include <msw/callback/event.hpp>

#include <rubetek/log.hpp>


namespace rubetek
{
	struct idle_thread
		: msw::noncopyable
	{

		template <typename Fn>				idle_thread		(Fn&&, msw::chrono::steady_clock::duration interval)			;
											~idle_thread	()																;

		msw::chrono::steady_clock::duration	interval		()														const	;

	private:

		void								thread_fn		()																;

		logger								const	logger_		;
		msw::chrono::steady_clock::duration	const	interval_	;
		msw::callback::event						idle_event_	;
		msw::trigger								do_it_		;
		msw::thread									thread_		;
	};
}


namespace rubetek
{
	template <typename Fn>
	idle_thread::idle_thread(Fn&& fn, msw::chrono::steady_clock::duration interval)
		: logger_		("idle thread", this)
		, interval_		(interval)
		, idle_event_	(std::forward<Fn>(fn))
		, thread_
		(
#ifdef MSW_NO_LAMBDA
			std::bind(&idle_thread::thread_fn, this)
#else
			[this]()
			{
				thread_fn();
			}
#endif
		)
	{}
	inline idle_thread::~idle_thread()
	{
		logger_.info("stops...");
		do_it_.reset();
		thread_.join();
	}

	inline msw::chrono::steady_clock::duration idle_thread::interval() const
	{
		return interval_;
	}

	inline void idle_thread::thread_fn()
	{
		logger_.info("launched");
		try
		{
			do_it_.set();
			while (do_it_)
			{
				idle_event_();
				msw::this_thread::sleep_for(interval_);
			}
		}
		catch (std::exception const& e)
		{
			logger_.critical(e.what());
		}
		logger_.info("stopped");
	}
}
