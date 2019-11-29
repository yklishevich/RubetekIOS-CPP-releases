#ifndef SVL_BASE_TYPES_HPP
#define SVL_BASE_TYPES_HPP

// все функции интерфейсов классов point, size, range, range2, rect, rect2, color имеют спецификацию throw()
namespace svl
{
	typedef unsigned char byte;
	using std::size_t;

	const size_t no_sel = size_t(0) - 1;

	/*----------------------------------------------------------------------*/

	struct point
	{
		int x;
		int y;

		point();
		point(int x, int y);
	};

	bool operator==(point const&, point const&);
	bool operator!=(point const&, point const&);

	/*----------------------------------------------------------------------*/

	struct size
	{
		int dx;
		int dy;

		size();
		size(int dx, int dy);
	};

	size norm(size const&);
	bool is_norm(size const&);
	bool operator==(size const&, size const&);
	bool operator!=(size const&, size const&);


	/*----------------------------------------------------------------------*/

	struct range
	{
		int r;
		int dr;

		range();
		range(int r, int dr);
		range(range2 const&);
	};

	range norm(range const&);
	bool is_norm(range const&);
	bool operator==(range const&, range const&);
	bool operator!=(range const&, range const&);

	/*----------------------------------------------------------------------*/

	struct range2
	{
		int r0;
		int r1;

		range2();
		range2(int r0, int r1);
		range2(range const&);
	};

	range2 norm(range2 const&);
	bool is_norm(range2 const&);
	bool operator==(range2 const&, range2 const&);
	bool operator!=(range2 const&, range2 const&);

	/*----------------------------------------------------------------------*/

	struct rect
	{
		int x;
		int y;
		int dx;
		int dy;

		rect();
		rect(int x, int y, int dx, int dy);
		rect(point const&, size const&);
		rect(range const& rx, range const& ry);
		rect(rect2 const&);

		range x_range() const;
		range y_range() const;
		point pos() const;
		svl::size size() const;
	};

	rect norm(rect);
	bool is_norm(rect const&);
	rect centre_rect(rect const& rt, size const& sz);
	bool is_inside(point const&, rect const&);
	bool operator==(rect const&, rect const&);
	bool operator!=(rect const&, rect const&);

	/*----------------------------------------------------------------------*/

	struct rect2
	{
		int x0;
		int y0;
		int x1;
		int y1;

		rect2();
		rect2(int x0, int y0, int x1, int y1);
		rect2(point const&, size const&);
		rect2(range2 const& rx, range2 const& ry);
		rect2(rect const&);

		range2 x_range() const;
		range2 y_range() const;
		point pos() const;
		svl::size size() const;
	};

	rect2 norm(rect2);
	bool is_norm(rect2 const&);
	bool is_inside(point const&, rect2 const&);
	bool operator==(rect2 const&, rect2 const&);
	bool operator!=(rect2 const&, rect2 const&);

	/*----------------------------------------------------------------------*/

	struct color
	{
		byte b;
		byte g;
		byte r;
		byte a;

		color();
		color(byte r, byte g, byte b);
		color(byte a, byte r, byte g, byte b);
		color(unsigned argb);
		unsigned argb() const;

