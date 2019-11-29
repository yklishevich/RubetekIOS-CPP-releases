
#ifndef SVL_BASE_STRING_HPP
#define SVL_BASE_STRING_HPP

#include <svl/detail/types.hpp>

namespace svl
{
	class string
	{
	public:
		// !!! все размеры указываются в элементах tchar без учета завершающего 0 (за исключением функции capacity())
		explicit string(std::size_t max_str_size = 1000);
		string(string const&);
		string& operator = (string const&);
		~string();

		std::size_t capacity() const; // включает завершающий 0
		tchar* data();
		tchar const* data() const;

		std::size_t size() const;

		// str_size <= max_str_size
		string& resize(std::size_t str_size);
		string& resize(int str_size);

		string& auto_resize(); // автоматически устанавливает размер строки по завершающему нулю

		operator std::string() const;
		operator std::wstring() const;

		std::string  as_string() const;
		std::wstring as_wstring() const;

		operator safe_bool() const; // возвращает true, если размер строки ненулевой

#if SVL_USE_BOOST_LEXICAL_CAST
		template <typename T> T as() const        { return boost::lexical_cast<T>( as_string() ); }
#endif
	private:
		detail::str_data* data_;
	};

	/*----------------------------------------------------------------------*/

	class string_reference
		: noncopyable
	{
	public:
		string_reference(char const*);
		string_reference(wchar_t const*);
		string_reference(svl::string const&);
		string_reference(std::string const&);
		string_reference(std::wstring const&);
		string_reference(detail::cstr_ref<char> const&);
		string_reference(detail::cstr_ref<wchar_t> const&);
#if SVL_USE_BOOST_FORMAT
		string_reference(boost::format const&);
#endif
#if SVL_USE_BOOST_LEXICAL_CAST
		template <typename T>
		string_reference(detail::value_ref<T> const& v)       { std::string str = boost::lexical_cast<std::string>(v.val); init(str.c_str(), str.size()); }
#endif
		~string_reference();

		tchar const* data() const;
		std::size_t size() const;

	private:
		void init(char const*, std::size_t);
		void init(wchar_t const*, std::size_t);
		void init_ref(char const*, std::size_t);
		void init_ref(wchar_t const*, std::size_t);

		tchar const* str_;
		std::size_t size_    : 31;
		std::size_t to_free_ : 1;
	};

	/*----------------------------------------------------------------------*/

	typedef string_reference const& str_ref;

#if SVL_USE_BOOST_LEXICAL_CAST
	template <typename T>
	detail::value_ref<T>      to_str(T const& v)                           { detail::value_ref<T> vref; vref.val = v; return vref; }
#endif
	detail::cstr_ref<char>    to_str(char const*    str, std::size_t size);
	detail::cstr_ref<wchar_t> to_str(wchar_t const* str, std::size_t size);

	/*----------------------------------------------------------------------*/

	std::size_t convert_string(char const* in, std::size_t in_size, wchar_t* out, std::size_t out_max_size);
	std::size_t convert_string(wchar_t const* in, std::size_t in_size, char* out, std::size_t out_max_size);
}

#endif // SVL_BASE_STRING_HPP
