#ifndef SVL_ENVIRONMENT_HPP
#define SVL_ENVIRONMENT_HPP

#include <svl/detail/types.hpp>
#include <svl/base/string.hpp>

namespace svl
{
	class environment
		: noncopyable
	{
	public:
		environment(str_ref app_name = SVL_T(""));
		~environment();

	private:
		SVL_SHARED_PTR<detail::env> env_;
	};


	void run_msg_loop( environment& );
	bool process_messages();

	string app_name();
	string module_path();
	string module_name();

	void post_exit_app_event();
}

#endif //SVL_ENVIRONMENT_HPP
