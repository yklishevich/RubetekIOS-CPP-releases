#ifndef SVL_BASE_WINDOW_HPP
#define SVL_BASE_WINDOW_HPP

#include <svl/detail/types.hpp>
#include <svl/detail/win32.hpp>
#include <svl/base/events.hpp>
#include <svl/base/string.hpp>
#include <svl/base/types.hpp>

namespace svl
{
	class window
		: public event::target
	{
	public:
		void hide();
		void show();
		void show(bool visible);
		void move(point const&);
		void resize(size const&);
		void resize(svl::rect const&);
		void update();
		void update(svl::rect const&);
		void repaint();
		void repaint(svl::rect const&);

		svl::point pos() const;
		svl::size  size() const;
		svl::rect  rect() const;
		bool       visible() const;

		using target::on_destroy;
		connector<event::move>           on_move();
		connector<event::resize>         on_resize();
		connector<event::paint>          on_paint();
		connector<event::mouse_move>     on_mouse_move();
		connector<event::mouse_key_down> on_mouse_key_down();
		connector<event::mouse_key_up>   on_mouse_key_up();
		// on_mouse_enter() и on_mouse_leave() сигнализируют только при использовании класса mouse_enter_leave_scheme
		connector<event::mouse_enter>    on_mouse_enter();
		connector<event::mouse_leave>    on_mouse_leave();

		~window();

		HWND win32_handle() const;

	protected:
		friend class mouse_enter_leave_scheme;
		window();

		template <typename Event> void send_event(Event& e) /* thread(any) */   { do_send_event( e, detail::event_id_of<Event>::result ); }
		void do_send_event(event::entity&, detail::event_id);

		void do_handle_event(event::processor&); // override
		virtual void pre_handle_event(event::processor&);
		virtual void post_handle_event(event::processor&);

		LRESULT w32_send_msg(UINT msg, WPARAM wparam = 0, LPARAM lparam = 0) const; // throw()
		void w32_create_form_window(DWORD style, DWORD ex_style, COLORREF bg_color = ::GetSysColor(COLOR_BTNFACE), HICON icon = 0 );
		void w32_create_child_window(DWORD style, DWORD ex_style, COLORREF bg_color = ::GetSysColor(COLOR_BTNFACE));
		void w32_set_window_bg_color(COLORREF);
		void w32_destroy_window(); // throw()
		void w32_create_control(tchar const* name, DWORD style = 0, DWORD ex_style = 0);
		void w32_destroy_control(); // throw()

		virtual void do_resize(svl::size const&);
		virtual void do_resize(svl::rect const&);

		static bool w32_process_wm_keydown(window*, WPARAM, LPARAM);

	private:
		friend class widget;
		friend class graphics::canvas;
		friend class graphics::buffered_canvas;
		friend struct i_container;
		virtual svl::rect do_get_rect() const = 0;
		virtual form*     do_get_parent_form() const = 0;

		void w32_create_window(WNDPROC, DWORD class_style, DWORD style, DWORD ex_style, COLORREF bg_color, HICON icon);
		static bool w32_def_window_proc(window*, UINT, WPARAM, LPARAM);
		static LRESULT CALLBACK w32_form_window_proc (HWND, UINT, WPARAM, LPARAM);
		static LRESULT CALLBACK w32_child_window_proc(HWND, UINT, WPARAM, LPARAM);
		static LRESULT CALLBACK w32_control_proc     (HWND, UINT, WPARAM, LPARAM);
		static LRESULT CALLBACK w32_groupbox_proc    (HWND, UINT, WPARAM, LPARAM);
		static bool w32_process_wm_command(WPARAM, LPARAM);
		
		HWND w32_handle_;
		union
		{
			WNDPROC w32_wnd_proc_;
			ATOM    w32_wnd_class_;
		};

		SVL_IN_DEBUG_2(bool w32_is_control_;)
	};

	/*----------------------------------------------------------------------*/

	class mouse_enter_leave_scheme
	{
	public:
		mouse_enter_leave_scheme(window& wnd);

	private:
		void on_mouse_move(event::mouse_move&);
		void on_mouse_leave(event::mouse_leave&);

	private:
		event::connection connection_mm_;
		event::connection connection_ml_;
		window& wnd_;
		bool is_enter_;
	};
}

#endif // SVL_BASE_WINDOW_HPP
