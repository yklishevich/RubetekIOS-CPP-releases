#ifndef SVL_EXCEPTIONS_HPP
#define SVL_EXCEPTIONS_HPP

namespace svl
{
	class win32_error
		: public std::runtime_error
	{
	public:
		win32_error(std::string const& fn);
		win32_error(std::string const& fn, unsigned err);
		unsigned err_code() const;

	private:
		unsigned err_code_;
	};

	/*----------------------------------------------------------------------*/

	class error
		: public std::runtime_error
	{
	public:
		error(std::string const& msg);
	};

	/*----------------------------------------------------------------------*/

	class thread_error
		: public std::runtime_error
	{
	public:
		thread_error(std::string const& msg);
	};
}

#endif // SVL_EXCEPTIONS_HPP
