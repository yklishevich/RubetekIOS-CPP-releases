
namespace svl
{
	window::window()
	{
		SVL_IN_DEBUG_2( w32_handle_ = 0; )
	}

	window::~window()
	{
		SVL_ASSERT_2( w32_handle_ == 0 );
		//start_destruction();
	}

	void window::do_handle_event(event::processor& p)
	{
		pre_handle_event(p);
		target::do_handle_event(p);
		post_handle_event(p);
	}

	void window::pre_handle_event(event::processor&)
	{}

	void window::post_handle_event(event::processor&)
	{}

	void window::do_send_event(event::entity& e, detail::event_id id)
	{
		w32_send_msg(WM_SVL_EVENT, id, reinterpret_cast<LPARAM>( &e ));
	}

	event::target::connector<event::move> window::on_move()
	{
		return connection_info();
	}

	event::target::connector<event::resize> window::on_resize()
	{
		return connection_info();
	}

	event::target::connector<event::paint> window::on_paint()
	{
		return connection_info();
	}
	
	event::target::connector<event::mouse_move> window::on_mouse_move()
	{
		return connection_info();
	}

	event::target::connector<event::mouse_key_down> window::on_mouse_key_down()
	{
		return connection_info();
	}
	
	event::target::connector<event::mouse_key_up> window::on_mouse_key_up()
	{
		return connection_info();
	}

	event::target::connector<event::mouse_enter> window::on_mouse_enter()
	{
		return connection_info();
	}

	event::target::connector<event::mouse_leave> window::on_mouse_leave()
	{
		return connection_info();
	}

	void window::show()
	{
		::ShowWindow( win32_handle(), SW_SHOWNOACTIVATE );
	}

	void window::show(bool visible)
	{
		::ShowWindow( win32_handle(), visible ? SW_SHOWNOACTIVATE : SW_HIDE );
	}

	void window::hide()
	{
		::ShowWindow( win32_handle(), SW_HIDE );
	}

	void window::move( point const& p )
	{
		SVL_VERIFY( (::SetWindowPos( win32_handle(), 0, p.x, p.y, 0, 0,
			SWP_NOACTIVATE | SWP_NOSIZE | SWP_NOZORDER )), != 0);
	}

	void window::resize( svl::size const& s )
	{
		do_resize( s );
		update();
	}

	void window::resize( svl::rect const& r )
	{
		do_resize( r );
		update();
	}

	void window::do_resize( svl::size const& s )
	{
		SVL_VERIFY( (::SetWindowPos( win32_handle(), 0, 0, 0, s.dx, s.dy,
			SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOZORDER )), != 0);
	}

	void window::do_resize( svl::rect const& r )
	{
		SVL_VERIFY( (::SetWindowPos( win32_handle(), 0, r.x, r.y, r.dx, r.dy,
			SWP_NOACTIVATE | SWP_NOZORDER )), != 0);
	}

	svl::point window::pos() const
	{
		return do_get_rect().pos();
	}

	svl::size window::size() const
	{
		return do_get_rect().size();
	}

	svl::rect window::rect() const
	{
		return do_get_rect();
	}

	bool window::visible() const
	{
		return ::IsWindowVisible( win32_handle() ) != 0;
	}

	void window::update()
	{
		SVL_VERIFY( ::InvalidateRect( win32_handle(), 0, TRUE ), != 0 );
	}

	void window::update(svl::rect const& r)
	{
		RECT R = win32::from_rect( r );
		SVL_VERIFY( ::InvalidateRect( win32_handle(), &R, TRUE ), != 0 );
	}

	void window::repaint()
	{
		SVL_VERIFY( ::RedrawWindow( win32_handle(), 0, 0, RDW_ERASE | RDW_INVALIDATE | RDW_UPDATENOW ), != 0 );
	}

	void window::repaint(svl::rect const& r)
	{
		RECT R = win32::from_rect( r );
		SVL_VERIFY( ::RedrawWindow( win32_handle(), &R, 0, RDW_ERASE | RDW_INVALIDATE | RDW_UPDATENOW ), != 0 );
	}

	/*----------------------------------------------------------------------*/

	HWND window::win32_handle() const // throw()
	{
		return w32_handle_;
	}

	LRESULT window::w32_send_msg( UINT msg, WPARAM wparam, LPARAM lparam ) const // throw()
	{
		return SVL_MS(SendMessage)( w32_handle_, msg, wparam, lparam );
	}

	/*----------------------------------------------------------------------*/

