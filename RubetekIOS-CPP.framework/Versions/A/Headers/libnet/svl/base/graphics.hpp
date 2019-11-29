
#ifndef SVL_BASE_GRAPHICS_HPP
#define SVL_BASE_GRAPHICS_HPP

#include <svl/base/types.hpp>
#include <svl/base/string.hpp>
#include <svl/detail/win32.hpp>

namespace svl {
namespace graphics
{
	struct edge
	{
	public:
		static edge none;
		static edge bump;
		static edge etched;
		static edge raised;
		static edge sunken;

		struct border { enum type
		{
			none,
			raised,
			sunken
		};};

		struct lines { enum type
		{
			left   = 0x01,
			top    = 0x02,
			right  = 0x04,
			bottom = 0x08,
			all    = left | top | right | bottom
		};};

		explicit edge(border::type outer, border::type inner = border::none, unsigned lines = lines::all);

		border::type outer() const;
		border::type inner() const;
		unsigned     lines() const;
		unsigned     borders() const;

	private:
		unsigned borders_;
		unsigned lines_;
	};

	/*----------------------------------------------------------------------*/

	namespace text
	{
		typedef unsigned format;

		enum {
			multiline             = DT_WORDBREAK,
			left                  = DT_LEFT   | DT_TOP,
			right                 = DT_RIGHT  | DT_TOP,
			top                   = DT_CENTER | DT_TOP,

			// следующие опции действительны только при наличии опции multiline
			center                = DT_CENTER | DT_VCENTER | DT_SINGLELINE,
			bottom                = DT_CENTER | DT_BOTTOM  | DT_SINGLELINE,
			left_vcenter          = DT_LEFT   | DT_VCENTER | DT_SINGLELINE,
			right_vcenter         = DT_RIGHT  | DT_VCENTER | DT_SINGLELINE,
			left_bottom           = DT_LEFT   | DT_BOTTOM  | DT_SINGLELINE,
			right_bottom          = DT_RIGHT  | DT_BOTTOM  | DT_SINGLELINE
		};
	}

	/*----------------------------------------------------------------------*/

	class pen
	{
	public:
		explicit pen(color);
		pen(color, unsigned width);
		pen(pen const&);
		pen& operator = (pen const&);
		~pen();

		HPEN win32_handle() const;
		pen(HPEN, win32::system_object_t);

	private:
		win32::gdi_object<HPEN> pen_;
	};

	/*----------------------------------------------------------------------*/

	class brush
	{
	public:
		static const brush transparent;

		explicit brush(color);
		brush(brush const&);
		brush& operator = (brush const&);
		~brush();

		HBRUSH win32_handle() const;
		brush(HBRUSH, win32::system_object_t);

	private:
		win32::gdi_object<HBRUSH> brush_;
	};

	/*----------------------------------------------------------------------*/

	class font
	{
	public:
		static const font def;

		enum style
		{
			bold      = 0x01,
			italic    = 0x02,
			underline = 0x04
		};

		font(str_ref name, int height, unsigned styles = 0);
		font(font const&);
		font& operator = (font const&);
		~font();

		HFONT win32_handle() const;
		font(HFONT, win32::system_object_t);

	private:
		static HFONT create_font(str_ref, int, unsigned);
		win32::gdi_object<HFONT> font_;
	};

	/*----------------------------------------------------------------------*/

	class image
		: noncopyable
	{
	public:
		image(svl::size const& = svl::size(0, 0));
		~image();

		svl::size size() const;
		void resize(svl::size const&);

		HBITMAP win32_handle() const;

	private:
		friend class image_canvas;
		friend class buffered_canvas;
		win32::gdi_object<HBITMAP> bitmap_;
		svl::size size_;
	};

	/*----------------------------------------------------------------------*/

	class painting
		: noncopyable
	{
	public:
		typedef point const& point_;
		typedef rect  const& rect_;
		typedef rect2 const& rect2_;
		typedef size  const& size_;
		typedef pen   const& pen_;
		typedef brush const& brush_;
		typedef font  const& font_;

		void draw_pixel(point_, color);
		void move_to(point_);

		void draw_line_to(point_, pen_);

		void draw_line(point_ from, point_ to, pen_);

		void draw_rectangle(rect2_, brush_);
		void draw_rectangle(rect2_, pen_, brush_ = brush::transparent);

		void draw_round_rect(rect2_, size_, pen_, brush_ = brush::transparent);

		void draw_ellipse(rect2_, pen_, brush_ = brush::transparent);
		void draw_pie( rect2_, point_, point_, pen_, brush_ = brush::transparent);

		point draw_text(str_ref str, point_, font_, color text_color, color back_color = color::transparent);
		void  draw_text(str_ref str, rect2_, text::format, font_, color text_color, color back_color = color::transparent);

		void draw(edge, rect2_);
		void draw(image const&, point_);
		void draw(image const&, point_, rect_);

		HDC win32_handle() const;

	protected:
		painting();
		~painting();
		void open(HDC);
		void close();

	private:
		void select(pen const&);
		void select(brush const&);
		void select(font const&);
		
		HDC     hdc_;
		HGDIOBJ def_pen_;
		HPEN    cur_pen_;
		HGDIOBJ def_brush_;
		HBRUSH  cur_brush_;
		HGDIOBJ def_font_;
		HFONT   cur_font_;
	};

	/*----------------------------------------------------------------------*/

	class canvas
		: public painting
	{
	public:
		explicit canvas(window&);
		~canvas();

	private:
		bool own_;
		HWND hwnd_;
	};

	/*----------------------------------------------------------------------*/
	
	class image_canvas
		: public painting
	{
	public:
		explicit image_canvas(image&);
		~image_canvas();

	private:
		HGDIOBJ def_bitmap_;
		win32::gdi_object<HBITMAP> bitmap_;
	};

	/*----------------------------------------------------------------------*/

	class buffered_canvas
		: public painting
	{
	public:
		static image& shared_image();

		explicit buffered_canvas(window&, image& = shared_image());
		void flush();
		~buffered_canvas(); // автоматически вызывает flush (но "теряет" исключения)

		void fill_window_area();
		void fill_window_area(brush const&);

	private:
		static image& widen(image&, size const&);
		size size_;
		win32::gdi_object<HBITMAP> bitmap_;
		HWND hwnd_;
		bool own_;
		HDC  wnd_hdc_;
		HGDIOBJ def_bitmap_;
	};
}}

#endif // SVL_BASE_GRAPHICS_HPP
