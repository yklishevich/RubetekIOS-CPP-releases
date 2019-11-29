
namespace svl
{
	push_button::push_button()
	{
		w32_create_child_window( WS_CLIPCHILDREN, 0 );
		pushed_ = false;
		mkey_down_ = false;
		auto_ = true;
	}
	
	push_button::push_button(str_ref caption, bool pushed)
	{
		w32_create_child_window( WS_CLIPCHILDREN, 0 );
		btn_.set_caption( caption );
		pushed_ = pushed;
		mkey_down_ = false;
		auto_ = true;
	}

	push_button::~push_button()
	{
		start_destruction();
		w32_destroy_window();
	}

	string push_button::caption() const
	{
		return btn_.caption();
	}

	void push_button::set_caption(str_ref str)
	{
		btn_.set_caption(str);
	}

	void push_button::set_auto_mode(bool value)
	{
		auto_ = value;
	}

	void push_button::click() // thread(any)
	{
		event::click ev;
		send_event( ev );
	}

	void push_button::push_down() 
	{
		push(true);
	}

	void push_button::push_up()
	{
		push(false);
	}

	void push_button::push(bool down)
	{
		pushed_ = down;
		update();
	}

	bool push_button::pushed() const
	{
		return pushed_;
	}

	void push_button::pre_handle_event( event::processor& ep )
	{
		control::pre_handle_event( ep );
		ep( &push_button::handle_paint );
		ep( &push_button::handle_resize );
		ep( &push_button::handle_mouse_key_down );
		ep( &push_button::handle_mouse_key_up );
	}

	void push_button::handle_paint( event::paint& )
	{
		graphics::canvas canvas(*this);
		SVL_MS(SendMessage)(btn_.win32_handle(), BM_SETSTATE, pushed_ ? TRUE : FALSE, 0);
		SVL_MS(SendMessage)(btn_.win32_handle(), WM_PRINT, reinterpret_cast<WPARAM>(canvas.win32_handle()), PRF_CLIENT | PRF_CHILDREN);
	}

	void push_button::handle_resize( event::resize& )
	{
		btn_.resize( this->size() );
		update();
	}

	void push_button::handle_mouse_key_down( event::mouse_key_down& e)
	{
		if (e.left().just_pressed())
		{
			mkey_down_ = true;
			::SetCapture(win32_handle());
		}
	}

	void push_button::handle_mouse_key_up( event::mouse_key_up& e)
	{
		if (mkey_down_)
		{
			mkey_down_ = false;
			::ReleaseCapture();

			if (e.left().just_released())
			{
				svl::rect r(point(0,0), this->size());
				if (is_inside(e.cursor(), r))
				{
					if (auto_)
					{
						pushed_ = !pushed_;
						update();
					}
					click();
				}
			}
		}
	}

	event::target::connector<event::click> push_button::on_click()
	{
		return connection_info();
	}
}