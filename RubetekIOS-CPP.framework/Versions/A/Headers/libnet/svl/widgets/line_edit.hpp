#ifndef SVL_WIDGETS_LINE_EDIT_HPP
#define SVL_WIDGETS_LINE_EDIT_HPP

#include <svl/base/widget.hpp>

namespace svl
{
	class line_edit
		: public control
	{
	public:
		line_edit();
		line_edit(str_ref text, bool read_only = false);
		~line_edit();

		bool read_only() const;
		string text() const;

		void set_text(str_ref);
		void set_read_only(bool = true);

		connector<event::text_change> on_text_change();

	protected:
		void pre_handle_event(event::processor&);

	private:
		void handle_command(event::win32::command&);
	};
}

#endif // SVL_WIDGETS_LINE_EDIT_HPP
