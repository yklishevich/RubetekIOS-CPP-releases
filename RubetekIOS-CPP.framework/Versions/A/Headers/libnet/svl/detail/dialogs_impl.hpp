
namespace svl {
namespace detail
{
	char const* const select_path_text = "Укажите необходимую папку, а затем нажмите кнопку \"OK\"";
	unsigned const path_size = 4096;

	void parse_filter(str_ref filter, tstring& filter_str)
	{
		unsigned semicolons = 0;
		unsigned open_quotes = 0;
		tchar const* data = filter.data();
		tstring mask;

		for (std::size_t i = 0, count = filter.size(); i < count; i++)
		{
			tchar ch = data[i];
			if (ch == SVL_T(';'))
			{
				if (++semicolons == 2)
				{
					semicolons = 0;
					open_quotes = 0;
					(*--filter_str.end()) = 0;
					filter_str += mask;
					filter_str.push_back( 0 );
					mask.clear();
					continue;
				}
			}
			else if (ch == SVL_T('('))
			{
				open_quotes++;
			}
			else if (ch == SVL_T(')') && open_quotes != 0)
			{
				open_quotes--;
			}
			else if (open_quotes != 0)
			{
				if (semicolons == 1)
					mask.push_back( SVL_T(';') );
				if (ch == SVL_T(' '))
					mask.push_back( SVL_T(';') );
				else
					mask.push_back( ch );
			}

			filter_str.push_back( ch );

			if (ch != SVL_T(';') && semicolons == 1)
				semicolons = 0;
		}

		if (!mask.empty())
		{
			tchar& back_ch = (*--filter_str.end());
			if (back_ch == SVL_T(';'))
				back_ch = 0;
			else
				filter_str.push_back( 0 );
			filter_str += mask;
			filter_str.push_back( 0 );
		}
		filter_str.push_back( 0 );
	}

	void set_filter(str_ref filter, tstring& filter_str)
	{
		if (filter.size() != 0)
			parse_filter( filter, filter_str );
		else
			parse_filter( "Все файлы (*.*)", filter_str );
	}

	void normalize_path_name(str_ref initial_path, tstring& init_path)
	{
		init_path.assign(initial_path.data(), initial_path.size());
		std::replace_if(init_path.begin(), init_path.end(),
			std::bind1st(std::equal_to<tchar>(), SVL_T('/')), SVL_T('\\') );
	}
}}

/*----------------------------------------------------------------------*/

namespace svl
{
	unsigned desktop_t::displays_count()
	{
		return static_cast<unsigned>(::GetSystemMetrics(SM_CMONITORS));
	}

	::svl::rect desktop_t::display_rect(unsigned display_no)
	{
		struct monitor_infos
		{
			monitor_infos()
			{
				SVL_VERIFY(::EnumDisplayMonitors(0, 0, &monitor_infos::proc, reinterpret_cast<LPARAM>(this)), != 0);
			}

			static BOOL CALLBACK proc(HMONITOR hMonitor, HDC /*hdcMonitor*/, LPRECT lprcMonitor, LPARAM dwData)
			{
				monitor_infos& self = *reinterpret_cast<monitor_infos*>(dwData);
				info inf = { hMonitor, *lprcMonitor };
				self.data.push_back(inf);
				return TRUE;
			}

			struct info
			{
				HMONITOR handle;
				RECT     rect;
			};
			std::vector<info> data;
		};

		monitor_infos mi;

		if (mi.data.size() <= display_no)
			throw svl::error("неверное значение номера дисплея");

		return win32::to_rect(mi.data[display_no].rect);
	}

	::svl::rect desktop_t::pri_display_rect()
	{
		POINT p = { 0, 0 };
		HMONITOR h = ::MonitorFromPoint(p, MONITOR_DEFAULTTOPRIMARY);
		MONITORINFO i;
		i.cbSize = sizeof(i);
		SVL_MS(GetMonitorInfo)(h, &i);
		return win32::to_rect(i.rcMonitor);
	}

	::svl::rect desktop_t::pri_display_work_rect()
	{
		POINT p = { 0, 0 };
		HMONITOR h = ::MonitorFromPoint(p, MONITOR_DEFAULTTOPRIMARY);
		MONITORINFO i;
		i.cbSize = sizeof(i);
		SVL_MS(GetMonitorInfo)(h, &i);
		return win32::to_rect(i.rcWork);
	}

	::svl::rect desktop_t::rect()
	{
		int x = ::GetSystemMetrics(SM_XVIRTUALSCREEN);
		int y = ::GetSystemMetrics(SM_YVIRTUALSCREEN);
		int w = ::GetSystemMetrics(SM_CXVIRTUALSCREEN);
		int h = ::GetSystemMetrics(SM_CYVIRTUALSCREEN);
		return ::svl::rect(x, y, w, h);
	}

