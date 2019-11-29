
svl::win32_error::win32_error(std::string const& fn)
	: std::runtime_error(fn)
{
	err_code_ = ::GetLastError();
}

svl::win32_error::win32_error(std::string const& fn, unsigned err)
	: std::runtime_error(fn)
	, err_code_(err)
{}

unsigned svl::win32_error::err_code() const
{
	return err_code_;
}

//////////////////////////////////////////////////////////////////////////

svl::error::error(std::string const& msg)
	: std::runtime_error(msg)
{}

//////////////////////////////////////////////////////////////////////////

svl::thread_error::thread_error(std::string const& msg)
	: std::runtime_error(msg)
{}
