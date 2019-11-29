
namespace svl {
namespace graphics
{
	pen::pen(color colour)
		: pen_( win32::create_pen(PS_SOLID, 1, win32::rgb_from_color(colour) ))
	{}

	pen::pen(color colour, unsigned width)
		: pen_( win32::create_pen(PS_SOLID, static_cast<int>(width), win32::rgb_from_color(colour) ))
	{}

	pen::pen(pen const& other)
		: pen_( other.pen_ )
	{}

	pen& pen::operator = (pen const& other)
	{
		pen_ = other.pen_;
		return *this;
	}

	pen::~pen()
	{}

	HPEN pen::win32_handle() const
	{
		return pen_;
	}

	pen::pen(HPEN hpen, win32::system_object_t)
		: pen_( hpen, win32::system_object )
	{}

	/*----------------------------------------------------------------------*/

	brush::brush(color colour)
		: brush_( win32::create_solid_brush( win32::rgb_from_color(colour) ))
	{}

	brush::brush(brush const& other)
		: brush_( other.brush_ )
	{}

	brush& brush::operator = (brush const& other)
	{
		brush_ = other.brush_;
		return *this;
	}

	HBRUSH brush::win32_handle() const
	{
		return brush_;
	}

	brush::~brush()
	{}

	brush::brush(HBRUSH hbrush, win32::system_object_t)
		: brush_( hbrush, win32::system_object )
	{}

	brush const brush::transparent = brush( win32::get_stock_object<HBRUSH>(HOLLOW_BRUSH), win32::system_object );

	/*----------------------------------------------------------------------*/

	font::font(str_ref name, int height, unsigned styles)
		: font_( create_font( name, height, styles ))
	{}

	font::font(font const& other)
		: font_( other.font_ )
	{}

	font& font::operator = (font const& other)
	{
		font_ = other.font_;
		return *this;
	}

	HFONT font::create_font(str_ref name, int height, unsigned styles)
	{
		HFONT hfont = SVL_MS(CreateFont)(
			/* nHeight            */ height,
			/* nWidth             */ 0,
			/* nEscapement        */ 0,
			/* nOrientation       */ 0,
			/* fnWeight           */ (styles & font::bold) ? FW_BOLD : FW_NORMAL,
			/* fdwItalic          */ (styles & font::italic) ? TRUE : FALSE,
			/* fdwUnderline       */ (styles & font::underline) ? TRUE : FALSE,
			/* fdwStrikeOut       */ FALSE,
			/* fdwCharSet         */ ANSI_CHARSET,
			/* fdwOutputPrecision */ OUT_DEFAULT_PRECIS,
			/* fdwClipPrecision   */ CLIP_DEFAULT_PRECIS,
			/* fdwQuality         */ DEFAULT_QUALITY,
			/* fdwPitchAndFamily  */ DEFAULT_PITCH,
			/* lpszFace           */ name.data());

		if (hfont == 0)
			throw win32_error( "CreateFont" );

		return hfont;
	}

	HFONT font::win32_handle() const
	{
		return font_;
	}

	font::~font()
	{}

	font::font(HFONT hfont, win32::system_object_t)
		: font_( hfont, win32::system_object )
	{}

	font const font::def = font( win32::get_stock_object<HFONT>(DEFAULT_GUI_FONT), win32::system_object );

	/*----------------------------------------------------------------------*/

	edge::edge(border::type outer, border::type inner, unsigned lines)
		: borders_( outer | +inner << 2 )
		, lines_( lines )
	{}

	edge::border::type edge::outer() const
	{
		return static_cast<border::type>( borders_ & 0x03 );
	}

	edge::border::type edge::inner() const
	{
		return static_cast<border::type>( borders_ >> 2 );
	}

	unsigned edge::lines() const
	{
		return lines_;
	}

	unsigned edge::borders() const
	{
		return borders_;
	}

	edge edge::none   = edge( edge::border::none );
	edge edge::bump   = edge( edge::border::raised, edge::border::sunken );
	edge edge::etched = edge( edge::border::sunken, edge::border::raised );
	edge edge::raised = edge( edge::border::raised, edge::border::raised );
	edge edge::sunken = edge( edge::border::sunken, edge::border::sunken );

