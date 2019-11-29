
namespace svl
{
	label::label()
	{
		w32_create_control( SVL_T("STATIC"), 0 /*, WS_EX_CLIENTEDGE*/ );
		set_style( style::left_align );
	}

	label::label(str_ref text, style::enum_t st, bool border)
	{
		w32_create_control( SVL_T("STATIC"), 0 /*, WS_EX_CLIENTEDGE*/ );
		set_text( text );
		set_style( st );
		set_border( border );
	}

	label::~label()
	{
		start_destruction();
		w32_destroy_control();
	}

	void label::set_text(str_ref str)
	{
		win32::set_window_text( win32_handle(), str );
	}

	string label::text() const
	{
		return win32::get_window_text( win32_handle() );
	}

	void label::set_style( style::enum_t st )
	{
		unsigned rem_flags = SS_LEFT | SS_CENTER | SS_RIGHT | SS_CENTERIMAGE;
		unsigned add_flags = SS_LEFT; // for multiline

		if (st == style::left_align)
			add_flags = SS_LEFT | SS_CENTERIMAGE;
		else if (st == style::center_align)
			add_flags = SS_CENTER | SS_CENTERIMAGE;
		else if (st == style::right_align)
			add_flags = SS_RIGHT | SS_CENTERIMAGE;

		win32::modify_window_style( win32_handle(), rem_flags, add_flags );
		update();
	}

	void label::set_border(bool value)
	{
		win32::modify_window_style( win32_handle(), !value * SS_SUNKEN, value * SS_SUNKEN );
		win32::modify_window_ex_style( win32_handle(), !value * WS_EX_STATICEDGE, value * WS_EX_STATICEDGE );
		update();
	}
}