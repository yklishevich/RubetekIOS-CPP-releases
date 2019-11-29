
namespace svl {
namespace detail
{
	struct focus_drawing_activator
	{
		static void prepare(HWND form_hwnd)
		{
			HWND btn_hwnd = SVL_MS(CreateWindow)(SVL_T("BUTTON"), SVL_T(""), WS_TABSTOP | WS_CHILD, -10, -10, 1, 1, form_hwnd, 0, 0, 0);
			SVL_ASSERT(btn_hwnd != 0);
			SVL_VERIFY( ::DestroyWindow(btn_hwnd), != 0 );
		}

		static void activate(HWND form_hwnd)
		{
			HWND btn_hwnd = SVL_MS(CreateWindow)(SVL_T("BUTTON"), SVL_T(""), WS_TABSTOP | WS_CHILD | WS_VISIBLE, -10, -10, 1, 1, form_hwnd, 0, 0, 0);
			SVL_ASSERT(btn_hwnd != 0);

			MSG msg;
			memset(&msg, 0, sizeof(msg));
			msg.hwnd = form_hwnd;
			msg.message = WM_KEYDOWN;
			msg.wParam = 0x00000009;
			msg.lParam = 0x000f0001;
			SVL_VERIFY( SVL_MS(IsDialogMessage)(form_hwnd, &msg), != 0 );	

			SVL_VERIFY( ::DestroyWindow(btn_hwnd), != 0 );
		}
	};

	/*----------------------------------------------------------------------*/

	class tab_order
	{
	private:
		typedef std::vector<control*> vec_t;
		typedef vec_t::iterator       iterator;
		typedef std::size_t           size_t;

		bool      is_focus_drawing_activate_;
		vec_t     controls_;
		bool      ctrl_has_focus_;
		control*  focus_ctrl_;
		i_button* def_button_;

	private:
		iterator find(control* ctrl)
		{
			return std::find( controls_.begin(), controls_.end(), ctrl );	
		}

		static i_tab_group* group_of(control* ctrl)
		{
			i_tab_group_control* group_ctrl = dynamic_cast<i_tab_group_control*>( ctrl );
			return group_ctrl ? group_ctrl->tab_group() : 0;
		}

		control* get_focus_control(form& frm)
		{
			HWND hwnd = ::GetFocus();
			if (hwnd == 0)
				return 0;

			window* wnd = win32::get_wnd_ptr( hwnd );
			if (wnd == 0)
				return 0;

			control* ctrl = dynamic_cast<control*>( wnd );
			if (ctrl == 0)
				return 0;

			return ctrl->get_parent_form() == &frm ? ctrl : 0;
		}

		void ensure_focus_drawing(form& frm)
		{
			if (!is_focus_drawing_activate_)
			{
				control* ctrl = get_focus_control( frm );
				detail::focus_drawing_activator::activate( frm.win32_handle() );
				is_focus_drawing_activate_ = true;
				focus_ctrl_ = ctrl;
			}
		}

		bool is_tab_ready(i_tab_group* group, iterator ctrl_iter)
		{
			return (group_of( *ctrl_iter ) == group) && (*ctrl_iter)->enabled();
		}

		enum up_down_t { up, down };

		void focus_control(up_down_t val, form& frm)
		{
			if (!ctrl_has_focus_)
				return;

			i_tab_group* group = group_of( focus_ctrl_ );
			if (group == 0)
				return;

			iterator end = controls_.end();
			iterator iter = find( focus_ctrl_ );
			iterator start_iter = iter;

			do
			{
				if (val == down)
				{
					if (++iter == end)
						iter = controls_.begin();
				}
				else
				{
					if (iter == controls_.begin())
						iter = end;
					--iter;
				}
				if (iter == start_iter)
					return;
			} while (!is_tab_ready(group, iter));

			ensure_focus_drawing( frm );
			(*iter)->focus();
		}

