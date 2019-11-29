#ifndef SVL_WIDGETS_COMBO_EDIT_HPP
#define SVL_WIDGETS_COMBO_EDIT_HPP

#include <svl/base/widget.hpp>

namespace svl
{
	class combo_edit
		: public control
	{
	public:
		combo_edit(str_ref text = SVL_T(""));
		template <typename IIter> combo_edit(str_ref text, IIter first, IIter last);
		~combo_edit();

		void append(str_ref);
		template <typename IIter> void append(IIter first, IIter last);
		void insert(str_ref, size_t index);
		void remove(size_t index);
		void clear();
		void select(size_t index);
		void set_selected(size_t index); // не генерирует события event::select
		void change_text(str_ref);
		void set_text(str_ref); // не генерирует события event::select

		string text() const;
		size_t items_count() const;
		string item(size_t index) const;

		connector<event::text_change> on_text_change();

#if SVL_USE_BOOST_RANGE
		template <class Range> combo_edit(str_ref text, Range const&);
		template <class Range> void append_range(Range const&);
#endif

	protected:
		void pre_handle_event(event::processor&);

	private:
		void create_combo_edit();
		void handle_command(event::win32::command&);
		static LRESULT CALLBACK w32_child_proc(HWND, UINT, WPARAM, LPARAM);
		WNDPROC w32_child_proc_;
	};
}

#include <svl/detail/widgets/combo_edit_incl.hpp>

#endif // SVL_WIDGETS_COMBO_EDIT_HPP
