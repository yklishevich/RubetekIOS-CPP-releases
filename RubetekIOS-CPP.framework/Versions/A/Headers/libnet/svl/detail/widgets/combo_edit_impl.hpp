
namespace svl
{
	void combo_edit::create_combo_edit()
	{
		w32_create_control( SVL_T("COMBOBOX"), CBS_DROPDOWN, WS_EX_CLIENTEDGE );
		HWND child = ::GetWindow(win32_handle(), GW_CHILD);
		if (child != 0)
		{
			w32_child_proc_ = win32::set_wnd_proc(child, &w32_child_proc);
			win32::set_wnd_ptr(child, this);
		}
	}

	combo_edit::combo_edit(str_ref text)
	{
		create_combo_edit();
		set_text(text);
	}

	combo_edit::~combo_edit()
	{
		start_destruction();
		HWND child = ::GetWindow(win32_handle(), GW_CHILD);
		if (child != 0)
		{
			SVL_VERIFY((win32::set_wnd_proc(child, w32_child_proc_)), != 0);
			win32::remove_wnd_ptr(child);
		}
		w32_destroy_control();
	}

	void combo_edit::append( str_ref str )
	{
		w32_send_msg( CB_INSERTSTRING, static_cast<WPARAM>( -1 ), reinterpret_cast<LPARAM>( str.data() ) );
	}

	void combo_edit::insert( str_ref str, size_t index )
	{
		w32_send_msg( CB_INSERTSTRING, index, reinterpret_cast<LPARAM>(str.data()) );
	}

	void combo_edit::remove( size_t index )
	{
		SVL_VERIFY( w32_send_msg( CB_DELETESTRING, index ), != CB_ERR ); // !!! invalid index
	}

	void combo_edit::clear()
	{
		w32_send_msg( CB_RESETCONTENT );
	}

	void combo_edit::select( size_t index )
	{
		set_selected( index );
		event::text_change ev(detail::combo_edit_select, win32_handle());
		handle_event(ev);
	}

	string combo_edit::text() const
	{
		return win32::get_window_text( win32_handle() );
	}

	void combo_edit::set_text(str_ref s)
	{
		win32::set_window_text( win32_handle(), s );
	}

	void combo_edit::change_text(str_ref s)
	{
		set_text(s);
		event::text_change ev(detail::edit_change, win32_handle());
		handle_event(ev);
	}

	void combo_edit::set_selected( size_t index )
	{
		LRESULT result = w32_send_msg( CB_SETCURSEL, index );
		unused(result);
		SVL_ASSERT( result != CB_ERR || index == no_sel );  // !!! invalid index
	}

	size_t combo_edit::items_count() const
	{
		return w32_send_msg( CB_GETCOUNT );
	}

	string combo_edit::item( size_t index ) const
	{
		LRESULT result = w32_send_msg( CB_GETLBTEXTLEN, index );
		SVL_ASSERT( result != CB_ERR ); // !!! invalid index
		if (result != CB_ERR)
		{
			string str( static_cast<size_t>( result ) );
			w32_send_msg( CB_GETLBTEXT, index, reinterpret_cast<LPARAM>( str.data() ) );
			return str.resize( static_cast<std::size_t>(result) );
		}
		return string(1);
	}

	void combo_edit::pre_handle_event( event::processor& ep )
	{
		control::pre_handle_event( ep );

		ep( &combo_edit::handle_command );
	}

	void combo_edit::handle_command( event::win32::command& e )
	{
		switch (e.id())
		{
		case CBN_EDITCHANGE: {
			event::text_change ev(detail::edit_change, win32_handle());
			handle_event(ev);
			break;
			}
		case CBN_SELCHANGE: {
			event::text_change ev(detail::combo_edit_select, win32_handle());
			handle_event(ev);
			break;
			}
		}
	}

	event::target::connector<event::text_change> combo_edit::on_text_change()
	{
		return connection_info();
	}

	LRESULT combo_edit::w32_child_proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		combo_edit* wnd = win32::get_wnd_ptr<combo_edit>(hwnd);

		LRESULT result = SVL_MS(CallWindowProc)(wnd->w32_child_proc_, hwnd, uMsg, wParam, lParam);

		switch (uMsg)
		{
		case WM_KEYDOWN:
			w32_process_wm_keydown(wnd, wParam, lParam);
			break;
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
		}

		return result;
	}
}