		static const unsigned alice_blue             = 0xfff0f8ff;
		static const unsigned antique_white          = 0xfffaebd7;
		static const unsigned aqua                   = 0xff00ffff;
		static const unsigned aquamarine             = 0xff7fffd4;
		static const unsigned azure                  = 0xfff0ffff;
		static const unsigned beige                  = 0xfff5f5dc;
		static const unsigned bisque                 = 0xffffe4c4;
		static const unsigned black                  = 0xff000000;
		static const unsigned blanched_almond        = 0xffffebcd;
		static const unsigned blue                   = 0xff0000ff;
		static const unsigned blue_violet            = 0xff8a2be2;
		static const unsigned brown                  = 0xffa52a2a;
		static const unsigned burly_wood             = 0xffdeb887;
		static const unsigned cadet_blue             = 0xff5f9ea0;
		static const unsigned chartreuse             = 0xff7fff00;
		static const unsigned chocolate              = 0xffd2691e;
		static const unsigned coral                  = 0xffff7f50;
		static const unsigned cornflower_blue        = 0xff6495ed;
		static const unsigned cornsilk               = 0xfffff8dc;
		static const unsigned crimson                = 0xffdc143c;
		static const unsigned cyan                   = 0xff00ffff;
		static const unsigned dark_blue              = 0xff00008b;
		static const unsigned dark_cyan              = 0xff008b8b;
		static const unsigned dark_goldenrod         = 0xffb8860b;
		static const unsigned dark_gray              = 0xffa9a9a9;
		static const unsigned dark_green             = 0xff006400;
		static const unsigned dark_khaki             = 0xffbdb76b;
		static const unsigned dark_magenta           = 0xff8b008b;
		static const unsigned dark_olive_green       = 0xff556b2f;
		static const unsigned dark_orange            = 0xffff8c00;
		static const unsigned dark_orchid            = 0xff9932cc;
		static const unsigned dark_red               = 0xff8b0000;
		static const unsigned dark_salmon            = 0xffe9967a;
		static const unsigned dark_sea_green         = 0xff8fbc8b;
		static const unsigned dark_slate_blue        = 0xff483d8b;
		static const unsigned dark_slate_gray        = 0xff2f4f4f;
		static const unsigned dark_turquoise         = 0xff00ced1;
		static const unsigned dark_violet            = 0xff9400d3;
		static const unsigned deep_pink              = 0xffff1493;
		static const unsigned deep_sky_blue          = 0xff00bfff;
		static const unsigned dim_gray               = 0xff696969;
		static const unsigned dodger_blue            = 0xff1e90ff;
		static const unsigned firebrick              = 0xffb22222;
		static const unsigned floral_white           = 0xfffffaf0;
		static const unsigned forest_green           = 0xff228b22;
		static const unsigned fuchsia                = 0xffff00ff;
		static const unsigned gainsboro              = 0xffdcdcdc;
		static const unsigned ghost_white            = 0xfff8f8ff;
		static const unsigned gold                   = 0xffffd700;
		static const unsigned goldenrod              = 0xffdaa520;
		static const unsigned gray                   = 0xff808080;
		static const unsigned green                  = 0xff008000;
		static const unsigned green_yellow           = 0xffadff2f;
		static const unsigned honeydew               = 0xfff0fff0;
		static const unsigned hot_pink               = 0xffff69b4;
		static const unsigned indian_red             = 0xffcd5c5c;
		static const unsigned indigo                 = 0xff4b0082;
		static const unsigned ivory                  = 0xfffffff0;
		static const unsigned khaki                  = 0xfff0e68c;
		static const unsigned lavender               = 0xffe6e6fa;
		static const unsigned lavender_blush         = 0xfffff0f5;
		static const unsigned lawn_green             = 0xff7cfc00;
		static const unsigned lemon_chiffon          = 0xfffffacd;
		static const unsigned light_blue             = 0xffadd8e6;
		static const unsigned light_coral            = 0xfff08080;
		static const unsigned light_cyan             = 0xffe0ffff;
		static const unsigned light_goldenrod_yellow = 0xfffafad2;
		static const unsigned light_gray             = 0xffd3d3d3;
		static const unsigned light_green            = 0xff90ee90;
		static const unsigned light_pink             = 0xffffb6c1;
		static const unsigned light_salmon           = 0xffffa07a;
		static const unsigned light_sea_green        = 0xff20b2aa;
		static const unsigned light_sky_blue         = 0xff87cefa;
		static const unsigned light_slate_gray       = 0xff778899;
		static const unsigned light_steel_blue       = 0xffb0c4de;
		static const unsigned light_yellow           = 0xffffffe0;
		static const unsigned lime                   = 0xff00ff00;
		static const unsigned lime_green             = 0xff32cd32;
		static const unsigned linen                  = 0xfffaf0e6;
		static const unsigned magenta                = 0xffff00ff;
		static const unsigned maroon                 = 0xff800000;
		static const unsigned medium_aquamarine      = 0xff66cdaa;
		static const unsigned medium_blue            = 0xff0000cd;
		static const unsigned medium_orchid          = 0xffba55d3;
		static const unsigned medium_purple          = 0xff9370db;
		static const unsigned medium_sea_green       = 0xff3cb371;
		static const unsigned medium_slate_blue      = 0xff7b68ee;
		static const unsigned medium_spring_green    = 0xff00fa9a;
		static const unsigned medium_turquoise       = 0xff48d1cc;
		static const unsigned medium_violet_red      = 0xffc71585;
		static const unsigned midnight_blue          = 0xff191970;
		static const unsigned mint_cream             = 0xfff5fffa;
		static const unsigned misty_rose             = 0xffffe4e1;
		static const unsigned moccasin               = 0xffffe4b5;
		static const unsigned navajo_white           = 0xffffdead;
		static const unsigned navy                   = 0xff000080;
		static const unsigned old_lace               = 0xfffdf5e6;
		static const unsigned olive                  = 0xff808000;
		static const unsigned olive_drab             = 0xff6b8e23;
		static const unsigned orange                 = 0xffffa500;
		static const unsigned orange_red             = 0xffff4500;
		static const unsigned orchid                 = 0xffda70d6;
		static const unsigned pale_goldenrod         = 0xffeee8aa;
		static const unsigned pale_green             = 0xff98fb98;
		static const unsigned pale_turquoise         = 0xffafeeee;
		static const unsigned pale_violet_red        = 0xffdb7093;
		static const unsigned papaya_whip            = 0xffffefd5;
		static const unsigned peach_puff             = 0xffffdab9;
		static const unsigned peru                   = 0xffcd853f;
		static const unsigned pink                   = 0xffffc0cb;
		static const unsigned plum                   = 0xffdda0dd;
		static const unsigned powder_blue            = 0xffb0e0e6;
		static const unsigned purple                 = 0xff800080;
		static const unsigned red                    = 0xffff0000;
		static const unsigned rosy_brown             = 0xffbc8f8f;
		static const unsigned royal_blue             = 0xff4169e1;
		static const unsigned saddle_brown           = 0xff8b4513;
		static const unsigned salmon                 = 0xfffa8072;
		static const unsigned sandy_brown            = 0xfff4a460;
		static const unsigned sea_green              = 0xff2e8b57;
		static const unsigned sea_shell              = 0xfffff5ee;
		static const unsigned sienna                 = 0xffa0522d;
		static const unsigned silver                 = 0xffc0c0c0;
		static const unsigned sky_blue               = 0xff87ceeb;
		static const unsigned slate_blue             = 0xff6a5acd;
		static const unsigned slate_gray             = 0xff708090;
		static const unsigned snow                   = 0xfffffafa;
		static const unsigned spring_green           = 0xff00ff7f;
		static const unsigned steel_blue             = 0xff4682b4;
		static const unsigned tan                    = 0xffd2b48c;
		static const unsigned teal                   = 0xff008080;
		static const unsigned thistle                = 0xffd8bfd8;
		static const unsigned tomato                 = 0xffff6347;
		static const unsigned transparent            = 0x00ffffff;
		static const unsigned turquoise              = 0xff40e0d0;
		static const unsigned violet                 = 0xffee82ee;
		static const unsigned wheat                  = 0xfff5deb3;
		static const unsigned white                  = 0xffffffff;
		static const unsigned white_smoke            = 0xfff5f5f5;
		static const unsigned yellow                 = 0xffffff00;
		static const unsigned yellow_green           = 0xff9acd32;
	};

	bool operator==(color, color);
	bool operator!=(color, color);

	/*----------------------------------------------------------------------*/
#if !SVL_USE_BOOST_TRIBOOL
	class tribool
	{
	public:
		typedef bool (*indeterminate_t)(tribool, tribool*);

		tribool(); // = false
		tribool(bool value);
		tribool(indeterminate_t);

		operator safe_bool() const;
		tribool operator!() const;
		bool operator == (bool) const;
		bool operator == (indeterminate_t) const;

	private:
		unsigned char value_;
	};

	bool indeterminate(tribool, tribool* = 0);
#else
	using boost::tribool;
	using boost::indeterminate;
#endif
}

#endif //SVL_BASE_TYPES_HPP

