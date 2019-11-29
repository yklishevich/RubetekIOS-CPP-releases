
namespace svl {
namespace ext {
namespace detail
{
	unsigned PD_Back_Color   = 0xFF001440;
	unsigned PD_Points_Color = 0xFF80FF00;

	color middle_color(color c1, color c2)
	{
		return color( (c1.r + c2.r) / 2, (c1.g + c2.g) / 2, (c1.b + c2.b) / 2 );
	}

	double check_dimension(double val)
	{
		if (val <= 0.0)
			throw std::invalid_argument("dimension <= 0.0");
		return val;
	}

	unsigned check_points_count(unsigned val)
	{
		if (val == 0)
			throw std::invalid_argument("points_count == 0.0");
		return val;
	}

	inline svl::point scale(std::complex<double> const& value, std::complex<double> const& scaling, svl::point const& center)
	{
		double x = center.x + scaling.real() * value.real();
		double y = center.y + scaling.imag() * value.imag();
		return point(
			static_cast<int>(x + (value.real() >= 0.0 ? 0.5 : -0.5)),
			static_cast<int>(y + (value.imag() >= 0.0 ? -0.5 : 0.5)));
	}
}}}

namespace svl {
namespace ext
{
	phase_diagram::phase_diagram(double dimension, unsigned points_count)
		: user_widget(detail::PD_Back_Color)
		, color1_(detail::PD_Points_Color)
		, color2_(detail::middle_color(detail::PD_Points_Color, detail::PD_Back_Color))
		, color3_(detail::PD_Back_Color)
		, center_(-1000, -1000)
		, dimension_(detail::check_dimension(dimension))
		, points_count_(detail::check_points_count(points_count))
		, canvas_in_use_(false)
		, scale_(0.0)
		, pos_(0)
		, draw_points_(points_count_, point(-10, -10))
		, real_points_(points_count_, cdouble(-1000.0, -1000.0))
	{
		timer_.on_timer().add(&phase_diagram::handle_timer, this);
		timer_.start(1000);
	}

	phase_diagram::~phase_diagram()
	{
		timer_.stop();
		start_destruction();
	}

	void phase_diagram::pre_handle_event(event::processor& ep)
	{
		user_widget::pre_handle_event(ep);

		ep(&phase_diagram::handle_paint);
	}

	void phase_diagram::handle_paint(event::paint&)
	{
		graphics::canvas canvas(*this);

		unsigned middle_pos = (pos_ + points_count_ / 2) % points_count_;

		for (unsigned i = pos_; i != middle_pos; i = (i + 1) % points_count_)
			canvas.draw_pixel(draw_points_[i], color2_);

		for (unsigned i = middle_pos; i != pos_; i = (i + 1) % points_count_)
			canvas.draw_pixel(draw_points_[i], color1_);
	}

	void phase_diagram::set_points_count(unsigned points_count)
	{
		points_count = detail::check_points_count( points_count );
		std::vector<point>   draw_points(points_count, point(-10, -10));;
		std::vector<cdouble> real_points(points_count, cdouble(-1000.0, -1000.0));;

		points_count_ = points_count;
		draw_points_.swap( draw_points );
		real_points_.swap( real_points );
		repaint();
	}

	void phase_diagram::clear()
	{
		std::fill( draw_points_.begin(), draw_points_.end(), point(-10, -10) );
		std::fill( real_points_.begin(), real_points_.end(), cdouble(-1000.0, -1000.0) );
		repaint();
	}

	void phase_diagram::handle_timer()
	{
		if (!canvas_in_use_)
		{
			canvas_.reset();
			canvas_in_use_ = false;
		}
	}

	void phase_diagram::do_resize(svl::rect const& r)
	{
		user_widget::do_resize(r);
		perform_resizing(r.size());
	}

	void phase_diagram::do_resize(svl::size const& s)
	{
		user_widget::do_resize(s);
		perform_resizing(s);
	}

	void phase_diagram::perform_resizing(svl::size const& sz)
	{
		center_ = point(sz.dx / 2, sz.dy / 2);
		double min_size = (std::min)(sz.dx, sz.dy) / 2.0;
		scale_ = cdouble(min_size / dimension_, -min_size / dimension_);

		for (unsigned i = 0; i < points_count_; i++)
		{
			draw_points_[i] = detail::scale(real_points_[i], scale_, center_);
		}
	}

	void phase_diagram::perform_drawing(cdouble const& val)
	{
		if (canvas_.get() == 0)
			canvas_.reset(new graphics::canvas(*this));
		canvas_in_use_ = true;

		unsigned const middle_pos = (pos_ + points_count_ / 2) % points_count_;

		real_points_[pos_] = val;
		point& active_point = draw_points_[pos_];

		canvas_->draw_pixel(active_point, color3_);
		canvas_->draw_pixel(draw_points_[middle_pos], color2_);

		active_point = detail::scale(val, scale_, center_);
		canvas_->draw_pixel(active_point, color1_);

		pos_ = (pos_ + 1) % points_count_;
	}
}}
