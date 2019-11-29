
namespace svl
{
	button::button()
	{
		w32_create_control( SVL_T("BUTTON"), BS_PUSHBUTTON );
	}
	
	button::button(str_ref caption)
	{
		w32_create_control( SVL_T("BUTTON"), BS_PUSHBUTTON );
		set_caption( caption );
	}

	button::~button()
	{
		start_destruction();
		w32_destroy_control();
	}

	string button::caption() const
	{
		return win32::get_window_text( win32_handle() );
	}

	void button::set_caption(str_ref str)
	{
		win32::set_window_text( win32_handle(), str );
	}

	void button::click() // thread(any)
	{
		event::click ev;
		send_event( ev );
	}

	void button::set_def_style(bool def_style)
	{
		DWORD style = win32::get_window_style( win32_handle() );

		style = (style & 0xFFFFFFF0) | (def_style ? BS_DEFPUSHBUTTON : BS_PUSHBUTTON);

		win32::set_window_style( win32_handle(), style );

		repaint();
	}

	void button::pre_handle_event( event::processor& ep )
	{
		control::pre_handle_event( ep );
		ep( &button::handle_command );
	}

	void button::handle_command( event::win32::command& e )
	{
		if (e.id() == BN_CLICKED)
		{
			event::click ev;
			handle_event( ev );
		}
	}

	event::target::connector<event::click> button::on_click()
	{
		return connection_info();
	}
}