#ifndef SVL_WIDGETS_BUTTON_HPP
#define SVL_WIDGETS_BUTTON_HPP

#include <svl/base/widget.hpp>

namespace svl
{
	class button
		: public control
		, public i_button
	{
	public:
		button();
		button(str_ref caption);
		~button();

		string caption() const;
		void set_caption(str_ref);

		void click();

		void set_def_style(bool); // override

		connector<event::click> on_click();

	protected:
		void pre_handle_event(event::processor&);

	private:
		void handle_command(event::win32::command&);
	};
}

#endif //SVL_WIDGETS_BUTTON_HPP
