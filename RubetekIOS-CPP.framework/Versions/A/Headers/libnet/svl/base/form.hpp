#ifndef SVL_FORM_HPP
#define SVL_FORM_HPP

#include <svl/base/window.hpp>
#include <svl/base/interfaces.hpp>
#include <svl/base/layout.hpp>

namespace svl
{
	class form 
		: public window
		, public i_container
	{
	public:
		struct style { enum
		{
			system_menu     = 0x01,
			minimize_box    = 0x02,
			maximize_box    = 0x04,
			resizable       = 0x08,
			short_caption   = 0x10,
			close_on_escape = 0x20,
			dialog          = system_menu,
			frame           = system_menu | minimize_box | maximize_box | resizable,
			tool            = system_menu | short_caption | resizable
		};};

		form();
		form(str_ref caption, unsigned style = style::dialog);
		~form();

		string caption() const;
		void set_caption(str_ref);

		bool  has_master() const; // throw()
		form& master() const;
		form* get_master() const; // throw()
		void set_master(form&);
		void set_no_master();

		bool active() const; // throw()
		void activate();

		bool enabled() const;
		void enable();
		void disable();

		void set_back_color(color);

		void set_style(unsigned);
		void add_style(unsigned);
		void remove_style(unsigned);

		void focus();

		bool modal() const; // throw()
		void do_modal(form&);
		void end_modal();

		void set_def_button(i_button&);
		void set_no_def_button();

		bool maximized() const;
		void maximize();
		void normalize();

		connector<event::close>      on_close();
		connector<event::activate>   on_activate();

	protected:
		void pre_handle_event (event::processor&); // override
		void post_handle_event(event::processor&); // override

	private:
		friend class tab_order;
		friend class control;
		void pre_handle_close (event::close&);
		void post_handle_close(event::close&);
		void handle_keydown(event::key_down&);
		void handle_resize (event::resize&);
		void handle_activate(event::activate&);
		void handle_lose_focus(event::lose_focus&);
		void create_form();
		svl::rect do_get_rect() const; // override
		svl::rect do_get_client_rect() const; // override
		form* do_get_parent_form() const; // override
		window* do_get_window(); // override
		rect2 do_get_margins(); // override

		HICON              icon_;
		detail::tab_order* tab_order_;
		form*              master_;
		std::list<form*>   slaves_;
		bool               modal_: 1;
		bool               close_on_escape_: 1;
	};

	/*----------------------------------------------------------------------*/

	class tab_order
		: svl::noncopyable
	{
	public:
		tab_order(form&);

		tab_order& add_control(control&);
		tab_order& operator % (control&); // аналог add_control(...)
		tab_order& insert_front();
		tab_order& insert_back();
		tab_order& insert_after(control&);
		tab_order& insert_before(control&);
		tab_order& remove(control&); // throw()
		tab_order& clear(); // throw()

	private:
		form&       form_;
		std::size_t pos_;
	};
}

#endif //SVL_FORM_HPP
