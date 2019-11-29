#ifndef SVL_WIDGETS_GROUP_BOX_HPP
#define SVL_WIDGETS_GROUP_BOX_HPP

#include <svl/base/widget.hpp>

namespace svl
{
	class group_box
		: public container
	{
	public:
		group_box();
		group_box(str_ref caption);
		~group_box();

		void set_caption( str_ref );
		string caption() const;

	private:
		rect2 do_get_margins(); // override
	};
}

#endif // SVL_WIDGETS_GROUP_BOX_HPP
