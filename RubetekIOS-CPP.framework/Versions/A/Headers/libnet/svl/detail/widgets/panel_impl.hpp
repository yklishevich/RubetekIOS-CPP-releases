
namespace svl
{
	panel::panel(edge edge)
		: edge_( edge )
	{
		w32_create_child_window( WS_CLIPCHILDREN, 0 );
	}

	panel::~panel()
	{
		start_destruction();
		w32_destroy_window();
	}

	void panel::set_edge(edge edge)
	{
		edge_ = edge;
		update();
	}

	void panel::pre_handle_event( event::processor& ep )
	{
		container::pre_handle_event( ep );

		ep( &panel::handle_paint );
	}

	void panel::handle_paint( event::paint& )
	{
		svl::size sz = size();

		graphics::canvas canvas( *this );
		canvas.draw( edge_, svl::rect( 0, 0, sz.dx, sz.dy ) );
	}

	rect2 panel::do_get_margins() // override
	{
		int margin = 9;
		if (edge_.inner() == graphics::edge::border::none)
		{
			margin--;
			if (edge_.outer() == graphics::edge::border::none)
				margin--;
		}
		return rect2( margin, margin, margin, margin );
	}
}