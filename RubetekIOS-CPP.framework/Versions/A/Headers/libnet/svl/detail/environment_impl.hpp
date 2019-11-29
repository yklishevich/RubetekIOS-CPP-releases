
namespace svl {
namespace detail
{
	template <typename T> class tls_data
	{
	public:
		tls_data()
		{
			tls_index_ = ::TlsAlloc();
			if (tls_index_ == TLS_OUT_OF_INDEXES)
				throw win32_error("TlsAlloc");
		}

		T get() // throw()
		{
			SVL_ASSERT( tls_index_ != TLS_OUT_OF_INDEXES );

			void* result = ::TlsGetValue(tls_index_);
			return reinterpret_cast<T>(result);
		}

		void set(T v)
		{
			SVL_ASSERT( tls_index_ != TLS_OUT_OF_INDEXES );

			BOOL result = ::TlsSetValue(tls_index_, reinterpret_cast<void*>( v ));
			if (!result)
				throw win32_error("TlsSetValue");
		}

		~tls_data()
		{
			::TlsFree(tls_index_);
			tls_index_ = TLS_OUT_OF_INDEXES;
		}

	private:
		unsigned tls_index_;
	};

	event_id event_id_counter()
	{
		static event_id id = 0;
		return ++id;
	}
}}

//////////////////////////////////////////////////////////////////////////

namespace svl {
namespace
{
	svl::detail::tls_data< SVL_SHARED_PTR<svl::detail::env>* > g_tls;


	tstring to_hex(unsigned __int64 value, unsigned bytes_count = 4)
	{
		tstring result(bytes_count * 2, '0');
		for (unsigned i = 0; i < bytes_count * 2; i++)
		{
			unsigned v = (value >> (i * 4)) & 0x0F;
			result[i] = static_cast<tchar>(
				v + (v < 10 ? SVL_T('0') : (SVL_T('A') - 10))
				);
		}
		return result;
	};

	const unsigned WM_SVL_EVENT = WM_USER + 123;
}}

//////////////////////////////////////////////////////////////////////////

namespace svl {
namespace detail
{
	class w32_sys_window
		: noncopyable
	{
	public:
		w32_sys_window()
			: wnd_class_( 0 )
		{
			INITCOMMONCONTROLSEX icce_data;
			icce_data.dwSize = sizeof(icce_data);
			icce_data.dwICC = ICC_WIN95_CLASSES;
			SVL_VERIFY(::InitCommonControlsEx(&icce_data), != 0);

			try
			{
				tchar const* class_name = SVL_T("SVL_WIN_SYSTEM");
				wnd_class_ = win32::register_class(class_name, &system_proc, 0, 0, 0); // ќ— сама уничтожит класс окна, когда приложение завершитс€
				handle_ = win32::create_window(class_name, 0, WS_OVERLAPPEDWINDOW);
			}
			catch (win32_error const& e)
			{
				if (e.err_code() != ERROR_CLASS_ALREADY_EXISTS)
				{
					if (wnd_class_ != 0)
						win32::unregister_class( wnd_class_ );
					throw;
				}
			}
		}

		~w32_sys_window()
		{
			win32::destroy_window( handle_ );
			win32::unregister_class( wnd_class_ );
		}

		operator HWND() const
		{
			return handle_;
		}

	private:
		static LRESULT CALLBACK system_proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
		{
			if (uMsg == WM_COMMAND)
			{
				HWND ctrl_hwnd = reinterpret_cast<HWND>( lParam );
				if (ctrl_hwnd != hwnd)
					return SVL_MS(SendMessage)( ctrl_hwnd, uMsg, wParam, lParam );
			}
			return SVL_MS(DefWindowProc)(hwnd, uMsg, wParam, lParam);
		}

		ATOM wnd_class_;
		HWND handle_;
	};

	/*----------------------------------------------------------------------*/

	class destroy_event_processor
		: noncopyable
	{
	private:
		typedef std::multimap< event::target*, SVL_SHARED_PTR<destroy_handler> > map_t;
		typedef map_t::value_type map_value;
		typedef map_t::iterator   map_iterator;
		map_t map_;

	public:
		void add(event::target* t, destroy_handler* h)
		{
			SVL_SHARED_PTR<destroy_handler> handler;

			map_iterator end = map_.end();
			for (map_iterator iter = map_.begin(); iter != end; ++iter )
			{
				if (iter->second->is_equal( h ))
				{
					handler = iter->second;
					break;
				}
			}

			if (handler.get() == 0)
				handler.reset( h->clone() );

			map_.insert( map_value(t, handler) );
		}

		void remove(event::target* t, void* obj) // throw()
		{
			map_iterator iter = map_.find( t );
			map_iterator end = map_.end();
			while (iter != end && iter->first == t)
			{
				map_iterator i = iter;
				++iter;
				if (i->second->is_same_sink(obj))
					map_.erase(i);
			}
		}

		void notify(event::target* t) // throw()
		{
			map_iterator iter = map_.find( t );
			map_iterator end = map_.end();
			while (iter != end && iter->first == t)
			{
				map_iterator i = iter;
				++iter;
				i->second->handle( t );
				map_.erase( i );
			}
		}

		~destroy_event_processor()
		{
			SVL_ASSERT( map_.empty() );
		}
	};

