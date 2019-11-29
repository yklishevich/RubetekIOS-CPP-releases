#ifndef SVL_BASE_TIMER_HPP
#define SVL_BASE_TIMER_HPP

#include <svl/base/events.hpp>

namespace svl
{
	class timer
		: public event::target
	{
	public:
		timer();
		~timer();

		void start(unsigned ms);
		void stop(); // throw()
		bool started() const; // throw()

		void notify();
		connector<event::timer> on_timer();
	};
}

#endif // SVL_BASE_TIMER_HPP
