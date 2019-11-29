#ifndef SVL_WIDGETS_LABEL_HPP
#define SVL_WIDGETS_LABEL_HPP

#include <svl/base/widget.hpp>

namespace svl
{
	class label
		: public control
	{
	public:
		struct style { enum enum_t
		{
			left_align,
			center_align,
			right_align,
			multiline
		};};

		label();
		label(str_ref text, style::enum_t = style::left_align, bool border = false);

		void set_text(str_ref);
		string text() const; 

		void set_style(style::enum_t);
		void set_border(bool = true);

		~label();
	};
}

#endif // SVL_WIDGETS_LABEL_HPP
