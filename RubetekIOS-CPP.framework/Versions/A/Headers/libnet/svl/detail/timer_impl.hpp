
namespace svl
{
	timer::timer()
	{
	}

	timer::~timer()
	{
		start_destruction();
		stop();
	}

	void timer::start( unsigned ms )
	{
		get_env().timers.create( this, ms );
	}

	void timer::stop() // throw()
	{
		get_env().timers.destroy( this );
	}

	bool timer::started() const // throw()
	{
		return get_env().timers.is_created( this );
	}

	void timer::notify()
	{
		event::timer ev;
		handle_event( ev );
	}

	event::target::connector<event::timer> timer::on_timer()
	{
		return connection_info();
	}
}