	/*----------------------------------------------------------------------*/

	class event_processor
		: noncopyable
	{
	private:
		struct key
		{
			event::target*   target;
			detail::event_id event_id;

			key(event::target* t, svl::detail::event_id id)
				: target( t )
				, event_id( id )
			{}

			bool operator == (key const& other) const
			{
				return (target == other.target) && (event_id == other.event_id);
			}

			bool operator < (key const& other) const
			{
				if (target == other.target)
					return event_id < other.event_id;
				else
					return target < other.target;
			}
		};

		typedef std::multimap<key, event_handler*> map_t;
		typedef map_t::value_type map_value;
		typedef map_t::iterator   map_iterator;
		map_t map_;

	public:
		void add(event::target* t, event_id eid, event_handler* eh) // own event_handler
		{
			std::auto_ptr<event_handler> ehp(eh);

			map_.insert( map_value( key(t, eid), eh ) );

			ehp.release();
		}

		void remove(event::target* t, event_handler* eh) // throw()
		{
			for (map_iterator iter = map_.begin(), end = map_.end(); iter != end; ++iter)
			{
				if (iter->first.target == t && iter->second == eh)
				{
					delete eh;
					map_.erase(iter);
					break;
				}
			}
		}

		void remove_all(event::target* t, event_id eid) // throw()
		{
			map_iterator iter = map_.begin();
			map_iterator end = map_.end();
			while (iter != end)
			{
				map_iterator i = iter;
				++iter;
				if (i->first.target == t && i->first.event_id == eid)
				{
					delete i->second;
					map_.erase(i);
				}
			}
		}

		void remove_all(event::target* t) // throw()
		{
			map_iterator iter = map_.begin();
			map_iterator end = map_.end();
			while (iter != end)
			{
				map_iterator i = iter;
				++iter;
				if (i->first.target == t)
				{
					delete i->second;
					map_.erase(i);
				}
			}
		}

		bool is_handler_exist(event::target* t, event_handler* eh) // throw()
		{
			for (map_iterator iter = map_.begin(), end = map_.end(); iter != end; ++iter)
			{
				if (iter->first.target == t && iter->second == eh)
					return true;
			}
			return false;
		}

		bool handle(event::target* t, event_id eid, event::entity& ev)
		{
			bool processed = false;
			key k(t, eid);

			map_iterator iter = map_.find( k );
			map_iterator end = map_.end();
			for (; iter != end && iter->first == k; ++iter)
			{
				iter->second->handle(ev);
				processed = true;
			}

			return processed;
		}

		~event_processor()
		{
			SVL_ASSERT( map_.empty() );
		}
	};

	/*----------------------------------------------------------------------*/

	class w32_dc_handles
	{
	private:
		struct item
		{
			HDC     hdc;
			window* wnd;
		};
		std::vector<item> items_;
		unsigned count_;

	public:
		w32_dc_handles()
			: items_( 10 )
			, count_( 0 )
		{}

		~w32_dc_handles()
		{
			SVL_ASSERT( count_ == 0 );
		}

		void push(HDC hdc, window& w)
		{
			if ( count_ == items_.size() )
				items_.resize( count_ + 5 );

			item& it = items_[ count_++ ];
			it.hdc = hdc;
			it.wnd = &w;
		}

		void pop(HDC hdc)
		{
			SVL_ASSERT( items_[count_ - 1].hdc == hdc );

			while (count_ != 0)
			{
				if (items_[--count_].hdc == hdc)
					return;
			}
		}

		HDC find(window& w) const
		{
			for (unsigned pos = count_; pos != 0; )
			{
				item const& it = items_[--pos];
				if (it.wnd == &w)
					return it.hdc;
			}
			return 0;
		}
	};

	/*----------------------------------------------------------------------*/

	class com_apartment_t
	{
	private:
		bool initialized;

	public:
		com_apartment_t()
			: initialized( false )
		{}

