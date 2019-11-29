
#ifdef _MSC_VER
  #pragma comment(lib, "comctl32.lib")
#endif

namespace svl {
namespace win32
{
	POINT from_point(point const& p )
	{
		POINT P = { p.x, p.y };
		return P;
	}

	point to_point( POINT const& P )
	{
		return point(P.x, P.y);
	}

	SIZE from_size( size const& s )
	{
		SIZE S = { s.dx, s.dy };
		return S;
	}

	size to_size( SIZE const& S )
	{
		return size( S.cx, S.cy );
	}

	RECT from_rect( rect const& r )
	{
		RECT R = { r.x, r.y, r.x + r.dx, r.y + r.dy };
		return R;
	}

	rect to_rect( RECT const& R )
	{
		return rect( R.left, R.top, R.right - R.left, R.bottom - R.top );
	}

	RECT from_rect2( rect2 const& r )
	{
		RECT R = { r.x0, r.y0, r.x1, r.y1 };
		return R;
	}

	rect2 to_rect2( RECT const& R )
	{
		return rect2( R.left, R.top, R.right, R.bottom );
	}

	COLORREF rgb_from_color( color const& c )
	{
		return +c.r | (+c.g << 8) | (+c.b << 16);
	}

	color rgb_to_color( COLORREF C )
	{
		byte r = static_cast<byte>(C & 0xFF);
		byte g = static_cast<byte>((C >> 8) & 0xFF);
		byte b = static_cast<byte>((C >> 16) & 0xFF);
		return color( r, g, b );
	}

	/*----------------------------------------------------------------------*/

	template <typename GdiObjectHandle>
	gdi_object<GdiObjectHandle>::gdi_object(GdiObjectHandle h)
		: handle_( h )
	{
		try
		{
			refs_ = new long(1);
		}
		catch (...)
		{
			SVL_VERIFY( ::DeleteObject(handle_), != 0 );
			throw;
		}
	}

	template <typename GdiObjectHandle>
	gdi_object<GdiObjectHandle>::gdi_object(GdiObjectHandle h, system_object_t)
		: handle_ ( h )
		, refs_   ( 0 )
	{}

	template <typename GdiObjectHandle>
	gdi_object<GdiObjectHandle>::gdi_object(gdi_object const& other)
		: handle_( other.handle_ )
		, refs_  ( other.refs_ )
	{
		if (refs_ != 0)
			::InterlockedIncrement( refs_ );
	}

	template <typename GdiObjectHandle>
	gdi_object<GdiObjectHandle>& gdi_object<GdiObjectHandle>::operator = (gdi_object const& other)
	{
		if (this != &other)
		{
			if (refs_ != 0 && ::InterlockedDecrement( refs_ ) == 0)
			{
				delete refs_;
				SVL_VERIFY( ::DeleteObject(handle_), != 0 );
			}
			handle_ = other.handle_;
			refs_   = other.refs_;
			::InterlockedIncrement( refs_ );
		}
		return *this;
	}

	template <typename GdiObjectHandle>
	gdi_object<GdiObjectHandle>::~gdi_object()
	{
		if (refs_ != 0 && ::InterlockedDecrement( refs_ ) == 0)
		{
			delete refs_;
			SVL_VERIFY( ::DeleteObject(handle_), != 0 );
		}
	}

	template <typename GdiObjectHandle>
	gdi_object<GdiObjectHandle>::operator GdiObjectHandle() const // throw()
	{
		return handle_;
	}

	template <typename GdiObjectHandle>
	void gdi_object<GdiObjectHandle>::swap(gdi_object& other) // throw()
	{
		using std::swap;
		swap( handle_, other.handle_ );
		swap( refs_,   other.refs_   );
	}
}}

/*----------------------------------------------------------------------*/

