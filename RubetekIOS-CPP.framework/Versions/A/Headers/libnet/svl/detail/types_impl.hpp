
namespace svl
{
	point::point()
		: x(0)
		, y(0)
	{}

	point::point( int X, int Y )
		: x( X )
		, y( Y )
	{}

	bool operator==(point const& a, point const& b)
	{
		return a.x == b.x && a.y == b.y;
	}
	
	bool operator!=(point const& a, point const& b)
	{
		return !(a == b);
	}

	/*----------------------------------------------------------------------*/

	size::size()
		: dx( 0 )
		, dy( 0 )
	{}

	size::size( int DX, int DY )
		: dx( DX )
		, dy( DY )
	{}

	bool is_norm( size const& s )
	{
		return 0 <= s.dx && 0 <= s.dy;
	}

	size norm( size const& s )
	{
		return size( 
			s.dx < 0 ? -s.dx : s.dx,
			s.dy < 0 ? -s.dy : s.dy
			);
	}
	
	bool operator==(size const& a, size const& b)
	{
		return a.dx == b.dx && a.dy == b.dy;
	}
	
	bool operator!=(size const& a, size const& b)
	{
		return !(a == b);
	}

	/*----------------------------------------------------------------------*/

	range::range()
		: r ( 0 )
		, dr( 0 )
	{}

	range::range( int R, int DR )
		: r ( R  )
		, dr( DR )
	{}

	range::range( range2 const& R )
		: r ( R.r0 )
		, dr( R.r1 - R.r0 )
	{}

	bool is_norm( range const& r )
	{
		return 0 <= r.dr ;
	}

	range norm( range const& r )
	{
		return 0 <= r.dr ? r : range(r.r + r.dr, -r.dr);
	}
	
	bool operator==(range const& a, range const& b)
	{
		return a.r == b.r && a.dr == b.dr;
	}
	
	bool operator!=(range const& a, range const& b)
	{
		return !(a == b);
	}

	/*----------------------------------------------------------------------*/

	range2::range2()
		: r0( 0 )
		, r1( 0 )
	{}

	range2::range2( int R0, int R1 )
		: r0( R0 )
		, r1( R1 )
	{}

	range2::range2( range const& r )
		: r0( r.r )
		, r1( r.r + r.dr )
	{}

	bool is_norm( range2 const& r )
	{
		return r.r0 <= r.r1;
	}

	range2 norm( range2 const& r )
	{
		return r.r0 <= r.r1 ? r : range2(r.r1, r.r0);
	}
	
	bool operator==(range2 const& a, range2 const& b)
	{
		return a.r0 == b.r0 && a.r1 == b.r1;
	}
	
	bool operator!=(range2 const& a, range2 const& b)
	{
		return !(a == b);
	}

	/*----------------------------------------------------------------------*/

	rect::rect()
		: x ( 0 )
		, y ( 0 )
		, dx( 0 )
		, dy( 0 )
	{}

	rect::rect( int X, int Y, int DX, int DY )
		: x ( X  )
		, y ( Y  )
		, dx( DX )
		, dy( DY )
	{}

	rect::rect( point const& p, svl::size const& s )
		: x ( p.x  )
		, y ( p.y  )
		, dx( s.dx )
		, dy( s.dy )
	{}

	rect::rect( range const& rx, range const& ry )
		: x ( rx.r  )
		, y ( ry.r  )
		, dx( rx.dr )
		, dy( ry.dr )
	{}

	rect::rect( rect2 const& r )
		: x ( r.x0 )
		, y ( r.y0 )
		, dx( r.x1 - r.x0 )
		, dy( r.y1 - r.y0 )
	{}

	range rect::x_range() const
	{
		return range( x, dx );
	}

	range rect::y_range() const
	{
		return range( y, dy );
	}

	point rect::pos() const
	{
		return point( x, y );
	}

	svl::size rect::size() const
	{
		return svl::size( dx, dy );
	}

	bool is_norm( rect const& r )
	{
		return 0 <= r.dx && 0 <= r.dy;
	}

	rect norm( rect r )
	{
		if (r.dx < 0)
		{
			r.x  += r.dx;
			r.dx = -r.dx;
		}
		if (r.dy < 0)
		{
			r.y  += r.dy;
			r.dy = -r.dy;
		}
		return r;
	}

	bool is_inside(point const& p, rect const& r)
	{
		return r.x <= p.x && p.x < (r.x + r.dx)   &&  r.y <= p.y && p.y <= (r.y + r.dy);
	}

	rect centre_rect(rect const& rt, size const& sz)
	{
		size s = norm(sz);
		rect2 r = norm(rt);
		return rect((r.x0 + r.x1 - s.dx) / 2, (r.y0 + r.y1 - s.dy) / 2, s.dx, s.dy);
	}
	
