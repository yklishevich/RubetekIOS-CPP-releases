#ifndef SVL_WIDGETS_CHECK_BOX_HPP
#define SVL_WIDGETS_CHECK_BOX_HPP

#include <svl/base/widget.hpp>

namespace svl
{
	class check_box
		: public control
	{
	public:
		check_box();
		check_box(str_ref caption, bool third_state = false, tribool initial_state = true, bool multiline = false);

		void set_caption(str_ref);
		string caption() const;

		void enable_third_state(bool = true);
		void set_multiline(bool = true);

		void check(tribool = true);
		void uncheck();
		void set_checked(tribool = true); // не генерирует события event::check
		tribool checked() const;
		bool third_state_enabled() const;

		connector<event::check> on_check();

		~check_box();

	protected:
		void pre_handle_event(event::processor&); // override

	private:
		void handle_command(event::win32::command&);
	};
}

#endif // SVL_WIDGETS_CHECK_BOX_HPP
