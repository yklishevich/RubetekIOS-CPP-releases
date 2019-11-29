
namespace svl
{
	check_box::check_box()
	{
		w32_create_control( SVL_T("BUTTON"), BS_AUTOCHECKBOX );
	}

	check_box::check_box( str_ref caption, bool third_state, tribool initial_state, bool multiline )
	{
		w32_create_control( SVL_T("BUTTON"), BS_AUTOCHECKBOX );
		set_caption( caption );
		enable_third_state( third_state );
		set_checked( initial_state );
		set_multiline( multiline );
	}

	check_box::~check_box()
	{
		start_destruction();
		w32_destroy_control();
	}

	void check_box::set_caption( str_ref str )
	{
		win32::set_window_text( win32_handle(), str );
	}

	string check_box::caption() const
	{
		return win32::get_window_text( win32_handle() );
	}

	void check_box::enable_third_state( bool val )
	{
		win32::modify_window_style( win32_handle(), 0x0F, val ? BS_AUTO3STATE : BS_AUTOCHECKBOX );
	}

	void check_box::set_multiline( bool m )
	{
		win32::modify_window_style( win32_handle(), !m * BS_MULTILINE, m * BS_MULTILINE );
		update();
	}

	void check_box::check( tribool val )
	{
		SVL_ASSERT( third_state_enabled() || !indeterminate(val) );

		set_checked( val );
		event::check ev( val );
		handle_event( ev );
	}

	void check_box::uncheck()
	{
		check( false );
	}

	void check_box::set_checked( tribool val )
	{
		unsigned value = val ? BST_CHECKED : (!val ? BST_UNCHECKED : BST_INDETERMINATE);
		w32_send_msg( BM_SETCHECK, value );
	}

	tribool check_box::checked() const
	{
		LRESULT rslt = w32_send_msg( BM_GETCHECK );
		if (rslt == BST_INDETERMINATE)
			return indeterminate;
		return rslt == BST_CHECKED;
	}

	bool check_box::third_state_enabled() const
	{
		unsigned style = win32::get_window_style( win32_handle() ) & 0x0F;
		return style == BS_AUTO3STATE;
	}

	event::target::connector<event::check> check_box::on_check()
	{
		return connection_info();
	}

	void check_box::pre_handle_event( event::processor& ep )
	{
		control::pre_handle_event( ep );

		ep( &check_box::handle_command );
	}

	void check_box::handle_command( event::win32::command& e )
	{
		if (e.id() == BN_CLICKED)
		{
			event::check ev( checked() );
			handle_event( ev );
		}
	}
}