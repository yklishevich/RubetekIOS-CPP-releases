#ifndef SVL_WIDGETS_PANEL_HPP
#define SVL_WIDGETS_PANEL_HPP

#include <svl/base/widget.hpp>

namespace svl
{
	class panel
		: public container
	{
	public:
		typedef graphics::edge edge;

		panel(edge = edge::etched);
		~panel();

		void set_edge(edge);

	protected:
		void pre_handle_event(event::processor&); // override

	private:
		void handle_paint(event::paint&);
		rect2 do_get_margins(); // override

		edge edge_;
	};
}

#endif // SVL_WIDGETS_PANEL_HPP
