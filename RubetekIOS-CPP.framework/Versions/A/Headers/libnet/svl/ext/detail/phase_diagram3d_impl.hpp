
namespace svl {
namespace ext {
namespace detail
{
	inline color get_spectrum_color(unsigned value, const unsigned max_value = 768)
	{
		assert( value < max_value );

		int val = (std::min)(value, max_value);
		int Spectrum_1_3 = max_value / 3;
		int Spectrum_1_2 = (max_value + 1) / 2;
		int Spectrum_2_3 = max_value * 2 / 3;

		int r = val < Spectrum_2_3 ? 255 * val / Spectrum_2_3 : 255;
		int g = val < Spectrum_1_2 ? 0 : 255 * (val - Spectrum_1_2) / Spectrum_1_2;
		int b = val < Spectrum_1_3 ? 128 * val / Spectrum_1_3 :
			(val < Spectrum_2_3 ? 128 * (Spectrum_2_3 - val) / Spectrum_1_3 :
			172 * (val - Spectrum_2_3) / Spectrum_1_3);
		return color(
			static_cast<unsigned char>(r),
			static_cast<unsigned char>(g),
			static_cast<unsigned char>(b));	
	}

	template<typename T> T sqr( T const& a )
	{
		return a * a;
	}
}}}

namespace svl {
namespace ext
{
	phase_diagram3d::phase_diagram3d( double dimension, unsigned points_count, unsigned grid_elements )
		: user_widget( color(0, 0, 0) )
		, canvas_in_use_( false )
		, dimension_( dimension )
		, history_( points_count, get_def_history_item() )
		, data_( detail::sqr(grid_elements + 2), 0 )
		, grid_elements_( grid_elements )
		, h_pos_( 0 )
	{
		{
			for (unsigned i = 0; i < s_colors_count; i++)
			{
				color c = detail::get_spectrum_color(i, s_colors_count);
				brushes[i].reset( new graphics::brush( c ) );
			}
		}

		event::resize ev;
		handle_resize(ev);

		timer.on_timer().add( &phase_diagram3d::handle_timer, this );
		timer.start(1000);
	}

	phase_diagram3d::~phase_diagram3d()
	{
		start_destruction();
	}

	void phase_diagram3d::clear()
	{
		std::fill( history_.begin(), history_.end(), get_def_history_item() );
		std::fill( data_.begin(), data_.end(), 0 );
		update();
	}

	void phase_diagram3d::handle_timer()
	{
		if (canvas)
		{
			if (!canvas_in_use_)
				canvas.reset();
			canvas_in_use_ = false;
		}
	}

