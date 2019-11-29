
namespace svl
{
	list_box::list_box()
	{
		w32_create_control( SVL_T("LISTBOX"), WS_VSCROLL | LBS_NOINTEGRALHEIGHT | LBS_NOTIFY, WS_EX_CLIENTEDGE);
	}

	list_box::~list_box()
	{
		start_destruction();
		w32_destroy_control();
	}

	void list_box::append(str_ref str)
	{
		SVL_VERIFY( w32_send_msg( LB_INSERTSTRING, static_cast<WPARAM>(-1), reinterpret_cast<LPARAM>(str.data()) ), != LB_ERR );
	}

	void list_box::clear()
	{
		w32_send_msg( LB_RESETCONTENT );
	}

	std::size_t list_box::items_count() const
	{
		return w32_send_msg( LB_GETCOUNT );
	}

	string list_box::item(std::size_t index) const
	{
		std::size_t str_size = w32_send_msg(LB_GETTEXTLEN, index);
		if (str_size != unsigned(LB_ERR))
		{
			string str(str_size + 1);
			w32_send_msg(LB_GETTEXT, index, reinterpret_cast<LPARAM>(str.data()));
			return str.resize(str_size);
		}
		else
		{
			string str(1);
			return str.resize(0);
		}
	}

	void list_box::insert(std::size_t index, str_ref str)
	{
		SVL_VERIFY( w32_send_msg(LB_INSERTSTRING, index, reinterpret_cast<LPARAM>(str.data())), != LB_ERR);
	}

	void list_box::change(std::size_t index, str_ref str)
	{
		if (index > items_count())
			throw std::range_error("index out of bounds");

		insert(index, str);
		remove(index + 1);
	}

	void list_box::set_redraw_flag(bool val)
	{
		w32_send_msg( WM_SETREDRAW, val ? TRUE : FALSE );
	}

	void list_box::remove(std::size_t index)
	{
		SVL_VERIFY( w32_send_msg(LB_DELETESTRING, index), != LB_ERR);
	}

	std::size_t list_box::top_index() const
	{
		return w32_send_msg(LB_GETTOPINDEX);
	}

	void list_box::set_top_index(std::size_t index)
	{
		SVL_VERIFY( w32_send_msg(LB_SETTOPINDEX, index), != LB_ERR );
	}

	event::target::connector<event::select> list_box::on_select()
	{
		return connection_info();
	}

	void list_box::pre_handle_event( event::processor& ep )
	{
		control::pre_handle_event( ep );

		ep( &list_box::handle_command );
	}

	void list_box::handle_command( event::win32::command& e )
	{
		switch (e.id())
		{
		case LBN_SELCHANGE: {
			event::select ev( w32_send_msg( LB_GETCURSEL ) );
			handle_event(ev);
			break;
			}
		}
	}

}