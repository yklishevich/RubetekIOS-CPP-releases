#ifndef SVL_WIDGETS_LIST_BOX_HPP
#define SVL_WIDGETS_LIST_BOX_HPP

#include <svl/base/widget.hpp>

namespace svl
{
	class list_box
		: public control
	{
	public:
		list_box();

		void append(str_ref);
		void insert(std::size_t index, str_ref);
		void change(std::size_t index, str_ref);
		void remove(std::size_t index);
		void clear();

		std::size_t top_index() const;
		void set_top_index(std::size_t index);
		
		std::size_t items_count() const;
		string item(std::size_t index) const;

		void set_redraw_flag(bool);

		connector<event::select> on_select();

		~list_box();
	protected:
		void pre_handle_event(event::processor&);

	private:
		void handle_command(event::win32::command&);
	};
}

#endif // SVL_WIDGETS_LIST_BOX_HPP
