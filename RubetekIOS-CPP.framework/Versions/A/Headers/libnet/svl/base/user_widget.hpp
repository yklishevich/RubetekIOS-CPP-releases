#ifndef SVL_BASE_USER_WIDGET_HPP
#define SVL_BASE_USER_WIDGET_HPP

#include <svl/base/widget.hpp>

namespace svl
{
	class user_widget
		: public widget
	{
	public:
		~user_widget();

	protected:
		user_widget(color back_color = color::white);
	};
}

#endif // SVL_BASE_USER_WIDGET_HPP
