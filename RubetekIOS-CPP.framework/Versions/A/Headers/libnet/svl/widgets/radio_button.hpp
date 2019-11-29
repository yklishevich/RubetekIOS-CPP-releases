#ifndef SVL_WIDGETS_RADIO_BUTTON_HPP
#define SVL_WIDGETS_RADIO_BUTTON_HPP

#include <svl/base/widget.hpp>

namespace svl
{
	class radio_button;

	/*----------------------------------------------------------------------*/

	class radio_group
		: public event::target
		, public i_tab_group
	{
	public:
		radio_group();
		~radio_group();

		size_t selected();
		void select(size_t);
		void set_selected(size_t); // не генерирует события event::select

		void append(radio_button&);
		void remove(radio_button&); // throw()
		void clear(); // throw()

		radio_group& operator % (radio_button&); // -> append

		connector<event::select> on_select();

	private:
		friend class radio_button;
		void notify_n_send_event(radio_button&, bool selected);
		size_t notify(radio_button&, bool selected);

		typedef std::vector<radio_button*> vec_t;
		vec_t  radios_;
		size_t selected_;
	};

	
	/*----------------------------------------------------------------------*/

	class radio_button
		: public control
		, public i_tab_group_control
	{
	public:
		radio_button();
		radio_button(str_ref caption, bool selected = false, bool multiline = false);
		~radio_button();

		string caption();
		bool selected();

		void set_caption(str_ref);
		void select();
		void deselect();
		void set_selected(bool = true); // не генерирует события event::select
		void set_multiline(bool = true);

		connector<event::select> on_select();

		i_tab_group* tab_group(); // override

	protected:
		void pre_handle_event(event::processor&); // override

	private:
		void handle_command(event::win32::command&);
		friend class radio_group;
		radio_group* radio_group_;
	};
}

#endif // SVL_WIDGETS_RADIO_BUTTON_HPP