	void phase_diagram3d::perform_drawing( cdouble const& p )
	{
		if (dimension_ <= abs(p.real()) || 
			dimension_ <= abs(p.imag()))
			return;

		if (!canvas)
			canvas.reset( new graphics::canvas(*this) );
		canvas_in_use_ = true;

		unsigned mult = grid_elements_ + 2;

		history_item cur;
		cur.x  = static_cast<unsigned>( (p.real() / dimension_ / 2.0 + 0.5) * grid_elements_ );
		cur.y  = static_cast<unsigned>( (-p.imag() / dimension_ / 2.0 + 0.5) * grid_elements_ );
		cur.xy = (cur.x + 1) + (cur.y + 1) * mult;

		history_item prev = history_[ h_pos_ ];
		history_[ h_pos_ ] = cur;

		unsigned m0 = 3;
		unsigned m1 = 2;
		unsigned m2 = 1;

		if (prev.xy != 0)
		{
			data_[ prev.xy ] -= m0;
			data_[ prev.xy - 1] -= m1;
			data_[ prev.xy + 1] -= m1;
			data_[ prev.xy + mult] -= m1;
			data_[ prev.xy - mult] -= m1;
			data_[ prev.xy + 1 - mult] -= m2;
			data_[ prev.xy + 1 + mult] -= m2;
			data_[ prev.xy - 1 - mult] -= m2;
			data_[ prev.xy - 1 + mult] -= m1;
		}

		data_[ cur.xy ] += m0;
		data_[ cur.xy - 1] += m1;
		data_[ cur.xy + 1] += m1;
		data_[ cur.xy + mult] += m1;
		data_[ cur.xy - mult] += m1;
		data_[ cur.xy + 1 - mult] += m2;
		data_[ cur.xy + 1 + mult] += m2;
		data_[ cur.xy - 1 - mult] += m2;
		data_[ cur.xy - 1 + mult] += m1;


		if (++h_pos_ == history_.size())
			h_pos_ = 0;

		if (prev.xy != 0)
		{
			draw_element( prev.x, prev.y, *canvas );
			draw_element( prev.x - 1, prev.y, *canvas );
			draw_element( prev.x + 1, prev.y, *canvas );
			draw_element( prev.x, prev.y - 1, *canvas );
			draw_element( prev.x, prev.y + 1, *canvas );
			draw_element( prev.x + 1, prev.y + 1, *canvas );
			draw_element( prev.x - 1, prev.y + 1, *canvas );
			draw_element( prev.x + 1, prev.y - 1, *canvas );
			draw_element( prev.x - 1, prev.y - 1, *canvas );
		}

		draw_element( cur.x,  cur.y,  *canvas );
		draw_element( cur.x - 1, cur.y, *canvas );
		draw_element( cur.x + 1, cur.y, *canvas );
		draw_element( cur.x, cur.y - 1, *canvas );
		draw_element( cur.x, cur.y + 1, *canvas );
		draw_element( cur.x + 1, cur.y + 1, *canvas );
		draw_element( cur.x - 1, cur.y + 1, *canvas );
		draw_element( cur.x + 1, cur.y - 1, *canvas );
		draw_element( cur.x - 1, cur.y - 1, *canvas );
	}

	void phase_diagram3d::pre_handle_event(event::processor& ep)
	{
		user_widget::pre_handle_event( ep );
		ep( &phase_diagram3d::handle_resize );
		ep( &phase_diagram3d::handle_paint );
	}

	void phase_diagram3d::handle_resize(event::resize&)
	{
		svl::size sz = size();
		center_ = point( sz.dx / 2, sz.dy / 2 );
		double vl = (std::min)( sz.dx, sz.dy );
		scale_ = vl / 2 / dimension_;

		if (sz.dx < sz.dy)
		{
			pos_x_ = 0;
			pos_y_ = (sz.dy - sz.dx) / 2.0;
			grid_scale_ = sz.dx / static_cast<double>(grid_elements_);
		}
		else
		{
			pos_x_ = (sz.dx - sz.dy) / 2.0;
			pos_y_ = 0;
			grid_scale_ = sz.dy / static_cast<double>(grid_elements_);
		}
	}

	void phase_diagram3d::handle_paint(event::paint&)
	{
		graphics::canvas canvas( *this );
		paint( canvas );
	}

	void phase_diagram3d::paint(graphics::painting& canvas)
	{
		for (unsigned j = 0; j < grid_elements_; j++)
			for (unsigned i = 0; i < grid_elements_; i++)
			{
				draw_element( i, j, canvas );
			}
	}

	void phase_diagram3d::draw_element( unsigned x, unsigned y, graphics::painting& canvas )
	{
		unsigned c = data_[ (x + 1) + (y + 1) * (grid_elements_ + 2) ];
		c = (std::min)(c, s_colors_count - 1);

		rect2 r;
		r.x0 = static_cast<unsigned>( pos_x_ + x * grid_scale_ + 1 );
		r.x1 = static_cast<unsigned>( pos_x_ + (x + 1.0) * grid_scale_ + 1 );
		r.y0 = static_cast<unsigned>( pos_y_ + y * grid_scale_ + 1 );
		r.y1 = static_cast<unsigned>( pos_y_ + (y + 1.0) * grid_scale_ + 1 );

		canvas.draw_rectangle( r, *brushes[c] );

	}

	phase_diagram3d::history_item phase_diagram3d::get_def_history_item()
	{
		history_item h = { 0, 0, 0 };
		return h;
	}
}}
