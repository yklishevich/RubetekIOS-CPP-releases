
namespace svl {
namespace detail
{
	enum text_change_type
	{
		edit_change,
		combo_edit_select
	};
}}

namespace svl
{
	inline mouse_key_state::mouse_key_state(unsigned state)
		: state_(state)
	{
	}
	
	inline bool mouse_key_state::is_pressed() const
	{
		return (state_ & 0x01) != 0;
	}
	
	inline bool mouse_key_state::just_changed() const
	{
		return (state_ & 0x02) != 0;
	}

	inline bool mouse_key_state::just_pressed() const
	{
		return state_ == 0x03;
	}

	inline bool mouse_key_state::just_released() const
	{
		return state_ == 0x02;
	}
	
	/*----------------------------------------------------------------------*/
	
	inline mouse_key_info::mouse_key_info(point const& p, unsigned state)
		: point_(p)
		, state_(state)
	{
	}
	
	inline point mouse_key_info::cursor() const
	{
		return point_;
	}

	inline mouse_key_state mouse_key_info::left() const
	{
		return state_ & 0x03;
	}
	
	inline mouse_key_state mouse_key_info::middle() const
	{
		return (state_ >> 2) & 0x03;
	}
	
	inline mouse_key_state mouse_key_info::right() const
	{
		return (state_ >> 4) & 0x03;
	}
	
	inline bool mouse_key_info::shift_pressed() const
	{
		return (state_ & 0x40) != 0;
	}
	
	inline bool mouse_key_info::ctrl_pressed() const
	{
		return (state_ & 0x80) != 0;
	}

	inline unsigned mouse_key_info::state() const
	{
		return state_;
	}

}

namespace svl {
namespace event
{
	inline close::close()
		: action_( action::do_nothing )
	{}

	inline close::action::type close::get_action() const
	{
		return action_;
	}

	inline void close::set_action(action::type a)
	{
		action_ = a;
	}

	/*----------------------------------------------------------------------*/

	inline key_down::key_down(unsigned key)
		: key_(key)
	{}

	inline unsigned key_down::win32_key() const
	{
		return key_;
	}

	/*----------------------------------------------------------------------*/

	inline activate::activate(bool a)
		: active_(a)
	{}

	inline bool activate::active() const
	{
		return active_;
	}

	/*----------------------------------------------------------------------*/

	inline select::select(size_t ind)
		: index_( ind )
	{}

	inline size_t select::index() const
	{
		return index_;
	}

	/*----------------------------------------------------------------------*/

	inline check::check(tribool c)
		: checked_( c )
	{}

	inline tribool check::checked() const
	{
		return checked_;
	}

	/*----------------------------------------------------------------------*/
	
	inline mouse_move::mouse_move(point const& p, unsigned state)
		: mouse_key_info(p, state)
	{}

	inline mouse_key_down::mouse_key_down(point const& p, unsigned state)
		: mouse_key_info(p, state)
	{}
	
	inline mouse_key_up::mouse_key_up(point const& p, unsigned state)
		: mouse_key_info(p, state)
	{}

	inline mouse_enter::mouse_enter(point const& p, unsigned state)
		: mouse_key_info(p, state)
	{}

	/*----------------------------------------------------------------------*/

	inline text_change::text_change(unsigned t, HWND h)
		: t_(t)
		, h_(h)
	{}

	inline string text_change::text() const
	{
		switch (t_)
		{
		case detail::edit_change: {
			string str(SVL_MS(GetWindowTextLength)(h_));
			int len = SVL_MS(GetWindowText)(h_, str.data(), static_cast<int>( str.capacity() ) );
			return str.resize(len);
			}
		case detail::combo_edit_select: {
			LRESULT index = SVL_MS(SendMessage)(h_, CB_GETCURSEL, 0, 0);
			SVL_ASSERT_2(index != CB_ERR);
			LRESULT length = SVL_MS(SendMessage)(h_, CB_GETLBTEXTLEN, index, 0);
			SVL_ASSERT_2(length != CB_ERR);
			string str(length);
			SVL_MS(SendMessage)(h_, CB_GETLBTEXT, index, reinterpret_cast<LPARAM>(str.data()));
			return str.resize(static_cast<std::size_t>(length));
			}
		default:
			SVL_ASSERT_2(FALSE);
			return string(0);
		}
	}

	/*----------------------------------------------------------------------*/

	namespace win32
	{
		struct command: entity
		{
			command( unsigned id )    : id_( id ) {}
			unsigned id() const       { return id_; }

