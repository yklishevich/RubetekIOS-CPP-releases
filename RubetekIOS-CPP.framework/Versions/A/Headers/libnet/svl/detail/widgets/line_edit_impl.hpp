
namespace svl
{
	line_edit::line_edit()
	{
		w32_create_control(SVL_T("EDIT"), ES_AUTOHSCROLL, WS_EX_CLIENTEDGE);
	}


	line_edit::line_edit( str_ref text, bool read_only )
	{
		w32_create_control(SVL_T("EDIT"), ES_AUTOHSCROLL, WS_EX_CLIENTEDGE);
		set_text( text );
		if (read_only)
			set_read_only();
	}

	line_edit::~line_edit()
	{
		start_destruction();
		w32_destroy_control();
	}

	void line_edit::set_text(str_ref s)
	{
		win32::set_window_text( win32_handle(), s );
	}

	string line_edit::text() const
	{
		return win32::get_window_text( win32_handle() );
	}

	bool line_edit::read_only() const
	{
		return (win32::get_window_style( win32_handle() ) & ES_READONLY) != 0;
	}

	void line_edit::set_read_only(bool v)
	{
		w32_send_msg( EM_SETREADONLY, v ? TRUE : FALSE );
	}

	event::target::connector<event::text_change> line_edit::on_text_change()
	{
		return connection_info();
	}

	void line_edit::pre_handle_event( event::processor& ep )
	{
		control::pre_handle_event( ep );

		ep( &line_edit::handle_command );
	}

	void line_edit::handle_command( event::win32::command& e )
	{
		switch (e.id())
		{
		case EN_CHANGE: {
			event::text_change ev(detail::edit_change, win32_handle());
			handle_event(ev);
			break;
			}
		}
	}
}