		void initialize()
		{
			if (!initialized)
			{
				HRESULT result = ::CoInitializeEx( 0, COINIT_APARTMENTTHREADED );
				if ( result != S_OK && result != S_FALSE )
					throw win32_error("CoInitializeEx");
			}
		}

		~com_apartment_t()
		{
			if (initialized)
				::CoUninitialize();
		}
	};

	/*----------------------------------------------------------------------*/

	class timers_base
	{
	public:
		timers_base( HWND sys_window )
			: sys_window_( sys_window )
			, timers_    ( 5, static_cast<timer*>(0) )
		{
			win32::set_prop( sys_window_, timer_prop(), this );
		}

		~timers_base()
		{
			SVL_ASSERT_2( 0 == *std::max_element(timers_.begin(), timers_.end()) );
			win32::remove_prop( sys_window_, timer_prop() );
		}

		void create(timer* t, unsigned ms)
		{
			timers::iterator iter = std::find( timers_.begin(), timers_.end(), t );
			if (iter != timers_.end())
			{
				std::size_t index = std::distance( timers_.begin(), iter ) + 1;
				SVL_VERIFY( ::KillTimer( sys_window_, index ), != 0 );
				*iter = 0;
			}
			else
			{
				iter = std::find( timers_.begin(), timers_.end(), static_cast<timer*>(0) );
				if (iter == timers_.end())
				{
					timers_.push_back( 0 );
					iter = timers_.end() - 1;
				}
			}
			std::size_t index = std::distance( timers_.begin(), iter ) + 1;

			UINT_PTR ind = ::SetTimer( sys_window_, index, ms, &timers_base::w32_timer_proc );
			if (ind != index)
				throw win32_error("SetTimer");

			timers_[index - 1] = t;
		}

		void destroy(timer* t) // throw()
		{
			timers::iterator iter = std::find( timers_.begin(), timers_.end(), t );
			if (iter != timers_.end())
			{
				std::size_t index = std::distance( timers_.begin(), iter ) + 1;
				SVL_VERIFY( ::KillTimer( sys_window_, index ), != 0 );
				*iter = 0;
			}
		}

		bool is_created(timer const* t) const // throw()
		{
			return timers_.end() != std::find( timers_.begin(), timers_.end(), t );
		}

	private:
		static VOID CALLBACK w32_timer_proc(HWND hwnd, UINT, UINT_PTR idEvent, DWORD)
		{
			timers_base* base = win32::get_prop<timers_base>( hwnd, timer_prop() );
			SVL_ASSERT_2( base != 0 );
			base->timers_[ idEvent - 1 ]->notify();
		}

		static tchar const* timer_prop()
		{
			return SVL_T("SVL_Timer");
		}

		typedef std::vector<timer*> timers;
		HWND   sys_window_;
		timers timers_;
	};

	/*----------------------------------------------------------------------*/

	struct env
		: noncopyable
	{
		tstring                 app_name;
		destroy_event_processor destroy_events;
		event_processor         events;
		w32_sys_window          sys_window;
		w32_dc_handles          device_contexts;
		graphics::image         shared_image;
		com_apartment_t         com_apartment;
		timers_base             timers;

		env()
			: timers( sys_window )
		{}

		static env* instance() // throw()
		{
			SVL_SHARED_PTR<env>* obj = g_tls.get();
			return obj == 0 ? 0 : obj->get();
		}

		static SVL_SHARED_PTR<env>& shared_instance()
		{
			SVL_SHARED_PTR<env>* obj = g_tls.get();
			if (obj == 0)
				throw thread_error("вызов из потока, в котором не инициализировано окружение SVL");
			return *obj;
		}
	};

	/*----------------------------------------------------------------------*/

	void add_event_handler(event::target* t, event_id eid, event_handler* eh, env& e) // own event_handler
	{
		e.events.add(t, eid, eh);
	}

	void remove_all_event_handlers(event::target* t, detail::event_id id, env& e)
	{
		e.events.remove_all(t, id);
	}

	void add_destroy_handler(event::target* t, destroy_handler* dh, env& e)
	{
		e.destroy_events.add(t, dh);
	}

	void remove_destroy_handler(event::target* t, void* obj, env& e) // throw()
	{
		e.destroy_events.remove(t, obj);
	}
}}

//////////////////////////////////////////////////////////////////////////

namespace svl {
namespace event
{
	target::target()
		: env_( detail::env::shared_instance() )
	{
		SVL_IN_DEBUG_2( start_destruction_call_ = false; )
	}

	target::connector_info_t target::connection_info()
	{
		connector_info_t cd = { this };
		return cd;
	}

	target::destroy_connector target::on_destroy()
	{
		return connection_info();
	}