			private: unsigned id_;
		};
	}
}}

//////////////////////////////////////////////////////////////////////////

namespace svl {
namespace detail
{
	event_id event_id_counter();

	/*----------------------------------------------------------------------*/

	template <class Event> struct event_id_of
	{
		static const event_id result;
	};

	template <class Event> const event_id event_id_of<Event>::result = event_id_counter();

	/*----------------------------------------------------------------------*/

	struct event_handler
	{
		virtual void handle(event::entity&) = 0;
		virtual ~event_handler() {}
	};

	/*----------------------------------------------------------------------*/

	template <class Event, class Handler> class event_handler_functor
		: public event_handler
	{
	public:
		event_handler_functor(Handler const& h_fn)
			: handle_( h_fn )
		{}

		void handle(event::entity& ev)
		{
			handle_( SVL_CAST(Event&, ev) );
		}

	private:
		event_handler_functor(event_handler_functor const&);
		event_handler_functor& operator=(event_handler_functor const&);

		Handler handle_;
	};

	/*----------------------------------------------------------------------*/

	template<class Event, class Handler> class event_handler_functor_0
		: public event_handler
	{
	public:
		event_handler_functor_0(Handler const& h_fn)
			: handle_(h_fn)
		{}

		void handle(event::entity&)
		{
			handle_();
		}

	private:
		event_handler_functor_0(event_handler_functor_0 const&);
		event_handler_functor_0& operator=(event_handler_functor_0 const&);

		Handler handle_;
	};

	/*----------------------------------------------------------------------*/

	template <class Event, class T> class event_handler_mem_fn_0
		: public event_handler
	{
	public:
		typedef void (T::*mem_fn_t)();

		event_handler_mem_fn_0(mem_fn_t fn, T* obj)
			: fn_ ( fn )
			, obj_( obj )
		{}

		void handle(event::entity&)
		{
			(obj_->*fn_)();
		}

	private:
		mem_fn_t fn_;
		T*       obj_;
	};

	/*----------------------------------------------------------------------*/

	template <class Event, class T> class event_handler_mem_fn_1
		: public event_handler
	{
	public:
		typedef void (T::*mem_fn_t)(Event&);

		event_handler_mem_fn_1(mem_fn_t fn, T* obj)
			: fn_ ( fn )
			, obj_( obj )
		{}

		void handle(event::entity& ev)
		{
			(obj_->*fn_)( SVL_CAST(Event&, ev) );
		}

	private:
		mem_fn_t fn_;
		T*       obj_;
	};

	/*----------------------------------------------------------------------*/

	struct destroy_handler
	{
		virtual destroy_handler* clone() = 0;
		virtual bool is_same_sink(void*) = 0;
		virtual bool is_equal(destroy_handler*) = 0;
		virtual void handle(event::target*) = 0;
		virtual ~destroy_handler() {}
	};

	/*----------------------------------------------------------------------*/

	template <class T> class destroy_handler_t
		: public destroy_handler
	{
	public:
		typedef void (T::*handle_fn)(event::target*);

		destroy_handler_t(handle_fn fn, T* obj)
			: fn_(fn), obj_(obj)
		{}

		bool is_same_sink(void* obj)
		{
			return obj_ == obj;
		}

		destroy_handler_t* clone()
		{
			return new destroy_handler_t(fn_, obj_);
		}

		void handle(event::target* target)
		{
			(obj_->*fn_)( target );
		}

		bool is_equal(destroy_handler* dh)
		{
			destroy_handler_t* dht = dynamic_cast<destroy_handler_t*>(dh);
			return (dht != 0) && (dht->fn_ == fn_) && (dht->obj_ == obj_);
		}

	private:
		handle_fn fn_;
		T*        obj_;
	};

	/*----------------------------------------------------------------------*/

	void add_event_handler(event::target*, event_id, event_handler*, env&); // own event_handler
	void add_destroy_handler(event::target*, destroy_handler*, env&);
	void remove_destroy_handler(event::target*, void* obj, env&); // throw
}}

//////////////////////////////////////////////////////////////////////////

namespace svl {
namespace event
{
	struct target::connector_info_t
	{
		target* target_;
	};

	/*----------------------------------------------------------------------*/

	template <class Event> bool target::handle_event(Event& e)
	{
		processor p( *this, e, detail::event_id_of<Event>::result );
		do_handle_event( p );
		return p.processed();
	}

