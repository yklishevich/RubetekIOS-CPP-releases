
namespace svl {
namespace detail
{
	typedef void (*enumerate_fn)(void*, widget&);
	void enumerate_widgets(window& wnd, enumerate_fn fn, void* ptr);

	/*----------------------------------------------------------------------*/

	template <typename Functor> struct enumerator
	{
		Functor* f_;
		window*  parent_;

		enumerator(Functor& f, window& parent)
			: f_     ( &f )
			, parent_( &parent )
		{}

		static void enumerate(void* p, widget& w)
		{
			enumerator& self = *reinterpret_cast<enumerator*>( p );
			if (w.get_parent() == self.parent_)
				(*self.f_)( w );
		}
	};

	/*----------------------------------------------------------------------*/

	template <typename Functor> struct recursive_enumerator
	{
		Functor* f_;

		recursive_enumerator(Functor& f)
			: f_( &f )
		{}

		static void enumerate(void* p, widget& w)
		{
			(*reinterpret_cast<recursive_enumerator*>( p )->f_)( w );
		}
	};
}}

/*----------------------------------------------------------------------*/

namespace svl
{
	template <typename Functor> void i_container::for_each_widget(Functor f)
	{
		typedef detail::enumerator<Functor> enumerator_t;
		enumerator_t enumerator( f, dynamic_cast<window &>(*this) );
		enumerate_widgets( *do_get_window(), &enumerator_t::enumerate, &enumerator );
	}

	template <typename Functor> void i_container::recursive_for_each_widget(Functor f)
	{
		typedef detail::recursive_enumerator<Functor> enumerator_t;
		enumerator_t enumerator( f );
		enumerate_widgets( *do_get_window(), &enumerator_t::enumerate, &enumerator );
	}
}
