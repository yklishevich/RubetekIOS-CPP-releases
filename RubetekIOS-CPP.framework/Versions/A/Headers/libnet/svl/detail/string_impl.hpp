
namespace svl {
namespace 
{
	detail::str_data* add_ref(detail::str_data* d)
	{
		d->refs++;
		return d;
	}

	void remove_ref(detail::str_data*& d)
	{
		if (--d->refs == 0)
		{
			delete[] reinterpret_cast<char*>( d );
			SVL_IN_DEBUG_2( d = 0; )
		}
	}
}}

namespace svl
{
	string::string(std::size_t str_size)
	{
		char* d = new char[ sizeof(detail::str_data) + str_size * sizeof(tchar) ]; // завершающий ноль внутри tchar[1]
		data_ = reinterpret_cast<detail::str_data*>(d);

		data_->refs = 1;
		data_->capacity = str_size + 1;
		data_->size = 0;
		data_->str[0] = 0;
	}

	string::string(string const& other)
		: data_( add_ref(other.data_) )
	{}

	string& string::operator = (string const& other)
	{
		if (&other != this)
		{
			remove_ref(data_);
			data_ = add_ref( other.data_ );
		}
		return *this;
	}

	string::~string()
	{
		remove_ref(data_);
	}

	std::size_t string::capacity() const
	{
		return data_->capacity;
	}

	tchar* string::data()
	{
		return data_->str;
	}

	tchar const* string::data() const
	{
		return data_->str;
	}

	std::size_t string::size() const
	{
		return data_->size;
	}

	string& string::resize(std::size_t str_size)
	{
		SVL_ASSERT( str_size < data_->capacity );

		data_->size = static_cast<unsigned>(str_size);
		data_->str[ str_size ] = 0;

		return *this;
	}

	string& string::resize(int str_size)
	{
		SVL_ASSERT( 0 <= str_size );

		return resize( static_cast<std::size_t>(str_size) );
	}

	string& string::auto_resize()
	{
		data_->str[ data_->capacity - 1 ] = 0;
		data_->size = std::char_traits<tchar>::length( data_->str );
		return *this;
	}

	string::operator std::string() const
	{
		return as_string();
	}

	string::operator std::wstring() const
	{
		return as_wstring();
	}

#if SVL_UNICODE
	std::string string::as_string() const
	{
		if (data_->size == 0)
			return std::string();

		std::vector<char> vec( data_->size * 2 );
		std::size_t sz = convert_string( data_->str, data_->size, &vec.front(), vec.size() );

		return std::string( &vec.front(), sz );
	}

	std::wstring string::as_wstring() const
	{
		return std::wstring( data_->str, data_->size );
	}
#else
	std::string string::as_string() const
	{
		return std::string( data_->str, data_->size );
	}

	std::wstring string::as_wstring() const
	{
		if (data_->size == 0)
			return std::wstring();

		std::vector<wchar_t> vec( data_->size );
		std::size_t sz = convert_string( data_->str, data_->size, &vec.front(), vec.size() );

		return std::wstring( &vec.front(), sz );
	}
#endif

	string::operator safe_bool() const
	{
		return safe_bool_value( data_->size != 0 );
	}

	/*----------------------------------------------------------------------*/

#if SVL_UNICODE
	void string_reference::init(char const* str, std::size_t size)
	{
		wchar_t* new_str = new wchar_t[size + 1];
		size_ = convert_string(str, size, new_str, size);
		new_str[size_] = 0;
		str_ = new_str;
		to_free_ = true;
	}

	void string_reference::init(wchar_t const* str, std::size_t size)
	{
		wchar_t* new_str = new wchar_t[size + 1];
		memcpy(new_str, str, size * sizeof(wchar_t));
		new_str[size] = 0;

		str_ = new_str;
		size_ = size;
		to_free_ = true;
	}

	void string_reference::init_ref(char const* str, std::size_t size)
	{
		init(str, size);
	}

	void string_reference::init_ref(wchar_t const* str, std::size_t size)
	{
		str_ = str;
		size_ = size;
		to_free_ = false;
	}
#else
	void string_reference::init(char const* str, std::size_t size)
	{
		char* new_str = new char[size + 1];
		memcpy(new_str, str, size * sizeof(char));
		new_str[size] = 0;

		str_ = new_str;
		size_ = size;
		to_free_ = true;
	}

	void string_reference::init(wchar_t const* str, std::size_t size)
	{
		std::size_t new_size = 2 * size;
		char* new_str = new char[new_size + 1];
		size_ = convert_string(str, size, new_str, new_size);
		new_str[size_] = 0;
		str_ = new_str;
		to_free_ = true;
	}

	void string_reference::init_ref(char const* str, std::size_t size)
	{
		str_ = str;
		size_ = size;
		to_free_ = false;
	}

	void string_reference::init_ref(wchar_t const* str, std::size_t size)
	{
		init(str, size);		
	}
#endif

	string_reference::string_reference(char const* str)
	{
		std::size_t size = std::char_traits<char>::length( str );
		init_ref( str, size );
	}

	string_reference::string_reference(wchar_t const* str)
	{
		std::size_t size = std::char_traits<wchar_t>::length( str );
		init_ref( str, size );
	}

	string_reference::string_reference(svl::string const& str)
	{
		init_ref( str.data(), str.size() );
	}

	string_reference::string_reference(std::string const& str)
	{
		init_ref( str.c_str(), str.size() );
	}

	string_reference::string_reference(std::wstring const& str)
	{
		init_ref( str.c_str(), str.size() );
	}

	string_reference::string_reference(detail::cstr_ref<char> const& str)
	{
		init(str.str, str.size);
	}

	string_reference::string_reference(detail::cstr_ref<wchar_t> const& str)
	{
		init(str.str, str.size);
	}

#if SVL_USE_BOOST_FORMAT
	string_reference::string_reference(boost::format const& frmt)
	{
		std::string str = frmt.str();
		init( str.c_str(), str.size() );
	}
#endif

	string_reference::~string_reference()
	{
		if (to_free_)
			delete[] str_;
	}

	tchar const* string_reference::data() const
	{
		return str_;
	}

	std::size_t string_reference::size() const
	{
		return size_;
	}

	/*----------------------------------------------------------------------*/

	detail::cstr_ref<char> to_str(char const* str, std::size_t size)
	{
		detail::cstr_ref<char> sref;
		sref.str = str;
		sref.size = size;
		return sref;
	}

	detail::cstr_ref<wchar_t> to_str(wchar_t const* str, std::size_t size)
	{
		detail::cstr_ref<wchar_t> sref;
		sref.str = str;
		sref.size = size;
		return sref;
	}

	/*----------------------------------------------------------------------*/

#if !SVL_USER_STRING_CONVERSION

	inline std::size_t convert_string(char const* in, std::size_t in_size, wchar_t*  out, std::size_t out_max_size)
	{
		return ::MultiByteToWideChar(
			CP_ACP,
			MB_PRECOMPOSED,
			in,
			static_cast<int>(in_size),
			out,
			static_cast<int>(out_max_size));
	}

	inline std::size_t convert_string(wchar_t const* in, std::size_t in_size, char* out, std::size_t out_max_size)
	{
		BOOL used_default_char;
		return ::WideCharToMultiByte(
			CP_ACP,
			WC_COMPOSITECHECK,
			in,
			static_cast<int>(in_size),
			out,
			static_cast<int>(out_max_size),
			0,
			&used_default_char);
	}

#endif // SVL_USER_STRING_CONVERSION
}