	/*----------------------------------------------------------------------*/

	inline processor::processor(target& trgt, entity& e, detail::event_id id)
		: target_   (trgt)
		, event_id_ (id)
		, event_    (e)
		, processed_(false)
	{}

	template <class Event, class Target>
	inline void processor::operator () (void (Target::*fn)(Event&))
	{
		if (detail::event_id_of<Event>::result == event_id_)
		{
			(SVL_CAST(Target&, target_).*fn)(SVL_CAST(Event&, event_));
			processed_ = true;
		}
	}

	inline bool processor::processed() const
	{
		return processed_;
	}

	/*----------------------------------------------------------------------*/

	template <class Event>
	inline target::connector<Event>::connector(target::connector_info_t const& t)
		: target_( t.target_ )
	{}

	template <class Event> template <class F>
	inline target::connector<Event>& target::connector<Event>::add(F const& f)
	{
		detail::event_handler* handler = new detail::event_handler_functor<Event, F>(f);
		detail::add_event_handler( target_, detail::event_id_of<Event>::result, handler, target_->get_env() );
		return *this;
	}

	template <class Event> template <class T>
	inline target::connector<Event>& target::connector<Event>::add(void (T::*fn)(), T* p)
	{
		SVL_ASSERT( fn != 0 && p != 0);

		detail::event_handler* handler = new detail::event_handler_mem_fn_0<Event, T>(fn, p);
		detail::add_event_handler( target_, detail::event_id_of<Event>::result, handler, target_->get_env() );
		return *this;
	}

	template <class Event> template <class T>
	inline target::connector<Event>& target::connector<Event>::add(void (T::*fn)(Event&), T* p)
	{
		SVL_ASSERT( fn != 0 && p != 0);

		detail::event_handler* handler = new detail::event_handler_mem_fn_1<Event, T>(fn, p);
		detail::add_event_handler( target_, detail::event_id_of<Event>::result, handler, target_->get_env() );
		return *this;
	}

	template <class Event> template <class F>
	inline target::connector<Event>& target::connector<Event>::add(F const& f, connection& c)
	{
		detail::event_handler* handler = new detail::event_handler_functor<Event, F>(f);
		detail::add_event_handler( target_, detail::event_id_of<Event>::result, handler, target_->get_env() );
		c.connect( *target_, handler );
		return *this;
	}

	template <class Event> template <class T>
	inline target::connector<Event>& target::connector<Event>::add(void (T::*fn)(), T* p, connection& c)
	{
		SVL_ASSERT( fn != 0 && p != 0);

		detail::event_handler* handler = new detail::event_handler_mem_fn_0<Event, T>(fn, p);
		detail::add_event_handler( target_, detail::event_id_of<Event>::result, handler, target_->get_env() );
		c.connect( *target_, handler );
		return *this;
	}

	template <class Event> template <class T>
	inline target::connector<Event>& target::connector<Event>::add(void (T::*fn)(Event&), T* p, connection& c)
	{
		SVL_ASSERT( fn != 0 && p != 0);

		detail::event_handler* handler = new detail::event_handler_mem_fn_1<Event, T>(fn, p);
		detail::add_event_handler( target_, detail::event_id_of<Event>::result, handler, target_->get_env() );
		c.connect( *target_, handler );
		return *this;
	}

	template<class Event> template<class F> 
	inline target::connector<Event>& target::connector<Event>::operator+=(F const& f)
	{
		return add(f);
	}

	template <class Event> template <class F>
	inline target::connector<Event>& target::connector<Event>::operator|=(F const& f)
	{
		detail::event_handler* handler = new detail::event_handler_functor_0<Event, F>(f);
		detail::add_event_handler( target_, detail::event_id_of<Event>::result, handler, target_->get_env() );
		return *this;
	}

	/*----------------------------------------------------------------------*/

	inline target::destroy_connector::destroy_connector(target::connector_info_t const& t)
		: target_( t.target_ )
	{}

	template <class T>
	inline target::destroy_connector& target::destroy_connector::add(void (T::*fn)(target*), T* p)
	{
		detail::destroy_handler_t<T> handler( fn, p );
		detail::add_destroy_handler( target_, &handler, target_->get_env() );
		return *this;
	}

	template <class T>
	inline target::destroy_connector& target::destroy_connector::remove(T* p) // throw()
	{
		detail::remove_destroy_handler( target_, p, target_->get_env() );
		return *this;
	}
}}