	/*----------------------------------------------------------------------*/

	selected_file::operator safe_bool() const
	{
		return safe_bool_value( file_name );
	}

	selected_files::operator safe_bool() const
	{
		return safe_bool_value( !file_names.empty() );
	}

	/*----------------------------------------------------------------------*/

	void show_error(form& frm, str_ref text, str_ref caption)
	{
		SVL_MS(MessageBox)( frm.win32_handle(), text.data(), caption.data(), MB_OK | MB_ICONERROR );
	}

	void show_error(desktop_t, str_ref text, str_ref caption)
	{
		SVL_MS(MessageBox)( 0, text.data(), caption.data(), MB_OK | MB_ICONERROR );
	}

	/*----------------------------------------------------------------------*/

	void show_warning(form& frm, str_ref text, str_ref caption)
	{
		SVL_MS(MessageBox)( frm.win32_handle(), text.data(), caption.data(), MB_OK | MB_ICONWARNING );
	}

	void show_warning(desktop_t, str_ref text, str_ref caption)
	{
		SVL_MS(MessageBox)( 0, text.data(), caption.data(), MB_OK | MB_ICONWARNING );
	}

	/*----------------------------------------------------------------------*/

	void show_info(form& frm, str_ref text, str_ref caption)
	{
		SVL_MS(MessageBox)( frm.win32_handle(), text.data(), caption.data(), MB_OK | MB_ICONINFORMATION );
	}

	void show_info(desktop_t, str_ref text, str_ref caption)
	{
		SVL_MS(MessageBox)( 0, text.data(), caption.data(), MB_OK | MB_ICONINFORMATION );
	}

	/*----------------------------------------------------------------------*/

	buttons::_ ask_yes_no_question(form& frm, str_ref text, str_ref caption)
	{
		return SVL_MS(MessageBox)( frm.win32_handle(), text.data(), caption.data(), MB_YESNO | MB_ICONQUESTION ) == IDYES ? buttons::yes : buttons::no;
	}

	buttons::_ ask_yes_no_question(desktop_t, str_ref text, str_ref caption)
	{
		return SVL_MS(MessageBox)( 0, text.data(), caption.data(), MB_YESNO | MB_ICONQUESTION ) == IDYES ? buttons::yes : buttons::no;
	}

	buttons::_ ask_yes_no_cancel_question(form& frm, str_ref text, str_ref caption)
	{
		int r = SVL_MS(MessageBox)( frm.win32_handle(), text.data(), caption.data(), MB_YESNO | MB_ICONQUESTION );
		if ( r == IDYES )
			return buttons::yes;
		else if ( r == IDNO )
			return buttons::no;
		return buttons::cancel;
	}

	buttons::_ ask_yes_no_cancel_question(desktop_t, str_ref text, str_ref caption)
	{
		int r = SVL_MS(MessageBox)( 0, text.data(), caption.data(), MB_YESNO | MB_ICONQUESTION );
		if ( r == IDYES )
			return buttons::yes;
		else if ( r == IDNO )
			return buttons::no;
		return buttons::cancel;
	}

	/*----------------------------------------------------------------------*/

	string select_path(form& frm, str_ref initial_path, str_ref text)
	{
		struct browse { static int CALLBACK proc(HWND hwnd, UINT uMsg, LPARAM, LPARAM lpData)
		{
			if (uMsg == BFFM_INITIALIZED)
				SVL_MS(SendMessage)( hwnd, BFFM_SETSELECTION, TRUE, lpData );
			return 0;
		}};

		tstring init_path;
		detail::normalize_path_name(initial_path, init_path);

		detail::env::shared_instance()->com_apartment.initialize();

		string_reference select_path_text( detail::select_path_text );

		SVL_MS(BROWSEINFO) binfo;
		memset( &binfo, 0, sizeof(binfo) );
		binfo.hwndOwner      = frm.win32_handle();
		binfo.ulFlags        = BIF_NEWDIALOGSTYLE | BIF_RETURNONLYFSDIRS; // | BIF_NONEWFOLDERBUTTON
		binfo.pszDisplayName = 0;
		binfo.lpszTitle      = text.size() == 0 ? select_path_text.data() : text.data();

		if (!init_path.empty())
		{
			binfo.lParam = reinterpret_cast<LPARAM>( init_path.c_str() );
			binfo.lpfn   = &browse::proc;
		}

		if (LPITEMIDLIST list = SVL_MS(SHBrowseForFolder)( &binfo ))
		{
			string path_name( detail::path_size );
			SVL_VERIFY( SVL_MS(SHGetPathFromIDList)( list, path_name.data() ), != 0 );
			::CoTaskMemFree( list );
			return path_name.auto_resize();
		}
		return string( 1 );
	}

