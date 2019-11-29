#pragma once

#include <functional>

#include <msw/config.hpp>
#include <msw/std/thread.hpp>
#include <msw/noncopyable.hpp>
#include <msw/include/boost/asio.hpp>

#include <rubetek/log.hpp>


namespace rubetek
{
	struct io_thread
		: msw::noncopyable
	{
		explicit	io_thread	(boost::asio::io_service& io_service)	;
					~io_thread	()										;

	private:

		void		work		()										;

		logger						const	logger_		;
		boost::asio::io_service&			io_service_	;
		msw::thread							thread_		;
	};
}


namespace rubetek
{
	inline io_thread::io_thread(boost::asio::io_service& io_service)
		: logger_     ("io service", this)
		, io_service_ (io_service)
		, thread_
		(
#ifdef MSW_NO_LAMBDA
			std::bind(&io_thread::work, this)
#else
			[this]()
			{
				work();
			}
#endif
		)
	{}
	inline io_thread::~io_thread()
	{
		logger_.info("stops...");
		io_service_.stop();
		thread_.join();
	}

	inline void io_thread::work()
	{
		logger_.info("launched");
		try
		{
			boost::asio::io_service::work work(io_service_);
			io_service_.run();
		}
		catch (std::exception const& e)
		{
			logger_.critical(e.what());
		}
		logger_.info("stopped");
	}
}
