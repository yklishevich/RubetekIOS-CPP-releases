
namespace svl
{
	widget::widget()
		: parent_( 0 )
	{}

	widget::~widget()
	{
		//start_destruction();
		SVL_ASSERT( parent_ == 0 );
	}

	void widget::start_destruction()
	{
		window::start_destruction();
		set_no_parent();
	}

	bool widget::has_parent() const // throw()
	{
		return parent_ != 0;
	}

	window& widget::parent() const
	{
		if (parent_ == 0)
			throw error("widget has no parent");

		return *parent_;
	}

	bool widget::has_parent_form() const // throw()
	{
		return do_get_parent_form() != 0;
	}

	form& widget::parent_form() const
	{
		if (form* frm = do_get_parent_form())
			return *frm;
		throw error("widget has no parent form");
	}

	window* widget::get_parent() const
	{
		return parent_;
	}

	form* widget::get_parent_form() const // throw()
	{
		return do_get_parent_form();
	}

	void widget::set_no_parent()
	{
		if (has_parent())
			dynamic_cast<i_container &>( parent() ).remove_widget( *this );
	}

	svl::rect widget::do_get_rect() const
	{
		RECT r;
		SVL_VERIFY(::GetWindowRect( win32_handle(), &r ), != 0);

		HWND pwnd = ::GetParent( win32_handle() );
		SVL_ASSERT( pwnd != 0 );

		POINT p = { r.left, r.top };
		SVL_VERIFY( (::ScreenToClient( pwnd, &p )), != 0);
		return svl::rect( p.x, p.y, r.right - r.left, r.bottom - r.top );
	}

	form* widget::do_get_parent_form() const
	{
		return parent_ == 0 ? 0 : parent_->do_get_parent_form();
	}

	/*----------------------------------------------------------------------*/

	control::control()
	{}

	control::~control()
	{
		//start_destruction();
	}

	bool control::enabled()
	{
		return ::IsWindowEnabled( win32_handle() ) != 0;
	}

	bool control::focused()
	{
		return false;
	}

	void control::enable()
	{
		::EnableWindow( win32_handle(), TRUE );
	}

	void control::enable(bool val)
	{
		::EnableWindow( win32_handle(), val ? TRUE : FALSE );
	}

	void control::disable()
	{
		::EnableWindow( win32_handle(), FALSE );
	}

	void control::focus()
	{
		::SetFocus( win32_handle() );
	}

	void control::pre_handle_event(event::processor& ep)
	{
		widget::pre_handle_event( ep );

		ep( &control::handle_get_focus );
		ep( &control::handle_lose_focus );
	}
	
	void control::handle_get_focus(event::get_focus&)
	{
		if (form* frm = get_parent_form())
		{
			if (detail::tab_order* tab_order = frm->tab_order_)
			{
				tab_order->handle_get_focus( *this );
			}
		}
	}

	void control::handle_lose_focus(event::lose_focus&)
	{
		if (form* frm = get_parent_form())
		{
			if (detail::tab_order* tab_order = frm->tab_order_)
			{
				tab_order->handle_lose_focus( *this );
			}
		}
	}

	/*----------------------------------------------------------------------*/

	container::container()
	{}

	container::~container()
	{
		// start_destruction
	}

	void container::start_destruction() // throw()
	{
		widget::start_destruction();
		remove_all_widgets();
	}

	void container::enable()
	{
		::EnableWindow( win32_handle(), TRUE );
	}

	void container::disable()
	{
		::EnableWindow( win32_handle(), FALSE );
	}

	svl::rect container::do_get_client_rect() const
	{
		RECT R;
		SVL_VERIFY( (::GetClientRect( win32_handle(), &R )), != 0 );
		return win32::to_rect( R );
	}

	window* container::do_get_window()
	{
		return this;
	}

	void container::do_resize( svl::rect const& r )
	{
		SVL_VERIFY( (::SetWindowPos( win32_handle(), 0, r.x, r.y, r.dx, r.dy,
			SWP_NOACTIVATE | SWP_NOZORDER )), != 0);

		resize_widgets( svl::rect( 0, 0, r.dx, r.dy ));
	}
}