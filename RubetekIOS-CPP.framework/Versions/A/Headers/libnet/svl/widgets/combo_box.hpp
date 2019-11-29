#ifndef SVL_WIDGETS_COMBO_BOX_HPP
#define SVL_WIDGETS_COMBO_BOX_HPP

#include <svl/base/widget.hpp>

namespace svl
{
	class combo_box
		: public control
	{
	public:
		combo_box();
		template <typename IIter> combo_box(IIter first, IIter last);
		~combo_box();

		void append(str_ref);
		template <typename IIter> void append(IIter first, IIter last);
		void insert(str_ref, size_t index);
		void remove(size_t index);
		void clear();
		void select(size_t index);
		void set_selected(size_t index); // не генерирует события event::select

		size_t selected() const;
		string selected_item() const;
		size_t items_count() const;
		string item(size_t index) const;

		connector<event::select> on_select();

#if SVL_USE_BOOST_RANGE
		template <class Range> combo_box(Range const&);
		template <class Range> void append_range(Range const&);
#endif

	protected:
		void pre_handle_event(event::processor&);

	private:
		void create_combo_box();
		void handle_command(event::win32::command&);
	};
}

#include <svl/detail/widgets/combo_box_incl.hpp>

#endif // SVL_WIDGETS_COMBO_BOX_HPP