	/*----------------------------------------------------------------------*/

	painting::painting()
		: def_pen_  ( 0 )
		, cur_pen_  ( 0 )
		, def_brush_( 0 )
		, cur_brush_( 0 )
		, def_font_ ( 0 )
		, cur_font_ ( 0 )
	{
		SVL_IN_DEBUG_2( hdc_ = 0; )
	}

	painting::~painting()
	{
		SVL_ASSERT_2( hdc_ == 0 );
	}

	void painting::open( HDC hdc ) // throw()
	{
		SVL_ASSERT_2( hdc_ == 0 );
		hdc_ = hdc;
	}

	void painting::close() // throw()
	{
		if (def_pen_ != 0)
			win32::select_object<std::nothrow_t>( hdc_, def_pen_ );
		if (def_brush_ != 0)
			win32::select_object<std::nothrow_t>( hdc_, def_brush_ );
		if (def_font_ != 0)
			win32::select_object<std::nothrow_t>( hdc_, def_font_ );

		SVL_IN_DEBUG_2( hdc_ = 0; )
	}

	void painting::select(pen const& p)
	{
		HPEN hpen = p.win32_handle();
		if (hpen != cur_pen_)
		{
			HGDIOBJ prev = win32::select_object( hdc_, hpen );
			cur_pen_ = hpen;
			if (def_pen_ == 0)
				def_pen_ = prev;
		}
	}

	void painting::select(brush const& b)
	{
		HBRUSH hbrush = b.win32_handle();
		if (hbrush != cur_brush_)
		{
			HGDIOBJ prev = win32::select_object( hdc_, hbrush );
			cur_brush_ = hbrush;
			if (def_brush_ == 0)
				def_brush_ = prev;
		}
	}

	void painting::select(font const& f)
	{
		HFONT hfont = f.win32_handle();
		if (hfont != cur_font_)
		{
			HGDIOBJ prev = win32::select_object( hdc_, hfont );
			cur_font_ = hfont;
			if (def_font_ == 0)
				def_font_ = prev;
		}
	}

	HDC painting::win32_handle() const
	{
		return hdc_;
	}

	void painting::draw_pixel(point const& p, color c)
	{
		::SetPixel( hdc_, p.x, p.y, win32::rgb_from_color(c) );
	}

	void painting::move_to(point const& pos)
	{
		SVL_VERIFY( ::MoveToEx( hdc_, pos.x, pos.y, 0 ), != 0 );
	}

	void painting::draw_line_to(point const& pos, pen const& p)
	{
		select( p );
		SVL_VERIFY( ::LineTo( hdc_, pos.x, pos.y ), != 0 );
	}

	void painting::draw_line(point const& from, point const& to, pen const& p)
	{
		move_to( from );
		draw_line_to( to, p );
	}

	void painting::draw_rectangle(rect2 const& r, brush const& b)
	{
		RECT R = win32::from_rect2( r );
		SVL_VERIFY( ::FillRect( hdc_, &R, b.win32_handle() ), != 0 );
	}

	void painting::draw_rectangle(rect2 const& r, pen const& p, brush const& b)
	{
		select( p );
		select( b );
		SVL_VERIFY( ::Rectangle( hdc_, r.x0, r.y0, r.x1, r.y1 ), != 0 );
	}

	void painting::draw_round_rect(rect2 const& r, size const& ell_sz, pen const& p, brush const& b)
	{
		select( p );
		select( b );
		SVL_VERIFY( ::RoundRect( hdc_, r.x0, r.y0, r.x1, r.y1, ell_sz.dx, ell_sz.dy ), != 0 );
	}

	void painting::draw_ellipse(rect2 const& r, pen const& p, brush const& b)
	{
		select( p );
		select( b );
		SVL_VERIFY( ::Ellipse( hdc_, r.x0, r.y0, r.x1, r.y1 ), != 0 );
	}

