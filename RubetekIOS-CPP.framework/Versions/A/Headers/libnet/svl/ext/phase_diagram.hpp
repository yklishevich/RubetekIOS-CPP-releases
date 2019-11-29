#ifndef SVL_WIDGETS_PHASE_DIAGRAM_HPP
#define SVL_WIDGETS_PHASE_DIAGRAM_HPP

#include <svl/base/widget.hpp>
#include <svl/base/timer.hpp>
#include <complex>
#include <vector>
#include <memory>

namespace svl {
namespace ext
{
	class phase_diagram
		: public user_widget
	{
	public:
		phase_diagram(double dimension = 2.0, unsigned points_count = 1024);

		void set_dimension(double);
		void set_points_count(unsigned);
		void set_points_color(color);
		void set_back_color(color);
		void clear();
		
		template <typename T> void draw(T x, T y);
		template <typename T> void draw(std::complex<T> const&);

		~phase_diagram();

	private:
		typedef std::complex<double> cdouble;

		void pre_handle_event(event::processor&);
		void handle_paint(event::paint&);
		void handle_timer();
		void do_resize(svl::rect const&);
		void do_resize(svl::size const&);
		void perform_resizing(svl::size const&);
		void perform_drawing(cdouble const&);

		color    color1_;
		color    color2_;
		color    color3_;
		point    center_;
		double   dimension_;
		unsigned points_count_;
		cdouble  scale_;
		unsigned pos_;
		timer    timer_;
		bool     canvas_in_use_;
		std::vector<cdouble>             real_points_;
		std::vector<point>               draw_points_;
		SVL_SHARED_PTR<graphics::canvas> canvas_;
	};
}}

#include <svl/ext/detail/phase_diagram_incl.hpp>

#endif // SVL_WIDGETS_PHASE_DIAGRAM_HPP