	void target::start_destruction() // throw()
	{
		SVL_IN_DEBUG_2( start_destruction_call_ = true; )

		env_->destroy_events.notify( this );
		env_->events.remove_all( this );
	}

	detail::env& target::get_env() const
	{
		return *env_;
	}

	target::~target()
	{
		//start_destruction();
		SVL_ASSERT_2( start_destruction_call_ );
	}

	void target::do_handle_event(processor& p)
	{
		p.processed_ |= get_env().events.handle(&p.target_, p.event_id_, p.event_);
	}

	inline bool target::handle_event(entity& e, detail::event_id id)
	{
		processor p( *this, e, id );
		do_handle_event( p );
		return p.processed();
	}


	/*----------------------------------------------------------------------*/

	connection::connection()
		: target_(0)
		, handler_(0)
	{}

	connection::~connection()
	{
		disconnect();
	}

	bool connection::is_active()
	{
		if (target_ == 0)
			return false;

		if (target_->get_env().events.is_handler_exist(target_, handler_))
			return true;

		target_->on_destroy().remove(this);
		target_ = 0;
		return false;
	}

	void connection::disconnect() // throw()
	{
		if (target_ != 0)
		{
			target_->get_env().events.remove(target_, handler_);
			target_->on_destroy().remove(this);
			target_ = 0;
		}
	}

	void connection::connect(target& t, detail::event_handler* eh)
	{
		disconnect();

		t.on_destroy().add(&connection::on_target_destroy, this);
		target_ = &t;
		handler_ = eh;
	}

	void connection::on_target_destroy(target*) // throw()
	{
		target_ = 0;
	}
}}

/*----------------------------------------------------------------------*/

namespace svl
{
	environment::environment(str_ref app_name)
		: env_( new detail::env )
	{
		env_->app_name.assign( app_name.data(), app_name.size() );
		g_tls.set( &env_ );
	}

	environment::~environment()
	{
		g_tls.set( 0 );
	}

	void run_msg_loop( environment& )
	{
		for (;;)
		{
			MSG msg;
			int rv = static_cast<int>(SVL_MS(GetMessage)( &msg, 0, 0, 0 ));
			if (rv <= 0)
			{
				if (msg.message == WM_QUIT)
					::PostQuitMessage( static_cast<int>(msg.wParam) );
				if (rv < 0)
					throw win32_error( "GetMessage" );
				break;
			}

			::TranslateMessage( &msg );
			SVL_MS(DispatchMessage)( &msg );
		}
	}

	bool process_messages()
	{
		unsigned const max_msgs_per_call = 100;
		MSG msg;
		for (unsigned i = 0; i < max_msgs_per_call && SVL_MS(PeekMessage)( &msg, 0, 0, 0, PM_REMOVE ) != 0; i++)
		{
			if (msg.message == WM_QUIT)
			{
				::PostQuitMessage( static_cast<int>(msg.wParam) );
				return false;
			}

			::TranslateMessage( &msg );
			SVL_MS(DispatchMessage)( &msg );
		}

		return true;
	}

	string module_filename()
	{
		string str( 1023 );
		std::size_t len = SVL_MS(GetModuleFileName)( win32::module_handle(), str.data(), static_cast<int>( str.capacity() ) );
		if (len == 0)
			throw win32_error( "GetModuleFileName" );
		return str.resize( len );
	}

	string module_name()
	{
		string str = module_filename();
		tchar* data = str.data();
		for (unsigned i = static_cast<unsigned>( str.size() ); i != 0;)
		{
			tchar c = data[--i];
			if (c == SVL_T('/') || c == SVL_T('\\'))
			{
				std::size_t len = str.size() - i - 1;
				tchar* pdata = data + i + 1;
				std::copy( pdata, pdata + len, data );
				data[len] = 0;
				return str.resize(len);
			}
		}
		return str;
	}
	
	string module_path()
	{
		string str = module_filename();
		tchar* data = str.data();
		for (unsigned i = static_cast<unsigned>( str.size() ); i != 0;)
		{
			tchar c = data[--i];
			if (c == SVL_T('/') || c == SVL_T('\\'))
			{
				data[i] = 0;
				return str.resize(static_cast<int>(i));
			}
		}
		return str;
	}

	string app_name()
	{
		detail::env* env = detail::env::instance();

		if (env == 0 || env->app_name.empty())
			return module_name();

		tstring const& app_name = env->app_name;
		std::size_t len = app_name.size();
		svl::string str( len );
		std::copy( app_name.begin(), app_name.end(), str.data() );
		return str.resize( len );
	}

	void post_exit_app_event()
	{
		::PostQuitMessage(0);
	}
}
