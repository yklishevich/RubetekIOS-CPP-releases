
namespace svl
{
	group_box::group_box()
	{
		w32_create_control( SVL_T("BUTTON"), WS_CLIPCHILDREN | BS_GROUPBOX);
	}

	group_box::group_box(str_ref caption)
	{
		w32_create_control( SVL_T("BUTTON"), WS_CLIPCHILDREN | BS_GROUPBOX);
		set_caption( caption );
	}

	group_box::~group_box()
	{
		start_destruction();
		w32_destroy_control();
	}

	string group_box::caption() const
	{
		return win32::get_window_text( win32_handle() );
	}

	void group_box::set_caption( str_ref str )
	{
		win32::set_window_text( win32_handle(), str );
	}

	rect2 group_box::do_get_margins()
	{
		return rect2( 9, 15, 9, 9 );
	}
}