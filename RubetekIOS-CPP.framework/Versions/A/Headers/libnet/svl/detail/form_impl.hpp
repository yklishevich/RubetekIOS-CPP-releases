
namespace svl {
namespace detail
{
	unsigned const FormStylesCount = 5;
	struct style
	{
		unsigned value;
		unsigned ex;
	};
	const style form_styles[FormStylesCount] =
	{
		{ WS_SYSMENU, 0 },
		{ WS_MINIMIZEBOX, 0 },
		{ WS_MAXIMIZEBOX, 0 },
		{ WS_THICKFRAME, 0 },
		{ WS_EX_TOOLWINDOW, 1 }
	};
}}

/*----------------------------------------------------------------------*/

namespace svl
{
	void form::create_form()
	{
		master_ = 0;
		tab_order_ = 0;
		modal_ = false;
		close_on_escape_ = false;

		DWORD style = WS_OVERLAPPED | WS_CAPTION | WS_CLIPCHILDREN | WS_SYSMENU;
		COLORREF colour = ::GetSysColor( COLOR_BTNFACE );
		tchar const* icon_name = reinterpret_cast<tchar*>(200u);
		icon_ = static_cast<HICON>( SVL_MS(LoadImage)(win32::module_handle(), icon_name, IMAGE_ICON, 0, 0, 0) ); // если ресурс будет отсутствовать, функция вернет 0 - то что нужно.

		w32_create_form_window( style, 0, colour, icon_ );
		SVL_MS(SetWindowLongPtr)( win32_handle(), GWLP_HWNDPARENT, 0 );
	}

	form::form()
	{
		create_form();
		set_caption( app_name() );
		set_style( style::dialog );
	}

	form::form(str_ref caption, unsigned style)
	{
		create_form();
		set_caption( caption );
		set_style( style );
	}

	form::~form()
	{
		start_destruction();
		set_no_master();
		while (!slaves_.empty())
			slaves_.front()->set_no_master();
		remove_all_widgets();
		w32_destroy_window();
		if (icon_ != 0)
			SVL_VERIFY( ::DestroyIcon(icon_), != 0 );
		delete tab_order_;
	}

	string form::caption() const
	{
		string str(SVL_MS(GetWindowTextLength)(win32_handle()));
		int len = SVL_MS(GetWindowText)( win32_handle(), str.data(), static_cast<int>( str.capacity() ) );
		return str.resize(len);
	}

	void form::set_caption(str_ref str)
	{
		SVL_MS(SetWindowText)( win32_handle(), str.data() );
	}

	void form::set_back_color(color colour)
	{
		w32_set_window_bg_color( win32::rgb_from_color(colour) );
		update();
	}

	bool form::has_master() const // throw()
	{
		return master_ != 0;
	}

	form& form::master() const
	{
		if (master_ == 0)
			throw error("form has no master");
		return *master_;
	}

	form* form::get_master() const // throw()
	{
		return master_;
	}

	void form::set_master( form& frm )
	{
		if (master_ == &frm || this == &frm)
			return;

		if (master_ != 0)
			master_->slaves_.remove( this );
		master_ = 0;

		frm.slaves_.push_back( this );
		master_ = &frm;
		LONG_PTR p = reinterpret_cast<LONG_PTR>( frm.win32_handle() );
		SVL_VERIFY( SVL_MS(SetWindowLongPtr)( win32_handle(), GWLP_HWNDPARENT, p ), == 0 );
	}

	void form::set_no_master()
	{
		if (has_master())
		{
			master_->slaves_.remove( this );
			SVL_VERIFY( SVL_MS(SetWindowLongPtr)( win32_handle(), GWLP_HWNDPARENT, 0 ), != 0 );
			master_ = 0;
		}
	}

	bool form::active() const // throw()
	{
		return win32_handle() == ::GetActiveWindow();
	}

	void form::activate()
	{
		::SetActiveWindow( win32_handle() );
	}

	bool form::enabled() const // throw()
	{
		return ::IsWindowEnabled( win32_handle() ) != 0;
	}

	void form::enable()
	{
		::EnableWindow( win32_handle(), TRUE );
		std::for_each( slaves_.begin(), slaves_.end(), std::mem_fun(&form::enable) );
	}

	void form::disable()
	{
		::EnableWindow( win32_handle(), FALSE );
		std::for_each( slaves_.begin(), slaves_.end(), std::mem_fun(&form::disable) );
	}

	void form::set_style( unsigned styles )
	{
		using namespace detail;

		SVL_ARRAY(unsigned, 2, astyle) = SVL_A_O   0, 0   SVL_A_C;
		SVL_ARRAY(unsigned, 2, rstyle) = SVL_A_O   0, 0   SVL_A_C;
		for (unsigned i = 0; i < FormStylesCount; i++)
		{
			detail::style const& s = form_styles[i];
			rstyle[s.ex] |= s.value;
			if ((styles >> i) & 0x01)
				astyle[s.ex] |= s.value;
		}
		win32::modify_window_style   ( win32_handle(), rstyle[0], astyle[0] );
		win32::modify_window_ex_style( win32_handle(), rstyle[1], astyle[1] );

		close_on_escape_ = (styles & style::close_on_escape) != 0;

		update();
	}

