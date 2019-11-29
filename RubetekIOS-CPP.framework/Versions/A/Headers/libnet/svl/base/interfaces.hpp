#ifndef SVL_BASE_INTERFACES_HPP
#define SVL_BASE_INTERFACES_HPP

#include <svl/base/types.hpp>
#include <svl/base/layout.hpp>

namespace svl
{
	class z_order
	{
	public:
		static z_order top();
		static z_order bottom();
		static z_order under(widget&);
		static z_order above(widget&);

		struct type	{ enum enum_t
		{
			top, bottom, under, above
		};};

		type::enum_t get_type() const;
		widget*      get_widget() const;

	private:
		z_order(type::enum_t, widget*);
		type::enum_t type_;
		widget*      widget_;
	};

	/*----------------------------------------------------------------------*/

	struct i_container
		: noncopyable
	{
		void add_widget(widget&, z_order = z_order::bottom());
		void remove_widget(widget&); // throw()
		void remove_all_widgets(); // throw()

		void reorder(widget&, z_order);

		template <typename Functor> void for_each_widget(Functor);
		template <typename Functor> void recursive_for_each_widget(Functor);

		svl::rect client_rect() const;
		svl::size client_size() const;

		grid_layout& create_layout(unsigned rows, unsigned columns, grid_layout::profile = 0);
		bool has_layout() const; // throw()
		grid_layout& layout();
		grid_layout* get_layout(); // throw()
		void apply_layout();

	protected:
		i_container();
		void resize_widgets(rect const&);
		virtual window* do_get_window() = 0;
		virtual rect2 do_get_margins() = 0;
		virtual svl::rect do_get_client_rect() const = 0;
		virtual ~i_container();

	private:
		grid_layout* layout_;
	};

	/*----------------------------------------------------------------------*/

	struct i_button
	{
		virtual void set_def_style(bool) = 0;
		virtual void click() = 0;

		protected: virtual ~i_button() {}
	};

	/*----------------------------------------------------------------------*/

	struct i_tab_group
	{
		protected: ~i_tab_group() {}
		protected: friend class detail::tab_order; control* active_control_;
	};

	/*----------------------------------------------------------------------*/

	struct i_tab_group_control
	{
		virtual i_tab_group* tab_group() = 0;

		protected: virtual ~i_tab_group_control() {}
	};
}

#endif // SVL_BASE_INTERFACES_HPP
