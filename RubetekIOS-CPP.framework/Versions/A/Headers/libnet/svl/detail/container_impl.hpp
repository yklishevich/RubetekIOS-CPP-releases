
namespace svl {
namespace detail
{
	void enumerate_widgets(window& wnd, enumerate_fn fn, void* ptr)
	{
		struct enumerator
		{
			enumerate_fn fn_;
			void*        ptr_;

			enumerator(enumerate_fn fn, void* ptr) : fn_(fn), ptr_(ptr) {}

			static BOOL CALLBACK proc(HWND hwnd, LPARAM lparam)
			{
				enumerator& self = *reinterpret_cast<enumerator*>(lparam);
				widget* w_ptr = win32::get_wnd_ptr<widget>( hwnd );
				if (w_ptr != 0)
					self.fn_( self.ptr_, *w_ptr );
				return TRUE;
			}
		};

		enumerator e( fn, ptr );
		::EnumChildWindows( wnd.win32_handle(), &enumerator::proc, reinterpret_cast<LPARAM>(&e) );
	}

	void check_parent(window& parent, widget& w)
	{
		if ( &parent != w.get_parent() )
			throw error("specified widget has another parent");
	}


	struct remove_from_tab_order
	{
		svl::tab_order* tab_order_;

		remove_from_tab_order(svl::tab_order& tab_order)
			: tab_order_( &tab_order )
		{}

		void operator ()(widget& w)
		{
			if (control* ctrl = dynamic_cast<control*>( &w ))
			{
				tab_order_->remove( *ctrl );
			}
		}
	};


	struct remove_widget
	{
		i_container* container_;

		remove_widget(i_container& container)
			: container_( &container )
		{}

		void operator () (widget& w)
		{
			container_->remove_widget( w );
		}
	};
}}

/*----------------------------------------------------------------------*/

namespace svl
{
	z_order z_order::top()
	{
		return z_order( type::top, 0 );
	}

	z_order z_order::bottom()
	{
		return z_order( type::bottom, 0 );
	}

	z_order z_order::under(widget& w)
	{
		return z_order( type::under, &w );
	}

	z_order z_order::above(widget& w)
	{
		return z_order( type::above, &w );
	}

	z_order::type::enum_t z_order::get_type() const
	{
		return type_;
	}

	widget* z_order::get_widget() const
	{
		return widget_;
	}

	z_order::z_order(type::enum_t t, widget* w)
		: type_  ( t )
		, widget_( w )
	{}

	/*----------------------------------------------------------------------*/

	i_container::i_container()
		: layout_( 0 )
	{}

	i_container::~i_container()
	{
		delete layout_;
	}

	void i_container::add_widget(widget& w, z_order z)
	{
		window* this_wnd = do_get_window();
		window* w_parent = w.get_parent();
		if (w_parent != this_wnd)
		{
			if (w_parent != 0)
				dynamic_cast<i_container &>( *w_parent ).remove_widget( w );
			::SetParent( w.win32_handle(), this_wnd->win32_handle() );
			w.parent_ = this_wnd;
		}
		reorder( w, z );
	}

	void i_container::remove_widget(widget& w) // throw()
	{
		window* this_wnd = do_get_window();
		SVL_ASSERT( this_wnd == w.get_parent() );

		if (this_wnd != w.get_parent())
			return;

		if (form* frm = this_wnd->do_get_parent_form())
		{
			if (control* ctrl = dynamic_cast<control*>( &w ))
			{
				tab_order( *frm ).remove( *ctrl );
			}
			else if (i_container* cntnr = dynamic_cast<i_container*>( &w ))
			{
				tab_order tab_order( *frm );
				cntnr->recursive_for_each_widget( detail::remove_from_tab_order( tab_order ) );
			}
		}

		if (has_layout())
			layout().unpin_widget(w);

		::SetParent( w.win32_handle(), this_wnd->get_env().sys_window );
		w.parent_ = 0;
	}

	void i_container::remove_all_widgets() // throw()
	{
		for_each_widget( detail::remove_widget( *this ) );
	}

	void i_container::reorder(widget& w, z_order z)
	{
		window* this_wnd = do_get_window();
		detail::check_parent( *this_wnd, w );

		switch (z.get_type())
		{
		case z_order::type::top:
			win32::place_window_after( w.win32_handle(), HWND_TOP);
			break;
		case z_order::type::bottom:
			win32::place_window_after( w.win32_handle(), HWND_BOTTOM);
			break;
		case z_order::type::under:
			detail::check_parent( *this_wnd, *z.get_widget() );
			win32::place_window_after( w.win32_handle(), z.get_widget()->win32_handle() );
			break;
		case z_order::type::above: {
			detail::check_parent( *this_wnd, *z.get_widget() );
			HWND hwnd = w.win32_handle();
			HWND pos_wnd = z.get_widget()->win32_handle();
			for (;;)
			{
				pos_wnd = ::GetWindow( pos_wnd, GW_HWNDPREV );
				if (pos_wnd == 0)
				{
					win32::place_window_after( hwnd, HWND_TOP );
					return;
				}
				if (win32::get_wnd_ptr( pos_wnd ) != 0)
				{
					win32::place_window_after( hwnd, pos_wnd );
					return;
				}
			}}
		}
	}

	svl::rect i_container::client_rect() const
	{
		return do_get_client_rect();
	}

	svl::size i_container::client_size() const
	{
		return do_get_client_rect().size();
	}

	grid_layout& i_container::create_layout(unsigned rows, unsigned columns, grid_layout::profile profile)
	{
		std::auto_ptr<grid_layout> layout( new grid_layout( rows, columns, profile ));

		rect2 r = do_get_margins();
		layout->set_h_margins( r.x0, r.x1 );
		layout->set_v_margins( r.y0, r.y1 );

		delete layout_;
		layout_ = layout.release();

		return *layout_;
	}

	bool i_container::has_layout() const // throw()
	{
		return layout_ != 0;
	}

	grid_layout& i_container::layout()
	{
		if (layout_ == 0)
			throw error("container doesn't has layout");
		return *layout_;
	}

	grid_layout* i_container::get_layout() // throw()
	{
		return layout_;
	}

	void i_container::apply_layout()
	{
		resize_widgets( client_rect() );
	}

	void i_container::resize_widgets(rect const& r)
	{
		if (layout_)
			layout_->resize_widgets( r );	
	}
}
