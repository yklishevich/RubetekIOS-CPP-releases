#pragma once

#include <memory>
#include <functional>

#include <boost/asio/steady_timer.hpp>

#include <rubetek/log.hpp>


namespace rubetek
{
	struct timer_impl
		: std::enable_shared_from_this<timer_impl>
	{
		typedef std::function<void()> on_time;

		timer_impl(boost::asio::io_service& io_service, on_time on_time, std::chrono::steady_clock::duration interval)
			: logger_	("timer", this)
			, interval_	(interval)
			, timer_	(io_service)
			, on_time_	(on_time)
		{}
		~timer_impl()
		{}
		void start()
		{
			start_wait();
		}
		void close()
		{
			timer_.cancel();
			logger_.debug("closed");
		}
	private:

		void start_wait()
		{
			timer_.expires_from_now(interval_);
			auto self(shared_from_this());
			timer_.async_wait([this, self](boost::system::error_code const& ec)
			{
				if (ec) logger_.debug("timer fail: ", ec.message());
				else on_time_();
				start_wait();
			});
		}

		logger								const	logger_		;
		std::chrono::steady_clock::duration	const	interval_	;
		boost::asio::steady_timer					timer_		;
		on_time										on_time_	;
	};

	struct timer
	{
		typedef timer_impl::on_time on_time;

		timer(boost::asio::io_service& io_service, on_time on_time, std::chrono::steady_clock::duration interval)
			: timer_(new timer_impl(io_service, on_time, interval))
		{
			timer_->start();
		}
		~timer()
		{
			timer_->close();
		}
	private:
		std::shared_ptr<timer_impl> timer_;
	};
}
