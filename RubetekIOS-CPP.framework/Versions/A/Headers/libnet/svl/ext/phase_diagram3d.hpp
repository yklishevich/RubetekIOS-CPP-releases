#ifndef SVL_WIDGETS_PHASE_DIAGRAM3D_HPP
#define SVL_WIDGETS_PHASE_DIAGRAM3D_HPP

#include <svl/base/widget.hpp>
#include <svl/base/timer.hpp>
#include <complex>
#include <vector>
#include <memory>
#include <cassert>

namespace svl {
namespace ext
{
	class phase_diagram3d
		: public user_widget
	{
		static const unsigned s_colors_count = 64;

	public:
		phase_diagram3d( double dimension = 2.0, unsigned points_count = 20000, unsigned grid_elements = 120 );
		~phase_diagram3d();

		void clear();

		template <typename T> void draw(T x, T y)                    { perform_drawing( cdouble( static_cast<double>(x), static_cast<double>(y) )); }
		template <typename T> void draw(std::complex<T> const& xy)   { perform_drawing( cdouble( static_cast<double>(xy.real()), static_cast<double>(xy.imag()) )); }

	private:
		typedef std::complex<double> cdouble;

		void pre_handle_event(event::processor&);
		void handle_paint(event::paint&);
		void handle_timer();
		void handle_resize(event::resize&);
		void perform_drawing(cdouble const&);
		void draw_element( unsigned x, unsigned y, graphics::painting& canvas );
		void paint(graphics::painting& canvas);

		struct history_item
		{
			unsigned xy;
			unsigned x;
			unsigned y;
		};

		static history_item get_def_history_item();

		std::array< SVL_SHARED_PTR<graphics::brush>, s_colors_count > brushes;
		svl::timer                        timer;
		SVL_SHARED_PTR<graphics::canvas>  canvas;
		bool                              canvas_in_use_;
		point                             center_;
		double                            dimension_;
		double                            scale_;
		std::vector<history_item>         history_;
		unsigned                          h_pos_;
		std::vector<int>                  data_;
		unsigned                          grid_elements_;
		double                            grid_scale_;
		double                            pos_x_;
		double                            pos_y_;
	};
}}

#endif // SVL_WIDGETS_PHASE_DIAGRAM_3DHPP