	/*----------------------------------------------------------------------*/

	selected_file get_open_file_name(form& frm, str_ref filter, unsigned filter_index, str_ref initial_path, str_ref caption)
	{
		tstring filter_str;
		detail::set_filter(filter, filter_str);

		tstring init_path;
		detail::normalize_path_name(initial_path, init_path);

		string path( detail::path_size );

		SVL_MS(OPENFILENAME) data;
		memset( &data, 0, sizeof(data) );
		data.lStructSize     = sizeof(data);
		data.hwndOwner       = frm.win32_handle();
		data.lpstrFilter     = filter_str.c_str();
		data.nFilterIndex    = filter_index + 1;
		data.lpstrFile       = path.data();
		data.nMaxFile        = detail::path_size;
		data.lpstrInitialDir = init_path.size() == 0 ? 0 : init_path.c_str();
		data.lpstrTitle      = caption.size() == 0   ? 0 : caption.data();

		if (SVL_MS(GetOpenFileName)( &data ) != 0)
		{
			selected_file result = { path.auto_resize(), data.nFilterIndex - 1 };
			return result;
		}
		else
		{
			selected_file result = { string(1), 0 };
			return result;
		}
	}

	selected_files get_open_file_names(form& frm, str_ref filter, unsigned filter_index, str_ref initial_path, str_ref caption)
	{
		tstring filter_str;
		detail::set_filter(filter, filter_str);

		tstring init_path;
		detail::normalize_path_name(initial_path, init_path);

		string path( detail::path_size * 2 );

		SVL_MS(OPENFILENAME) data;
		memset( &data, 0, sizeof(data) );
		data.lStructSize     = sizeof(data);
		data.hwndOwner       = frm.win32_handle();
		data.lpstrFilter     = filter_str.c_str();
		data.nFilterIndex    = filter_index + 1;
		data.lpstrFile       = path.data();
		data.nMaxFile        = detail::path_size;
		data.lpstrInitialDir = init_path.size() == 0 ? 0 : init_path.c_str();
		data.lpstrTitle      = caption.size() == 0   ? 0 : caption.data();
		data.Flags           = OFN_EXPLORER | OFN_ALLOWMULTISELECT;

		selected_files result;
		if (SVL_MS(GetOpenFileName)( &data ) != 0)
		{
			path.auto_resize();
			tchar* files = path.data() + path.size() + 1;

			if (*files == 0)
			{
				result.file_names.push_back( path );
			}
			else
			{
				tstring file_path( path.data(), path.size() );
				typedef std::char_traits<tchar> traits;
				while (*files != 0)
				{
					std::size_t len = traits::length( files );
					tstring file_name = file_path + SVL_T("\\") + tstring( files, len );
					string fname( file_name.size() );
					std::copy( file_name.begin(), file_name.end(), fname.data() );
					fname.resize( file_name.size() );
					result.file_names.push_back( fname );
					files += len + 1;
				}
			}
			result.filter_index = data.nFilterIndex - 1;
		}
		else
		{
			result.filter_index = 0;
		}
		return result;
	}

	selected_file get_save_file_name(form& frm, str_ref filter, unsigned filter_index, str_ref initial_path, str_ref caption, unsigned flags)
	{
		tstring filter_str;
		detail::set_filter(filter, filter_str);

		tstring init_path;
		detail::normalize_path_name(initial_path, init_path);

		string path( detail::path_size );

		SVL_MS(OPENFILENAME) data;
		memset( &data, 0, sizeof(data) );
		data.lStructSize     = sizeof(data);
		data.hwndOwner       = frm.win32_handle();
		data.lpstrFilter     = filter_str.c_str();
		data.nFilterIndex    = filter_index + 1;
		data.lpstrFile       = path.data();
		data.nMaxFile        = detail::path_size;
		data.lpstrInitialDir = init_path.size() == 0 ? 0 : init_path.c_str();
		data.lpstrTitle      = caption.size() == 0   ? 0 : caption.data();

		if (flags & dlg_option::overwrite_prompt)
			data.Flags |= OFN_OVERWRITEPROMPT;

		if (SVL_MS(GetSaveFileName)( &data ) != 0)
		{
			selected_file result = { path.auto_resize(), data.nFilterIndex - 1 };
			return result;
		}
		else
		{
			selected_file result = { string(1), 0 };
			return result;
		}
	}
}