namespace svl {
namespace win32 {
namespace
{
	typedef LRESULT (CALLBACK* window_proc_t)(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	HINSTANCE module_handle()
	{
		return SVL_MS(GetModuleHandle)(0);
	}

	ATOM register_class(tchar const* class_name, window_proc_t proc, DWORD style, HICON icon, HBRUSH bg_brush)
	{
		SVL_MS(WNDCLASSEX) data;
		data.cbSize        = sizeof(data);
		data.style         = style;
		data.lpfnWndProc   = proc;
		data.cbClsExtra    = 0;
		data.cbWndExtra    = 0;
		data.hInstance     = module_handle();
		data.hIcon         = icon;
		data.hCursor       = LoadCursor(0, IDC_ARROW);
		data.hbrBackground = bg_brush;
		data.lpszMenuName  = 0;
		data.lpszClassName = class_name;
		data.hIconSm       = 0;

		ATOM result = SVL_MS(RegisterClassEx)(&data);
		if (result == 0)
			throw win32_error("RegisterClassEx");

		return result;
	}

	void unregister_class(ATOM wnd_class) // throw()
	{
		SVL_VERIFY( (SVL_MS(UnregisterClass)(
				reinterpret_cast<tchar*>(static_cast<std::uintptr_t>(wnd_class)),
				module_handle())), != 0 );
	}

	HWND create_window(tchar const* class_name, HWND parent, DWORD style, DWORD ex_style = 0)
	{
		HWND handle = SVL_MS(CreateWindowEx)(ex_style, class_name, SVL_T(""), style,
			CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, parent, 0,
			module_handle(), 0);
		if (handle == 0)
			throw win32_error("CreateWindowEx");
		return handle;
	}

	void destroy_window(HWND handle) // throw()
	{
		SVL_VERIFY(DestroyWindow(handle), != 0);
	}

	HBRUSH create_solid_brush(COLORREF color)
	{
		HBRUSH br = ::CreateSolidBrush(color);
		if (br == 0)
			throw win32_error("CreateSolidBrush");
		return br;
	}

	template <typename HTYPE> HTYPE get_stock_object(int type)
	{
		HGDIOBJ handle = ::GetStockObject( type );
		if (handle == 0)
			throw win32_error("GetStockObject");
		return reinterpret_cast<HTYPE>( handle );
	}

	HPEN create_pen(int style, int width, COLORREF c)
	{
		HPEN hpen = ::CreatePen( style, width, c );
		if (hpen == 0)
			throw win32_error("CreatePen");
		return hpen;
	}

	HGDIOBJ select_object(HDC hdc, HGDIOBJ gdi_obj)
	{
		HGDIOBJ prev = ::SelectObject( hdc, gdi_obj );
		if (prev == 0)
			throw win32_error( "SelectObject" );
		return prev;
	}

	template <typename T> HGDIOBJ select_object(HDC hdc, HGDIOBJ gdi_obj);

	template<> HGDIOBJ select_object<std::nothrow_t>(HDC hdc, HGDIOBJ gdi_obj)
	{
		HGDIOBJ hgdiobj = ::SelectObject( hdc, gdi_obj );
		SVL_ASSERT( hgdiobj != 0 );
		return hgdiobj;
	}

	void delete_object(HGDIOBJ obj) // throw()
	{
		SVL_VERIFY( ::DeleteObject(obj), != 0 );
	}

	template <typename T> void set_prop(HWND hwnd, tchar const* name, T* ptr)
	{
		BOOL result = SVL_MS(SetProp)( hwnd, name, reinterpret_cast<HANDLE>(ptr) );
		if (!result)
			throw win32_error( "SetProp" );
	}

	template <typename T> T* get_prop(HWND hwnd, tchar const* name) // throw()
	{
		HANDLE h = SVL_MS(GetProp)( hwnd, name );
		return reinterpret_cast<T*>( h );
	}

	void remove_prop(HWND hwnd, tchar const* name) // throw()
	{
		SVL_VERIFY( (SVL_MS(RemoveProp)( hwnd, name )), != 0 );
	}

	tchar const* const WndPropName = SVL_T("SVL_W");

	void set_wnd_ptr(HWND hwnd, window* wnd)
	{
		SVL_ASSERT( wnd != 0 );
		set_prop( hwnd, WndPropName, wnd );
	}

	window* get_wnd_ptr(HWND hwnd) // throw()
	{
		return get_prop<window>( hwnd, WndPropName );
	}

	template <class T> T* get_wnd_ptr(HWND hwnd) // throw()
	{
		window* wnd = get_wnd_ptr( hwnd );
		if (wnd == 0)
			return 0;
		return &SVL_CAST( T&, *wnd );
	}

	void remove_wnd_ptr(HWND hwnd) // throw()
	{
		remove_prop( hwnd, WndPropName );
	}

	WNDPROC set_wnd_proc(HWND hwnd, WNDPROC wnd_proc) // throw()
	{
		return reinterpret_cast<WNDPROC>(SVL_MS(SetWindowLongPtr)( hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(wnd_proc) ));
	}

	DWORD get_window_style(HWND hwnd)
	{
		return static_cast<DWORD>( SVL_MS(GetWindowLongPtr)( hwnd, GWL_STYLE ) );
	}

	void set_window_style(HWND hwnd, DWORD style)
	{
		SVL_MS(SetWindowLongPtr)( hwnd, GWL_STYLE, style );
	}

	void modify_window_style(HWND hwnd, DWORD remove_value, DWORD add_value)
	{
		if (remove_value == 0 && add_value == 0)
			return;

		DWORD style = get_window_style( hwnd );
		style = (style & ~remove_value) | add_value;
		set_window_style( hwnd, style );
	}

	DWORD get_window_ex_style(HWND hwnd)
	{
		return static_cast<DWORD>( SVL_MS(GetWindowLongPtr)( hwnd, GWL_EXSTYLE ) );
	}

	void set_window_ex_style(HWND hwnd, DWORD style)
	{
		SVL_MS(SetWindowLongPtr)( hwnd, GWL_EXSTYLE, style );
	}

	void modify_window_ex_style(HWND hwnd, DWORD remove_value, DWORD add_value)
	{
		if (remove_value == 0 && add_value == 0)
			return;

		DWORD style = get_window_ex_style( hwnd );
		style = (style & ~remove_value) | add_value;
		set_window_ex_style( hwnd, style );
	}

	void place_window_after(HWND wnd, HWND after) // throw()
	{
		if (wnd != after)
			SVL_VERIFY( (::SetWindowPos(wnd, after, 0, 0, 0, 0, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE)), != 0 );
	}

	HBITMAP create_compatible_bitmap( HDC hdc, int w, int h )
	{
		HBITMAP hbitmap = ::CreateCompatibleBitmap( hdc, w, h );
		if (hbitmap == 0)
			throw win32_error("CreateCompatibleDC");
		return hbitmap;
	}

	/*----------------------------------------------------------------------*/

	string get_window_text( HWND wnd )
	{
		string str(SVL_MS(GetWindowTextLength)(wnd));
		int len = SVL_MS(GetWindowText)( wnd, str.data(), static_cast<int>( str.capacity() ) );
		return str.resize(len);
	}

	void set_window_text( HWND wnd, str_ref str )
	{
		SVL_MS(SetWindowText)( wnd, str.data() );
	}

	/*----------------------------------------------------------------------*/

	class dc
	{
	private:
		HWND window_;
		HDC  handle_;

	public:
		static HDC get( HWND wnd )
		{
			HDC handle = ::GetDC( wnd );
			if (handle == 0)
				throw win32_error("GetDC");
			return handle;
		}

		static void release( HWND wnd, HDC hdc )
		{
			SVL_VERIFY( ::ReleaseDC( wnd, hdc ), == 1 );
		}

		dc(HWND wnd)
			: window_( wnd )
			, handle_( get(wnd) )
		{}

		~dc()
		{
			release( window_, handle_ );
		}

		HDC handle()
		{
			return handle_;
		}
	};

	/*----------------------------------------------------------------------*/

	class compatible_dc
	{
	private:
		HDC handle_;

	public:
		static HDC create_dc(HDC hdc)
		{
			HDC handle = ::CreateCompatibleDC( hdc );
			if (handle == 0)
				throw win32_error("CreateCompatibleDC");
			return handle;
		}

		static void delete_dc(HDC hdc)
		{
			SVL_VERIFY( ::DeleteDC( hdc ), != 0 );
		}

		compatible_dc(HDC hdc)
			: handle_( create_dc( hdc ) )
		{}

		~compatible_dc()
		{
			if (handle_ != 0)
				delete_dc( handle_ );
		}

		HDC detach()
		{
			HDC result = handle_;
			handle_ = 0;
			return result;
		}

		HDC handle() const
		{
			return handle_;
		}
	};
}}}