		bool is_tab_ready(iterator ctrl_iter)
		{
			if (!(*ctrl_iter)->enabled() || !(*ctrl_iter)->visible())
				return false;

			i_tab_group* group = group_of( *ctrl_iter );
			if (group == 0)
				return true;

			if (group->active_control_ == *ctrl_iter)
				return true;

			if (find(group->active_control_) == controls_.end())
			{
				group->active_control_ = *ctrl_iter;
				return true;
			}

			return false;
		}

		enum next_prev_t { next, prev };

		void focus_control(next_prev_t val, form& frm)
		{
			iterator end = controls_.end();
			iterator iter = find( focus_ctrl_ );

			if (iter == end || !ctrl_has_focus_)
			{
				if (iter == end)
					iter = controls_.begin();
				if (is_tab_ready(iter))
				{
					(*iter)->focus();
					return;
				}
			}

			iterator start_iter = iter;
			do
			{
				if (val == next)
				{
					if (++iter == end)
						iter = controls_.begin();
				}
				else
				{
					if (iter == controls_.begin())
						iter = end;
					--iter;
				}
				if (start_iter == iter)
				{
					frm.focus();
					return;
				}
	
			} while (!is_tab_ready(iter));

			ensure_focus_drawing( frm );
			(*iter)->focus();
		}
		
	public:
		tab_order(form& frm)
			: is_focus_drawing_activate_( false )
			, ctrl_has_focus_( false )
			, focus_ctrl_( 0 )
			, def_button_( 0 )
		{
			focus_drawing_activator::prepare( frm.win32_handle() );
		}

		~tab_order()
		{
			SVL_ASSERT( controls_.empty() );
		}

		void handle_keydown(event::key_down& e, form& frm)
		{
			if (controls_.empty())
				return;

			switch (e.win32_key())
			{
			case VK_TAB: {
				bool shift_down = ::GetKeyState( VK_SHIFT ) < 0;
				focus_control( shift_down ? prev : next, frm );
				break;
				}
			case VK_UP:
				focus_control( up, frm );
				break;

			case VK_DOWN:
				focus_control( down, frm );
				break;

			case VK_RETURN: {
				if (i_button* btn = dynamic_cast<i_button*>( focus_ctrl_ ))
					btn->click();
				else if (def_button_)
					def_button_->click();
				break;
				}
			}
		}

		void handle_form_get_focus()
		{
			if (def_button_)
				def_button_->set_def_style( true );
		}

		void handle_form_lose_focus()
		{
			if (def_button_)
				def_button_->set_def_style( false );
		}

		void handle_get_focus(control& ctrl)
		{
			focus_ctrl_ = &ctrl;
			ctrl_has_focus_ = true;

			if (i_tab_group* group = group_of( &ctrl ))
				group->active_control_ = &ctrl;

			if (i_button* btn = dynamic_cast<i_button*>( &ctrl ))
			{
				btn->set_def_style( true );
			}
			else if (def_button_)
			{
				def_button_->set_def_style( true );
			}
		}

		void handle_lose_focus(control& ctrl)
		{
			if (&ctrl == focus_ctrl_)
				ctrl_has_focus_ = false;

			if (i_button* btn = dynamic_cast<i_button*>( &ctrl ))
				btn->set_def_style( false );

			if (def_button_)
				def_button_->set_def_style( false );
		}

		std::size_t control_position(control& ctrl)
		{
			iterator iter = find( &ctrl );
			SVL_ASSERT( iter != controls_.end() );
			return std::distance( controls_.begin(), iter );
		}

		std::size_t insert_control(std::size_t pos, control& ctrl)
		{
			iterator iter = find( &ctrl );
			if (iter == controls_.end())
			{
				SVL_ASSERT( pos <= controls_.size() );
				controls_.insert( controls_.begin() + pos, &ctrl );
				return pos;
			}

			std::size_t ctrl_pos = std::distance( controls_.begin(), iter );

			if (ctrl_pos < pos)
			{
				std::copy( controls_.begin() + ctrl_pos + 1, controls_.begin() + pos, controls_.begin() + ctrl_pos);
				controls_[pos - 1] = &ctrl;
				return pos - 1;
			}

			if (pos < ctrl_pos)
			{
				std::copy_backward( controls_.begin() + pos, controls_.begin() + ctrl_pos, controls_.begin() + ctrl_pos + 1);
				controls_[pos] = &ctrl;
				return pos;
			}

			return pos; // элемент управления на своем месте
		}