	bool operator==(rect const& a, rect const& b)
	{
		return a.x == b.x && a.y == b.y && a.dx == b.dx && a.dy == b.dy;
	}
	
	bool operator!=(rect const& a, rect const& b)
	{
		return !(a == b);
	}

	/*----------------------------------------------------------------------*/
	
	rect2::rect2()
		: x0( 0 )
		, y0( 0 )
		, x1( 0 )
		, y1( 0 )
	{}

	rect2::rect2( int X0, int Y0, int X1, int Y1 )
		: x0( X0 )
		, y0( Y0 )
		, x1( X1 )
		, y1( Y1 )
	{}

	rect2::rect2( point const& p, svl::size const& s)
		: x0( p.x )
		, y0( p.y )
		, x1( p.x + s.dx )
		, y1( p.y + s.dy )
	{}

	rect2::rect2( range2 const& rx, range2 const& ry )
		: x0( rx.r0 )
		, y0( ry.r0 )
		, x1( rx.r1 )
		, y1( ry.r1 )
	{}

	rect2::rect2( rect const& r )
		: x0( r.x )
		, y0( r.y )
		, x1( r.x + r.dx )
		, y1( r.y + r.dy )
	{}

	range2 rect2::x_range() const
	{
		return range2( x0, x1 );
	}

	range2 rect2::y_range() const
	{
		return range2( y0, y1 );
	}

	point rect2::pos() const
	{
		return point( x0, y0 );
	}

	svl::size rect2::size() const
	{
		return svl::size( x1 - x0, y1 - y0 );
	}

	bool is_norm( rect2 const& r )
	{
		return r.x0 <= r.x1 && r.y0 <= r.y1;
	}
	
	rect2 norm( rect2 r )
	{
		if (r.x1 < r.x0)
			std::swap( r.x0, r.x1 );
		if (r.y1 < r.y0)
			std::swap( r.y0, r.y1 );
		return r;
	}

	bool is_inside(point const& p, rect2 const& r)
	{
		return r.x0 <= p.x && p.x <= r.x1  &&  r.y0 <= p.y && p.y <= r.y1;
	}
	
	bool operator==(rect2 const& a, rect2 const& b)
	{
		return a.x0 == b.x0 && a.y0 == b.y0 && a.x1 == b.x1 && a.y1 == b.y1;
	}
	
	bool operator!=(rect2 const& a, rect2 const& b)
	{
		return !(a == b);
	}
	
	/*----------------------------------------------------------------------*/

	color::color()
		: b( 0 )
		, g( 0 )
		, r( 0 )
		, a( 0xFFu )
	{}

	color::color( byte R, byte G, byte B)
		: b( B )
		, g( G )
		, r( R )
		, a( 0xFFu )
	{}

	color::color( byte A, byte R, byte G, byte B )
		: b( B )
		, g( G )
		, r( R )
		, a( A )
	{}

	color::color( unsigned argb )
	{
		SVL_STATIC_ASSERT( sizeof(color) == sizeof(unsigned), "" );
		*reinterpret_cast<unsigned*>(this) = argb;
	}

	unsigned color::argb() const
	{
		SVL_STATIC_ASSERT( sizeof(color) == sizeof(unsigned), "" );

		return *reinterpret_cast<unsigned const*>(this);
	}

	bool operator==(color a, color b)
	{
		return a.a == b.a && a.r == b.r && a.g == b.g && a.b == b.b;
	}

	bool operator!=(color a, color b)
	{
		return !(a == b);
	}

	/*----------------------------------------------------------------------*/
	
	template <typename T> void unused(T const&) {}
	
	/*----------------------------------------------------------------------*/

#if !SVL_USE_BOOST_TRIBOOL
	namespace detail
	{
		const unsigned char false_value = 0;
		const unsigned char indeterminate_value = 1;
		const unsigned char true_value = 2;
	}

	tribool::tribool()
		: value_( detail::false_value )
	{}

	tribool::tribool(bool value)
		: value_( value ? detail::true_value : detail::false_value )
	{}

	tribool::tribool(indeterminate_t)
		: value_( detail::indeterminate_value )
	{}

	tribool::operator safe_bool () const
	{
		return safe_bool_value(value_ == detail::true_value);
	}

	tribool tribool::operator ! () const
	{
		return 
			   value_ == detail::false_value ? tribool(true)
			: (value_ == detail::true_value  ? tribool(false) 
			:                                  tribool(indeterminate) );
	}

	bool tribool::operator == (bool value) const
	{
		return value_ == (value ? detail::true_value : detail::false_value);
	}

	bool tribool::operator == (indeterminate_t) const
	{
		return value_ == detail::indeterminate_value;
	}

	bool indeterminate(tribool value, tribool*)
	{
		return value == indeterminate;
	}
#endif
}