	void painting::draw_pie( rect2 const& r, point const& p0, point const& p1, pen const& p, brush const& b )
	{
		select( p );
		select( b );
		SVL_VERIFY( ::Pie( hdc_, r.x0, r.y0, r.x1, r.y1, p0.x, p0.y, p1.x, p1.y ), != 0 );
	}

	point painting::draw_text(str_ref str, point const& p, font const& f, color text_color, color back_color)
	{
		select( f );
		SVL_VERIFY( ::SetTextAlign( hdc_, TA_LEFT | TA_TOP | TA_UPDATECP ), != GDI_ERROR );
		SVL_VERIFY( ::SetTextColor( hdc_, win32::rgb_from_color(text_color)), != CLR_INVALID );
		if (back_color.argb() == color::transparent)
		{
			SVL_VERIFY( ::SetBkMode( hdc_, TRANSPARENT ), != 0 );
		}
		else
		{
			SVL_VERIFY( ::SetBkColor( hdc_, win32::rgb_from_color(back_color) ), != CLR_INVALID );
			SVL_VERIFY( ::SetBkMode( hdc_, OPAQUE ), != 0 );
		}
		SVL_VERIFY( ::MoveToEx( hdc_, p.x, p.y, 0 ), != 0 );
		SVL_VERIFY( SVL_MS(TextOut)( hdc_, 0, 0, str.data(), static_cast<int>(str.size()) ), != 0 );
		POINT P;
		SVL_VERIFY( ::MoveToEx( hdc_, 0, 0, &P ), != 0 );
		return win32::to_point( P );
	}

	void painting::draw_text(str_ref str, rect2 const& r, text::format frmt, font const& f, color text_color, color back_color)
	{
		select( f );
		SVL_VERIFY( ::SetTextAlign( hdc_, TA_LEFT | TA_TOP | TA_NOUPDATECP ), != GDI_ERROR );
		SVL_VERIFY( ::SetTextColor( hdc_, win32::rgb_from_color(text_color)), != CLR_INVALID );
		if (back_color.argb() == color::transparent)
		{
			SVL_VERIFY( ::SetBkMode( hdc_, TRANSPARENT ), != 0 );
		}
		else
		{
			SVL_VERIFY( ::SetBkColor( hdc_, win32::rgb_from_color(back_color) ), != CLR_INVALID );
			SVL_VERIFY( ::SetBkMode( hdc_, OPAQUE ), != 0 );
		}

		RECT R = win32::from_rect2( r );
		SVL_MS(DrawText)( hdc_, str.data(), static_cast<int>( str.size() ), &R, frmt );
	}

	void painting::draw(edge e, rect2 const& r)
	{
		RECT R = win32::from_rect2( r );
		SVL_VERIFY( ::DrawEdge( hdc_, &R, e.borders(), e.lines() ), != 0 );
	}

	void painting::draw(image const& img, point const& p)
	{
		draw( img, p, rect(point(0,0), img.size()) );
	}

	void painting::draw(image const& img, point const& p, rect const& r)
	{
		win32::compatible_dc mem_dc( win32_handle() );
		HGDIOBJ def_bitmap = win32::select_object( mem_dc.handle(), img.win32_handle() );

		SVL_VERIFY( ::BitBlt( win32_handle(), p.x, p.y, r.dx, r.dy, mem_dc.handle(), r.x, r.y, SRCCOPY ), != 0 );

		win32::select_object<std::nothrow_t>( mem_dc.handle(), def_bitmap );
	}

	/*----------------------------------------------------------------------*/

	canvas::canvas(window& wnd)
	{
		hwnd_ = wnd.win32_handle();
		HDC hdc = wnd.get_env().device_contexts.find( wnd );
		own_ = hdc == 0;

		if (own_)
			hdc = win32::dc::get( hwnd_ );

		open( hdc );
	}

	canvas::~canvas()
	{
		HDC hdc = win32_handle();
		close();
		if (own_)
			win32::dc::release( hwnd_, hdc );
	}

	/*----------------------------------------------------------------------*/

	image::image( svl::size const& sz)
		: bitmap_( 0, win32::system_object )
		, size_  ( sz )
	{
		resize( sz );
	}

