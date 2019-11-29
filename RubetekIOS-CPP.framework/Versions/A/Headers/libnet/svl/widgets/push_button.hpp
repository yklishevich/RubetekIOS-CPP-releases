#ifndef SVL_WIDGETS_PUSH_BUTTON_HPP
#define SVL_WIDGETS_PUSH_BUTTON_HPP

#include <svl/base/widget.hpp>
#include <svl/widgets/button.hpp>

namespace svl
{
	class push_button
		: public control
	{
	public:
		push_button();
		explicit push_button(str_ref caption, bool pushed = false);
		~push_button();

		void set_auto_mode(bool value = true);

		string caption() const;
		void set_caption(str_ref);

		void click();
		void push_down();
		void push_up();
		void push(bool down);

		bool pushed() const;

		connector<event::click> on_click();

	protected:
		void pre_handle_event(event::processor&);

	private:
		void handle_paint(event::paint&);
		void handle_resize(event::resize&);
		void handle_mouse_key_down( event::mouse_key_down& );
		void handle_mouse_key_up( event::mouse_key_up& );

		button btn_;
		bool   pushed_    : 1;
		bool   mkey_down_ : 1;
		bool   auto_      : 1;
	};
}

#endif //SVL_WIDGETS_PUSH_BUTTON_HPP