	void window::w32_create_control( tchar const* name, DWORD style, DWORD ex_style )
	{
		SVL_ASSERT_2( w32_handle_ == 0 );

		try
		{
			style |= WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE;
			w32_handle_ = win32::create_window( name, get_env().sys_window, style, ex_style );
			bool is_group_box = (style & 0x0F) == BS_GROUPBOX;
			w32_wnd_proc_ = win32::set_wnd_proc( w32_handle_, is_group_box ? &w32_groupbox_proc : &w32_control_proc );
			if (w32_wnd_proc_ == 0)
				throw win32_error("SetWindowLongPtr(GWLP_WNDPROC)");
			win32::set_wnd_ptr( w32_handle_, this );
		}
		catch (...)
		{
			if (w32_wnd_proc_ != 0)
				SVL_VERIFY( (win32::set_wnd_proc( w32_handle_, w32_wnd_proc_ )), != 0 );
			if (w32_handle_ != 0)
				win32::destroy_window( w32_handle_ );
			throw;
		}

		w32_send_msg(WM_SETFONT, (WPARAM)::GetStockObject(DEFAULT_GUI_FONT));

		SVL_IN_DEBUG_2( w32_is_control_ = true; )
	}

	void window::w32_destroy_control() // throw()
	{
		SVL_ASSERT_2( w32_handle_ != 0 );
		SVL_ASSERT_2( w32_is_control_ );

		SVL_VERIFY( (win32::set_wnd_proc( w32_handle_, w32_wnd_proc_ )), != 0);
		win32::remove_wnd_ptr( w32_handle_ );
		win32::destroy_window( w32_handle_ );

		SVL_IN_DEBUG_2( w32_handle_ = 0 );
	}

	/*----------------------------------------------------------------------*/

	void window::w32_create_window(WNDPROC wndproc, DWORD class_style, DWORD style, DWORD ex_style, COLORREF bg_color, HICON icon)
	{
		SVL_ASSERT_2( w32_handle_ == 0 );

		unsigned __int64 v = reinterpret_cast<unsigned __int64>(this);
		unsigned const __int64 h = 0x01234567u;
		unsigned const __int64 l = 0x89ABCDEFu;
		tstring class_name = SVL_T("SVL_WIN_") + to_hex(v ^ (h << 32) ^ l, sizeof(void*));

		HBRUSH bg_brush = 0;

		try
		{
			bg_brush = win32::create_solid_brush( bg_color );
			w32_wnd_class_ = win32::register_class( class_name.c_str(), wndproc, class_style, icon, bg_brush);
			bg_brush = 0;// за удаление bg_brush ответственность несет теперь ќ—
			w32_handle_ = win32::create_window( class_name.c_str(), get_env().sys_window, style, ex_style );
			win32::set_wnd_ptr( w32_handle_, this );
		}
		catch (...)
		{
			if (bg_brush != 0)
				win32::delete_object( bg_brush );
			if (w32_wnd_class_ != 0)
				win32::unregister_class( w32_wnd_class_ );
			if (w32_handle_ != 0)
				win32::destroy_window( w32_handle_ );
			throw;
		}

		w32_send_msg(WM_SETFONT, (WPARAM)::GetStockObject(DEFAULT_GUI_FONT));
		SVL_IN_DEBUG_2( w32_is_control_ = false; )
	}

	void window::w32_create_child_window(DWORD style, DWORD ex_style, COLORREF bg_color)
	{
		DWORD st = style | WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE;
		w32_create_window( &window::w32_child_window_proc, CS_PARENTDC, st, ex_style, bg_color, 0 );
	}

	void window::w32_create_form_window(DWORD style, DWORD ex_style, COLORREF bg_color, HICON icon)
	{
		w32_create_window( &window::w32_form_window_proc, 0, style, ex_style, bg_color, icon );
	}

	void window::w32_destroy_window() // throw()
	{
		SVL_ASSERT_2( w32_handle_ != 0 );
		SVL_ASSERT_2( !w32_is_control_ );

		win32::remove_wnd_ptr( w32_handle_ );
		win32::destroy_window( w32_handle_ );
		win32::unregister_class( w32_wnd_class_ );

		SVL_IN_DEBUG_2( w32_handle_ = 0 );
	}

	void window::w32_set_window_bg_color(COLORREF color)
	{
		SVL_ASSERT_2( !w32_is_control_ );

		HBRUSH bg_brush = win32::create_solid_brush( color );
		bg_brush = (HBRUSH) SVL_MS(SetClassLongPtr)( w32_handle_, GCLP_HBRBACKGROUND, (LONG_PTR)bg_brush );
		win32::delete_object (bg_brush );
	}

	/*----------------------------------------------------------------------*/

