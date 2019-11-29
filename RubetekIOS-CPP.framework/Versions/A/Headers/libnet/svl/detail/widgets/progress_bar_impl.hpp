
namespace svl
{
	progress_bar::progress_bar()
	{
		w32_create_control( SVL_T("msctls_progress32") );
	}

	progress_bar::progress_bar( int min_pos, int max_pos, bool vertical )
	{
		w32_create_control( SVL_T("msctls_progress32") );
		set_range( min_pos, max_pos );
		set_vertical( vertical );
	}

	progress_bar::~progress_bar()
	{
		start_destruction();
		w32_destroy_control();
	}

	int progress_bar::position() const
	{
		return static_cast<int>( w32_send_msg( PBM_GETPOS ) );
	}

	int progress_bar::min_position() const
	{
		return static_cast<int>( w32_send_msg( PBM_GETRANGE, TRUE ) );
	}

	int progress_bar::max_position() const
	{
		return static_cast<int>( w32_send_msg( PBM_GETRANGE, FALSE ) );
	}

	void progress_bar::set_position( int pos )
	{
		w32_send_msg( PBM_SETPOS, pos );
	}

	void progress_bar::set_range( int min_pos, int max_pos )
	{
		w32_send_msg( PBM_SETRANGE32, min_pos, max_pos );
	}

	void progress_bar::set_vertical( bool val )
	{
		win32::modify_window_style( win32_handle(), !val * PBS_VERTICAL, val * PBS_VERTICAL );
		update();
	}

	bool progress_bar::marquee_mode_enabled() const
	{
		unsigned style = win32::get_window_style( win32_handle() );
		return (style & PBS_MARQUEE) != 0;
	}

	void progress_bar::enable_marquee_mode( bool val )
	{
		bool mode_enabled = marquee_mode_enabled();
		if (val == mode_enabled)
			return;

		if (val)
		{
			win32::modify_window_style( win32_handle(), 0, PBS_MARQUEE );
			w32_send_msg( PBM_SETMARQUEE, TRUE );
		}
		else
		{
			w32_send_msg( PBM_SETMARQUEE, FALSE );
			win32::modify_window_style( win32_handle(), PBS_MARQUEE, 0 );
		}
	}

	void progress_bar::disable_marquee_mode()
	{
		enable_marquee_mode( false );
	}
}