	image::~image()
	{}

	size image::size() const
	{
		return size_;
	}

	void image::resize(svl::size const& sz)
	{
		if ( 0 < sz.dx && 0 < sz.dy )
		{
			win32::dc dc( svl::detail::env::shared_instance()->sys_window );

			win32::gdi_object<HBITMAP> new_bitmap( 
				win32::create_compatible_bitmap(dc.handle(), sz.dx, sz.dy )
				);

			bitmap_.swap( new_bitmap );
			size_ = sz;
		}
		else
		{
			win32::gdi_object<HBITMAP> empty_bitmap( 0, win32::system_object );
			bitmap_.swap( empty_bitmap );
			size_ = svl::size( 0, 0 );
		}
	}

	HBITMAP image::win32_handle() const
	{
		return bitmap_;
	}

	/*----------------------------------------------------------------------*/

	image_canvas::image_canvas(image& img)
		: bitmap_( img.bitmap_ )
	{
		win32::dc dc( svl::detail::env::shared_instance()->sys_window );
		win32::compatible_dc mem_dc( dc.handle() );

		open( mem_dc.handle() );

		def_bitmap_ = win32::select_object( mem_dc.handle(), bitmap_ );
		mem_dc.detach();
	}

	image_canvas::~image_canvas()
	{
		HDC hdc = win32_handle();
		close();
		win32::select_object<std::nothrow_t>( hdc, def_bitmap_ );
		win32::compatible_dc::delete_dc( hdc );
	}

	/*----------------------------------------------------------------------*/

	image& buffered_canvas::shared_image()
	{
		return svl::detail::env::shared_instance()->shared_image;
	}

	image& buffered_canvas::widen(image& img, size const& sz)
	{
		size img_size = img.size();
		if (img_size.dx < sz.dx || img_size.dy < sz.dy)
		{
			img_size = size(
				(std::max)(img_size.dx, sz.dx),
				(std::max)(img_size.dy, sz.dy)
				);
			img.resize( img_size );
		}
		return img;
	}

	buffered_canvas::buffered_canvas(window& w, image& img)
		: size_( w.size() )
		, bitmap_( widen( img, size_ ).bitmap_ )
		, hwnd_( w.win32_handle() )
	{
		wnd_hdc_ = w.get_env().device_contexts.find( w );
		own_ = wnd_hdc_ == 0;

		if (own_)
			wnd_hdc_ = win32::dc::get( hwnd_ );

		try
		{
			win32::compatible_dc mem_dc( wnd_hdc_ );
			open( mem_dc.handle() );
			def_bitmap_ = win32::select_object( mem_dc.handle(), bitmap_ );
			mem_dc.detach();
		}
		catch (...)
		{
			if (own_)
				win32::dc::release( hwnd_, wnd_hdc_ );
			throw;
		}
	}

	buffered_canvas::~buffered_canvas()
	{
		try
		{
			flush();
		}
		catch (...)
		{
			SVL_ASSERT_FALSE();
		}

		HDC mem_hdc = win32_handle();
		close();
		win32::select_object<std::nothrow_t>( mem_hdc, def_bitmap_ );
		win32::compatible_dc::delete_dc( mem_hdc );

		if (own_)
			win32::dc::release( hwnd_, wnd_hdc_ );
	}

	void buffered_canvas::flush()
	{
		SVL_VERIFY( ::BitBlt( wnd_hdc_, 0, 0, size_.dx, size_.dy, win32_handle(), 0, 0, SRCCOPY ), != 0 );
	}

	void buffered_canvas::fill_window_area(brush const& b)
	{
		draw_rectangle( rect2(0, 0, size_.dx, size_.dy), b );
	}

	void buffered_canvas::fill_window_area()
	{
		HBRUSH hbrush = reinterpret_cast<HBRUSH>(
			SVL_MS(GetClassLongPtr)( hwnd_, GCLP_HBRBACKGROUND )
			);

		if (hbrush != 0)
			fill_window_area( brush(hbrush, win32::system_object) );
	}
}}
