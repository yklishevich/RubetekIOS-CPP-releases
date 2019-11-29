#ifndef SVL_DETAIL_TYPES_HPP
#define SVL_DETAIL_TYPES_HPP

#include <svl/detail/config.hpp>

//////////////////////////////////////////////////////////////////////////

namespace svl
{
#if SVL_USE_BOOST_NONCOPYABLE
	using boost::noncopyable;
#else
	struct noncopyable
	{
	public:
		noncopyable() {}

	private:
		noncopyable(noncopyable const&);
		noncopyable& operator = (noncopyable const&);
	};
#endif

#if SVL_UNICODE
	typedef wchar_t tchar;
#else
	typedef char tchar;
#endif

	typedef std::basic_string<tchar> tstring;
}

namespace svl {
namespace detail
{
	struct str_data
	{
		unsigned refs;
		std::size_t capacity;
		std::size_t size;
		tchar str[1];
	};

#if SVL_USE_BOOST_LEXICAL_CAST
	template <typename T> struct value_ref
	{
		T val;
	};
#endif

	template <typename T> struct cstr_ref
	{
		T const* str;
		std::size_t size;
	};

	struct safe_bool
	{
		void safe_true_value() {}
	};
}}

namespace svl
{
	typedef void (detail::safe_bool::*safe_bool)();
	inline safe_bool safe_bool_value(bool b)
	{
		return b ? &detail::safe_bool::safe_true_value : 0;
	}
}

//////////////////////////////////////////////////////////////////////////
// forward declarations

namespace svl
{
	class window;
	class widget;
	class control;
	class form;
	struct rect2;
	struct range2;
	struct i_container;
	class tab_order;
}

namespace svl {
namespace graphics
{
	class canvas;
	class buffered_canvas;
}}

namespace svl {
namespace detail
{
	struct env;
	struct event_handler;
	struct destroy_handler;
	typedef unsigned event_id;
	template <typename Event> struct event_id_of;
	class tab_order;
}}

#endif // SVL_DETAIL_TYPES_HPP
