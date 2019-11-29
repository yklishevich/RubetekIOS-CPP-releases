
namespace svl
{
	radio_button::radio_button()
		: radio_group_( 0 )
	{
		w32_create_control( SVL_T("BUTTON"), BS_RADIOBUTTON );
	}

	radio_button::radio_button(str_ref caption, bool selected, bool multiline)
		: radio_group_( 0 )
	{
		w32_create_control( SVL_T("BUTTON"), BS_RADIOBUTTON );
		set_caption( caption );
		if (selected)
			set_selected();
		if (multiline)
			set_multiline();
	}

	radio_button::~radio_button()
	{
		start_destruction();
		if (radio_group_)
			radio_group_->remove( *this );
		w32_destroy_control();
	}

	string radio_button::caption()
	{
		return win32::get_window_text( win32_handle() );
	}

	void radio_button::set_caption( str_ref str )
	{
		win32::set_window_text( win32_handle(), str );
	}

	bool radio_button::selected()
	{
		return w32_send_msg( BM_GETCHECK ) == BST_CHECKED;
	}

	void radio_button::select()
	{
		w32_send_msg( BM_SETCHECK, BST_CHECKED );
		if (radio_group_)
		{
			radio_group_->notify_n_send_event( *this, true );
		}
		else
		{
			event::select ev( 0 );
			handle_event( ev );
		}
	}

	void radio_button::deselect()
	{
		w32_send_msg( BM_SETCHECK, BST_UNCHECKED );
		if (radio_group_)
			radio_group_->notify_n_send_event( *this, false );
	}

	void radio_button::set_selected( bool sel )
	{
		w32_send_msg( BM_SETCHECK, sel ? BST_CHECKED : BST_UNCHECKED );
		if (radio_group_)
			radio_group_->notify( *this, sel );
	}

	void radio_button::set_multiline( bool m )
	{
		win32::modify_window_style( win32_handle(), !m * BS_MULTILINE, m * BS_MULTILINE );
		update();
	}

	i_tab_group* radio_button::tab_group()
	{
		return radio_group_;
	}

	void radio_button::pre_handle_event(event::processor& ep)
	{
		control::pre_handle_event( ep );

		ep( &radio_button::handle_command );
	}

	void radio_button::handle_command(event::win32::command& e)
	{
		if (e.id() == BN_CLICKED)
			select();
	}

	event::target::connector<event::select> radio_button::on_select()
	{
		return connection_info();
	}

	/*----------------------------------------------------------------------*/

	radio_group::radio_group()
		: selected_( no_sel )
	{}

	radio_group::~radio_group()
	{
		start_destruction();
		clear();
	}

	size_t radio_group::selected()
	{
		return selected_;
	}

	void radio_group::select( size_t sel )
	{
		if (sel == no_sel)
			radios_[selected_]->deselect();
		else
			radios_[sel]->select();
	}

	void radio_group::set_selected( size_t sel )
	{
		if (sel == no_sel)
			radios_[selected_]->set_selected( false );
		else
			radios_[sel]->set_selected( true );
	}

	size_t radio_group::notify(radio_button& radio, bool selected)
	{
		SVL_ASSERT(radio.radio_group_ == this);
		vec_t::iterator iter = std::find( radios_.begin(), radios_.end(), &radio );
		SVL_ASSERT( iter != radios_.end() );
		size_t index = static_cast<size_t>( std::distance( radios_.begin(), iter ) );

		if (selected)
		{
			if (index != selected_)
			{
				if (selected_ != no_sel)
					radios_[selected_]->set_selected( false );
				selected_ = index;
				active_control_ = radios_[selected_];
			}
		}
		else
		{
			if (selected_ == index)
			{
				selected_ = no_sel;
				active_control_ = 0;
			}
		}

		return index;
	}

	void radio_group::notify_n_send_event(radio_button& radio, bool selected)
	{
		size_t index = notify( radio, selected );

		event::select ev( selected ? index : no_sel );
		if (index != no_sel)
			radio.handle_event( ev );
		handle_event( ev );
	}

	void radio_group::append( radio_button& radio )
	{
		bool is_sel = radio.selected();
		if (is_sel && selected_ != no_sel)
			radios_[selected_]->set_selected( false );

		vec_t::iterator iter = std::find( radios_.begin(), radios_.end(), &radio );
		if (iter == radios_.end())
		{
			if ( radio.radio_group_ != 0 )
				radio.radio_group_->remove( radio );
			SVL_ASSERT(radio.radio_group_ == 0);
			radios_.push_back( &radio );
			radio.radio_group_ = this;
		}
		else
		{
			std::copy( iter + 1, radios_.end(), iter );
			radios_.back() = &radio;
		}

		if (is_sel)
			radio.set_selected( true );
	}

	radio_group& radio_group::operator % ( radio_button& radio )
	{
		append( radio );
		return *this;
	}

	void radio_group::remove( radio_button& radio )
	{
		vec_t::iterator iter = std::find( radios_.begin(), radios_.end(), &radio );
		SVL_ASSERT( iter != radios_.end() );

		if (iter != radios_.end())
		{
			unsigned index = static_cast<unsigned>( std::distance( radios_.begin(), iter ) );
			radios_.erase( iter );

			if (index == selected_)
			{
				selected_ = no_sel;
				active_control_ = 0;
			}
			else if (index < selected_ && selected_ != no_sel)
				selected_--;

			radio.radio_group_ = 0;
		}
	}

	void radio_group::clear() // throw()
	{
		std::size_t count = radios_.size();
		for (unsigned i = 0; i < count; i++)
			radios_[i]->radio_group_ = 0;
		selected_ = no_sel;
		active_control_ = 0;
	}

	event::target::connector<event::select> radio_group::on_select()
	{
		return connection_info();
	}
}
