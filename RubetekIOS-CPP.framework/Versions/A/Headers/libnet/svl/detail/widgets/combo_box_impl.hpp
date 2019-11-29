
namespace svl
{
	void combo_box::create_combo_box()
	{
		w32_create_control( SVL_T("COMBOBOX"), CBS_DROPDOWNLIST, WS_EX_CLIENTEDGE );
	}

	combo_box::combo_box()
	{
		create_combo_box();	
	}

	combo_box::~combo_box()
	{
		start_destruction();
		w32_destroy_control();
	}

	void combo_box::append( str_ref str )
	{
		w32_send_msg( CB_INSERTSTRING, static_cast<WPARAM>( -1 ), reinterpret_cast<LPARAM>( str.data() ) );
	}

	void combo_box::insert( str_ref str, size_t index )
	{
		w32_send_msg( CB_INSERTSTRING, index, reinterpret_cast<LPARAM>(str.data()) );
	}

	void combo_box::remove( size_t index )
	{
		SVL_VERIFY( w32_send_msg( CB_DELETESTRING, index ), != CB_ERR ); // !!! invalid index
	}

	void combo_box::clear()
	{
		w32_send_msg( CB_RESETCONTENT );
	}

	void combo_box::select( size_t index )
	{
		set_selected( index );
		event::select ev( selected() );
		handle_event( ev );
	}

	void combo_box::set_selected( size_t index )
	{
		LRESULT result = w32_send_msg( CB_SETCURSEL, index );
		unused(result);
		SVL_ASSERT( result != CB_ERR || index == no_sel );  // !!! invalid index
	}

	size_t combo_box::items_count() const
	{
		return w32_send_msg( CB_GETCOUNT );
	}

	size_t combo_box::selected() const
	{
		LRESULT result = w32_send_msg( CB_GETCURSEL );
		return result == CB_ERR ? no_sel : result;
	}

	string combo_box::selected_item() const
	{
		return item( selected() );
	}

	string combo_box::item( size_t index ) const
	{
		LRESULT result = w32_send_msg( CB_GETLBTEXTLEN, index );
		SVL_ASSERT( result != CB_ERR ); // !!! invalid index
		if (result != CB_ERR)
		{
			string str( static_cast<size_t>( result ) );
			w32_send_msg( CB_GETLBTEXT, index, reinterpret_cast<LPARAM>( str.data() ) );
			return str.resize( static_cast<std::size_t>(result) );
		}
		return string(1);
	}

	void combo_box::pre_handle_event( event::processor& ep )
	{
		control::pre_handle_event( ep );

		ep( &combo_box::handle_command );
	}

	void combo_box::handle_command( event::win32::command& e )
	{
		if (e.id() == CBN_SELCHANGE)
		{
			event::select ev( selected() );
			handle_event( ev );
		}
	}

	event::target::connector<event::select> combo_box::on_select()
	{
		return connection_info();
	}
}