	bool window::w32_process_wm_keydown(window* wnd, WPARAM wParam, LPARAM /*lParam*/)
	{
		event::key_down ev( static_cast<int>( wParam ) );
		wnd->handle_event( ev );
		if (form* frm = wnd->do_get_parent_form())
			frm->handle_event( ev );
		return true;
	}

	bool window::w32_process_wm_command(WPARAM wParam, LPARAM lParam)
	{
		if (lParam == 0)
			return false;

		unsigned notify_code = static_cast<unsigned>( wParam >> 16 );
		HWND ctrl_hwnd = reinterpret_cast<HWND>( lParam );
		window* ctrl = win32::get_wnd_ptr( ctrl_hwnd );
		if (ctrl == 0)
			return false;

		SVL_ASSERT_2( ctrl->w32_is_control_ );

		event::win32::command ev( notify_code );
		return ctrl->handle_event( ev );
	}

	/*----------------------------------------------------------------------*/

	bool window::w32_def_window_proc(window* wnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		struct local
		{
			enum type { down, up, move };
			static bool process_mouse_event(type t, unsigned state, WPARAM wParam, LPARAM lParam, window* wnd)
			{
				int xy = static_cast<int>(lParam);
				point p(xy << 16 >> 16, xy >> 16);
				unsigned state_ex = 
					(((wParam & MK_LBUTTON) != 0) << 0) |
					(((wParam & MK_MBUTTON) != 0) << 2) |
					(((wParam & MK_RBUTTON) != 0) << 4) |
					(((wParam & MK_SHIFT)   != 0) << 6) |
					(((wParam & MK_CONTROL) != 0) << 7);
				if (t == up)
				{
					event::mouse_key_up ev(p, state | state_ex);
					return wnd->handle_event(ev);
				}
				else if (t == down)
				{
					event::mouse_key_down ev(p, state | state_ex);
					return wnd->handle_event(ev);
				}
				else
				{
					SVL_ASSERT(t == move);
					event::mouse_move ev(p, state | state_ex);
					return wnd->handle_event(ev);
				}
			}
		};
		
		switch (uMsg)
		{
		case WM_DESTROY: {
			SVL_ASSERT_FALSE();
			return true;
			}
		case WM_CLOSE: {
			SVL_ASSERT_FALSE();
			return true;
			}
		case WM_MOVE: {
			event::move ev;
			return wnd->handle_event( ev );
			}
		case WM_SIZE: {
			event::resize ev;
			return wnd->handle_event( ev );
			}
		case WM_COMMAND: {
			return w32_process_wm_command( wParam, lParam );
			}
		case WM_LBUTTONDOWN:
			return local::process_mouse_event(local::down, 0x02, wParam, lParam, wnd);
		case WM_MBUTTONDOWN:
			return local::process_mouse_event(local::down, 0x08, wParam, lParam, wnd);
		case WM_RBUTTONDOWN:
			return local::process_mouse_event(local::down, 0x20, wParam, lParam, wnd);
		case WM_LBUTTONUP:
			return local::process_mouse_event(local::up,   0x02, wParam, lParam, wnd);
		case WM_MBUTTONUP:
			return local::process_mouse_event(local::up,   0x08, wParam, lParam, wnd);
		case WM_RBUTTONUP:
			return local::process_mouse_event(local::up,   0x20, wParam, lParam, wnd);
		case WM_MOUSEMOVE:
			return local::process_mouse_event(local::move, 0x00, wParam, lParam, wnd);
		case WM_SETFOCUS: {
			event::get_focus ev;
			wnd->handle_event( ev );
			break;
			}
		case WM_KILLFOCUS: {
			event::lose_focus ev;
			wnd->handle_event( ev );
			break;
			}
		case WM_MOUSELEAVE: {
			event::mouse_leave ev;
			wnd->handle_event(ev);
			break;
			}
		case WM_SVL_EVENT: {
			return wnd->handle_event(*reinterpret_cast<event::entity*>( lParam ), static_cast<detail::event_id>( wParam ));
			}
		}
		return false;
	}