		std::size_t remove_control(control& ctrl) // throw()
		{
			if (focus_ctrl_ == &ctrl)
			{
				focus_ctrl_ = 0;
				ctrl_has_focus_ = false;
				if (form* frm = ctrl.get_parent_form())
					frm->focus();
			}

			if (dynamic_cast<i_button*>(&ctrl) == def_button_)
				def_button_ = 0;

			iterator iter = find( &ctrl );
			if (iter != controls_.end())
			{
				std::size_t ctrl_pos = std::distance( controls_.begin(), iter );
				controls_.erase( iter );
				return ctrl_pos;
			}
			return controls_.size();
		}

		void remove_all_controls() // throw()
		{
			controls_.clear();
			focus_ctrl_ = 0;
			ctrl_has_focus_ = false;
		}

		std::size_t controls_count()
		{
			return controls_.size();
		}

		void on_activate( form& frm )
		{
			if (focus_ctrl_ != 0)
				focus_ctrl_->focus();
			else
				frm.focus();
		}

		void set_def_button(i_button& btn)
		{
			if (def_button_ != &btn)
			{
				i_button* focus_btn = dynamic_cast<i_button*>( focus_ctrl_ );

				if (def_button_ && def_button_ != focus_btn)
					def_button_->set_def_style( false );

				def_button_ = &btn;
				if (focus_btn == 0)
					def_button_->set_def_style( true );
			}
		}

		void set_no_def_button()
		{
			if (def_button_)
			{
				def_button_->set_def_style( false );
				def_button_ = 0;
			}
		}
	};
}}

/*----------------------------------------------------------------------*/

namespace svl
{
	tab_order::tab_order(form& frm)
		: form_( frm )
		, pos_ ( 0 )
	{
		if (frm.tab_order_ != 0)
			pos_ = frm.tab_order_->controls_count();
	}

	tab_order& tab_order::add_control(control& ctrl)
	{
		return *this % ctrl;
	}

	tab_order& tab_order::operator % (control& ctrl)
	{
		if (ctrl.get_parent_form() != &form_)
			throw error("parent form for this control isn't same that tab orders's form");

		if (form_.tab_order_ == 0)
			form_.tab_order_ = new detail::tab_order( form_ );

		pos_ = form_.tab_order_->insert_control(pos_, ctrl);
		pos_++;
		return *this;
	}

	tab_order& tab_order::insert_front()
	{
		pos_ = 0;
		return *this;
	}

	tab_order& tab_order::insert_back()
	{
		if ( form_.tab_order_ != 0 )
			pos_ = form_.tab_order_->controls_count();
		return *this;
	}

	tab_order& tab_order::insert_before(control& ctrl)
	{
		SVL_ASSERT( form_.tab_order_ != 0 );

		if ( form_.tab_order_ != 0 )
		{
			pos_ = form_.tab_order_->control_position( ctrl );
		}

		return *this;
	}

	tab_order& tab_order::insert_after(control& ctrl)
	{
		SVL_ASSERT( form_.tab_order_ != 0 );

		if ( form_.tab_order_ != 0 )
		{
			pos_ = form_.tab_order_->control_position( ctrl );
			if (pos_ != form_.tab_order_->controls_count())
				pos_++;
		}

		return *this;
	}

	tab_order& tab_order::remove(control& ctrl) // throw()
	{
		if ( form_.tab_order_ != 0 )
		{
			std::size_t remove_pos = form_.tab_order_->remove_control( ctrl );
			if (remove_pos <= pos_)
			{
				if (pos_ != 0)
					pos_--;
			}
		}

		return *this;
	}
	
	tab_order& tab_order::clear() // throw()
	{
		if ( form_.tab_order_ != 0 )
		{
			form_.tab_order_->remove_all_controls();
			pos_ = 0;
		}
		return *this;
	}
}
