#ifndef SVL_BASE_EVENTS_HPP
#define SVL_BASE_EVENTS_HPP

#include <svl/detail/types.hpp>

namespace svl
{
	struct mouse_key_state
	{
		bool is_pressed() const;
		bool just_changed() const;
		bool just_pressed() const;
		bool just_released() const;

	public:  mouse_key_state(unsigned state);
	private: unsigned state_;
	};
	
	struct mouse_key_info
	{
		point cursor() const;
		mouse_key_state left() const;
		mouse_key_state middle() const;
		mouse_key_state right() const;
		bool shift_pressed() const;
		bool ctrl_pressed() const;

		unsigned state() const;
		
	public:  mouse_key_info(point const&, unsigned state);
	private: point point_; unsigned state_;	
	};
}

namespace svl {
namespace event
{
	struct entity { SVL_CAST_TYPE(entity) };


	struct resize: entity {};
	struct paint: entity {};
	struct click: entity {};
	struct move: entity {};
	struct get_focus: entity {};
	struct lose_focus: entity {};
	struct timer: entity {};


	struct close: entity
	{
		struct action {	enum type
		{
			do_nothing,
			hide_form,
			exit_app
		};};

		action::type get_action() const;
		void set_action(action::type);

	public:  close();
	private: action::type action_;
	};


	struct activate: entity
	{
		bool active() const;

	public:  activate(bool);
	private: bool active_;
	};


	struct mouse_move: entity
		, mouse_key_info
	{
	public: mouse_move(point const&, unsigned state);
	};
	
	
	struct mouse_key_down: entity
		, mouse_key_info
	{
	public: mouse_key_down(point const&, unsigned state);
	};
	
	
	struct mouse_key_up: entity
		, mouse_key_info
	{
	public: mouse_key_up(point const&, unsigned state);
	};

	struct mouse_enter: entity
		, mouse_key_info
	{
	public: mouse_enter(point const&, unsigned state);
	};

	struct mouse_leave: entity
	{};


	struct key_down: entity
	{
		unsigned win32_key() const;

	public:  key_down(unsigned key);
	private: unsigned key_;
	};


	struct select: entity
	{
		size_t index() const;

	public:  select(size_t ind);
	private: size_t index_;
	};

	struct check: entity
	{
		tribool checked() const;

	public:  check(tribool);
	private: tribool checked_;
	};

	struct text_change: entity
	{
		string text() const;

	public:  text_change(unsigned, HWND);
	private: unsigned t_; HWND h_;
	};

	/*----------------------------------------------------------------------*/

	class target;


	class processor
		: noncopyable
	{
	public:
		template <class Event, class Target>
		void operator () (void (Target::*)(Event&));

		bool processed() const;

	private:
		friend class target;

		processor(target&, entity&, detail::event_id);

		target&          target_;
		detail::event_id event_id_;
		entity&          event_;
		bool             processed_;
	};

	/*----------------------------------------------------------------------*/

	class target
		: noncopyable
	{
	public:
		virtual ~target();
		template <class Event> class connector;
		class destroy_connector;

	protected:
		target();
		template <class Event> bool handle_event(Event& e);
		bool handle_event(entity&, detail::event_id);
		virtual void do_handle_event(processor&);

		destroy_connector on_destroy();
		void start_destruction(); // throw()

		struct connector_info_t;
		connector_info_t connection_info();

		detail::env& get_env() const;

	private:
		friend class connection;

		SVL_SHARED_PTR<detail::env> env_;
		SVL_IN_DEBUG_2( bool start_destruction_call_; )
	};

	/*----------------------------------------------------------------------*/

	class connection
		: noncopyable
	{
	public:
		connection();
		~connection();
		void disconnect(); // throw()
		bool is_active(); // throw()

	private:
		template <class> friend class target::connector;
		void connect(target&, detail::event_handler*);
		void on_target_destroy(target*); // throw()

		target* target_;
		detail::event_handler* handler_;
	};

	/*----------------------------------------------------------------------*/

	template <class Event> class target::connector // default copyable
	{
	public:
		connector(target::connector_info_t const&);

		template<class F> connector& add(F const&); // F - функтор вида void (Event&)
		template<class T> connector& add(void (T::*)(), T*);
		template<class T> connector& add(void (T::*)(Event&), T*);

		template<class F> connector& add(F const&, connection&); // F - функтор вида void (Event&)
		template<class T> connector& add(void (T::*)(), T*, connection&);
		template<class T> connector& add(void (T::*)(Event&), T*, connection&);

		template<class F> connector& operator+=(F const&); // F - функтор вида void (Event&)
		template<class F> connector& operator|=(F const&); // F - функтор вида void ()

	private:
		target* target_;
	};

	/*----------------------------------------------------------------------*/

	class target::destroy_connector // default copyable
	{
	public:
		destroy_connector(target::connector_info_t const&);

		template <class T> destroy_connector& add(void (T::*)(target*) /*throw()*/, T*);
		template <class T> destroy_connector& remove(T*); // throw()

	private:
		target* target_;
	};
}}

#include <svl/detail/events_incl.hpp>

#endif // SVL_BASE_EVENTS_HPP