	LRESULT CALLBACK window::w32_control_proc( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
	{
		window* wnd = win32::get_wnd_ptr( hwnd );
		SVL_ASSERT_2( wnd != 0 );

		LRESULT result = SVL_MS(CallWindowProc)( wnd->w32_wnd_proc_, hwnd, uMsg, wParam, lParam );

		switch (uMsg)
		{
		case WM_PAINT: {
			event::paint ev;
			wnd->handle_event( ev );
			break;
			}
		case WM_KEYDOWN: {
			w32_process_wm_keydown( wnd, wParam, lParam );
			break;
			}
		default:
			w32_def_window_proc( wnd, uMsg, wParam, lParam );
		}

		return result;
	}

	LRESULT CALLBACK window::w32_groupbox_proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)
		{
		case WM_ERASEBKGND: {
			HDC dc = reinterpret_cast<HDC>(wParam);
			RECT r;
			SVL_VERIFY( ::GetClientRect(hwnd, &r), != 0 );
			HBRUSH brush = ::GetSysColorBrush(COLOR_BTNFACE);
			SVL_VERIFY( ::FillRect(dc, &r, brush), != 0 );
			SVL_VERIFY( ::UpdateWindow( hwnd ), != 0 );
			return TRUE;
			}
		case WM_PRINTCLIENT: {
			return SVL_MS(SendMessage)( ::GetParent(hwnd), uMsg, wParam, lParam );
			}
		default:
			return w32_control_proc( hwnd, uMsg, wParam, lParam );
		}
	}

	LRESULT CALLBACK window::w32_child_window_proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		bool processed = false;
		LRESULT result = 0;
		widget* wnd = win32::get_wnd_ptr<widget>( hwnd );
		if (wnd != 0)
		{
			switch (uMsg)
			{
			case WM_PAINT: {
				PAINTSTRUCT ps;
				::BeginPaint( hwnd, &ps );
				wnd->get_env().device_contexts.push( ps.hdc, *wnd );
				event::paint ev;
				wnd->handle_event( ev );
				wnd->get_env().device_contexts.pop( ps.hdc );
				::EndPaint( hwnd, &ps );
				processed = true;
				break;
				}
			case WM_KEYDOWN: {
				w32_process_wm_keydown( wnd, wParam, lParam );
				processed = true;
				break;
				}
			default:
				processed = w32_def_window_proc( wnd, uMsg, wParam, lParam );
			}
		}

		return processed ? result : SVL_MS(DefWindowProc)( hwnd, uMsg, wParam, lParam );
	}

	LRESULT CALLBACK window::w32_form_window_proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		bool processed = false;
		LRESULT result = 0;
		form* wnd = win32::get_wnd_ptr<form>( hwnd );
		if (wnd != 0)
		{
			switch (uMsg)
			{
			case WM_PAINT: {
				PAINTSTRUCT ps;
				::BeginPaint( hwnd, &ps );
				wnd->get_env().device_contexts.push( ps.hdc, *wnd );
				event::paint ev;
				wnd->handle_event( ev );
				wnd->get_env().device_contexts.pop( ps.hdc );
				::EndPaint( hwnd, &ps );
				processed = true;
				break;
				}
			case WM_CLOSE: {
				event::close ev;
				wnd->handle_event( ev );
				processed = true; // в любом случае не допускаем обработку DefWindowProc дл€ WM_CLOSE
				break;
				}
			case WM_KEYDOWN: {
				event::key_down ev( static_cast<unsigned>( wParam ) );
				wnd->handle_event( ev );
				processed = true;
				break;
				}
			case WM_ACTIVATE: {
				event::activate ev( wParam != WA_INACTIVE );
				processed = wnd->handle_event( ev );
				break;
				}
			default:
				processed = w32_def_window_proc( wnd, uMsg, wParam, lParam );
			}
		}

		return processed ? result : SVL_MS(DefWindowProc)( hwnd, uMsg, wParam, lParam );
	}

	/*----------------------------------------------------------------------*/

	mouse_enter_leave_scheme::mouse_enter_leave_scheme(window& wnd)
		: wnd_(wnd)
		, is_enter_(false)
	{
		wnd.on_mouse_move().add(&mouse_enter_leave_scheme::on_mouse_move, this, connection_mm_);
		wnd.on_mouse_leave().add(&mouse_enter_leave_scheme::on_mouse_leave, this, connection_ml_);
	}

	void mouse_enter_leave_scheme::on_mouse_move(event::mouse_move& e)
	{
		if (!is_enter_)
		{
			is_enter_ = true;
			event::mouse_enter ev(e.cursor(), e.state());
			wnd_.handle_event(ev);

			TRACKMOUSEEVENT tme;
			tme.cbSize = sizeof(tme);
			tme.dwFlags = TME_LEAVE;
			tme.hwndTrack = wnd_.win32_handle();
			tme.dwHoverTime = HOVER_DEFAULT;
			SVL_VERIFY(::TrackMouseEvent(&tme), != 0);
		}
	}

	void mouse_enter_leave_scheme::on_mouse_leave(event::mouse_leave&)
	{
		is_enter_ = false;
	}
}
