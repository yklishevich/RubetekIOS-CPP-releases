#ifndef SVL_WIDGETS_PROGRESS_BAR_HPP
#define SVL_WIDGETS_PROGRESS_BAR_HPP

#include <svl/base/widget.hpp>

namespace svl
{
	class progress_bar
		: public widget
	{
	public:
		progress_bar();
		progress_bar(int min_pos, int max_pos, bool vertical = false);
		~progress_bar();

		int position() const;
		int min_position() const;
		int max_position() const;

		void set_range(int min_pos, int max_pos);
		void set_vertical(bool = true);
		void set_position(int);

		bool marquee_mode_enabled() const;
		void enable_marquee_mode(bool = true);
		void disable_marquee_mode();
	};
}

#endif //SVL_WIDGETS_PROGRESS_BAR_HPP