	void form::add_style( unsigned styles )
	{
		using namespace detail;

		SVL_ARRAY(unsigned, 2, astyle) = SVL_A_O   0, 0   SVL_A_C;
		for (unsigned i = 0; i < FormStylesCount; i++)
		{
			detail::style const& s = form_styles[i];
			if ((styles >> i) & 0x01)
				astyle[s.ex] |= s.value;
		}
		win32::modify_window_style   ( win32_handle(), 0, astyle[0] );
		win32::modify_window_ex_style( win32_handle(), 0, astyle[1] );

		if (styles & style::close_on_escape)
			close_on_escape_ = true;

		update();
	}

	void form::remove_style( unsigned styles )
	{
		using namespace detail;

		SVL_ARRAY(unsigned, 2, rstyle) = SVL_A_O   0, 0   SVL_A_C;
		for (unsigned i = 0; i < FormStylesCount; i++)
		{
			detail::style const& s = form_styles[i];
			if ((styles >> i) & 0x01)
				rstyle[s.ex] |= s.value;
		}
		win32::modify_window_style   ( win32_handle(), rstyle[0], 0 );
		win32::modify_window_ex_style( win32_handle(), rstyle[1], 0 );

		if (styles & style::close_on_escape)
			close_on_escape_ = false;

		update();
	}

	bool form::modal() const // throw()
	{
		return modal_;
	}

	void form::do_modal(form& master)
	{
		struct local_t
		{
			form* master;
			form* frm;

			local_t(form& m, form& f)
                : master( &m ), frm( &f )
			{
				master->disable();
				frm->show();
				frm->set_master( *master );
				frm->enable();
				frm->activate();
			}

			~local_t()
			{
				master->enable();
				master->activate();
				frm->end_modal();
				frm->hide();
			}
		};

        local_t actions( master, *this );
		modal_ = true;
		while (modal_)
		{
			MSG msg;
			int rv = static_cast<int>(SVL_MS(GetMessage)( &msg, 0, 0, 0 ));
			if (rv <= 0)
			{
				if (rv != 0)
					throw win32_error( "GetMessage" );
				::PostQuitMessage( static_cast<int>(msg.wParam) );
				return;
			}
			::TranslateMessage( &msg );
			SVL_MS(DispatchMessage)( &msg );
		}
	}

	void form::end_modal()
	{
		modal_ = false;
	}

	event::target::connector<event::close> form::on_close()
	{
		return connection_info();
	}

	event::target::connector<event::activate> form::on_activate()
	{
		return connection_info();
	}

	window* form::do_get_window()
	{
		return this;
	}

	svl::rect form::do_get_rect() const
	{
		RECT R;
		SVL_VERIFY( (::GetWindowRect( win32_handle(), &R )), != 0 );
		return win32::to_rect( R );
	}

	svl::rect form::do_get_client_rect() const
	{
		RECT R;
		SVL_VERIFY( (::GetClientRect( win32_handle(), &R )), != 0 );
		return win32::to_rect( R );
	}

	rect2 form::do_get_margins() // override
	{
		return rect2( 7, 7, 7, 7 );
	}

	form* form::do_get_parent_form() const
	{
		return const_cast<form*>(this);
	}

	void form::focus()
	{
		::SetFocus( win32_handle() );
		if (tab_order_)
			tab_order_->handle_form_get_focus();
	}

	bool form::maximized() const
	{
		return ::IsZoomed(win32_handle()) != 0;
	}

	void form::normalize()
	{
		::ShowWindow(win32_handle(), SW_SHOWNOACTIVATE);
	}

	void form::maximize()
	{
		::ShowWindow(win32_handle(), SW_MAXIMIZE);
	}

	void form::set_def_button(i_button& btn)
	{
		if (!tab_order_)
			tab_order_ = new detail::tab_order( *this );
		tab_order_->set_def_button( btn );
	}

	void form::set_no_def_button()
	{
		if (tab_order_)
			tab_order_->set_no_def_button();
	}

	void form::pre_handle_event(event::processor& ep)
	{
		window::pre_handle_event( ep );

		ep( &form::pre_handle_close );
		ep( &form::handle_keydown );
		ep( &form::handle_resize );
		ep( &form::handle_activate );
		ep( &form::handle_lose_focus );
	}

	void form::post_handle_event(event::processor& ep)
	{
		ep( &form::post_handle_close );

		window::post_handle_event( ep );
	}

	void form::pre_handle_close(event::close& ev) // throw()
	{
		ev.set_action( has_master()
			? event::close::action::hide_form
			: event::close::action::exit_app );
	}

	void form::post_handle_close(event::close& ev) // throw()
	{
		typedef event::close::action action;

		switch ( ev.get_action() )
		{
		case action::do_nothing:
			break;

		case action::exit_app:
			::PostQuitMessage(0);
			if (modal())
				end_modal();
			break;

		case action::hide_form:
			if (modal())
				end_modal();
			else
				hide();
			break;

		default:
			SVL_ASSERT_FALSE();
		}
	}

	void form::handle_keydown(event::key_down& e) // throw()
	{
		if (tab_order_)
			tab_order_->handle_keydown( e, *this );
		if (e.win32_key() == VK_ESCAPE && close_on_escape_)
		{
			event::close ev;
			handle_event( ev );
		}
	}

	void form::handle_resize(event::resize&) // throw()
	{
		try
		{
			apply_layout();
		}
		catch (std::exception const&)
		{
			SVL_ASSERT_FALSE();
		}
	}

	void form::handle_activate(event::activate& e) // throw()
	{
		if (e.active() && tab_order_)
			tab_order_->on_activate( *this );
	}

	void form::handle_lose_focus(event::lose_focus&) // throw()
	{
		if (tab_order_)
			tab_order_->handle_form_lose_focus();
	}
}
