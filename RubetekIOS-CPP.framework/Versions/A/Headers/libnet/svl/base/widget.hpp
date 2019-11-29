#ifndef SVL_BASE_WIDGET_HPP
#define SVL_BASE_WIDGET_HPP

#include <svl/base/window.hpp>
#include <svl/base/interfaces.hpp>
#include <svl/base/graphics.hpp>

namespace svl
{
	class widget
		: public window
	{
	public:
		bool has_parent() const; // throw()
		bool has_parent_form() const; // throw()
		
		window& parent() const;
		form&   parent_form() const;
		window* get_parent() const; // throw()
		form*   get_parent_form() const; // throw()

		void set_no_parent();

		~widget();

	protected:
		widget();
		void start_destruction(); // throw()

	private:
		friend struct i_container;
		svl::rect do_get_rect() const; // override
		form* do_get_parent_form() const; // override

		window* parent_;
	};

	/*----------------------------------------------------------------------*/

	class control
		: public widget
	{
	public:
		bool enabled();
		bool focused();

		void enable();
		void enable(bool);
		void disable();
		void focus();

		~control();

	protected:
		control();
		void pre_handle_event(event::processor&); // override

	private:
		void handle_get_focus(event::get_focus&);
		void handle_lose_focus(event::lose_focus&);
	};

	/*----------------------------------------------------------------------*/

	class container
		: public widget
		, public i_container
	{
	public:
		void enable();
		void disable();

		~container();

	protected:
		container();
		void start_destruction(); // throw()

	private:
		window* do_get_window(); // override
		svl::rect do_get_client_rect() const; // override
		void do_resize( svl::rect const& r ); // override
	};
}

#include <svl/detail/interfaces_incl.hpp>

#endif // SVL_BASE_WIDGET_HPP
