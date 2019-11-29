#pragma once

#include <rubetek/config.hpp>

#ifdef SMART_HOUSE_WINDOWS
	#include <windows.h>
#else
	#include <dlfcn.h>
#endif

#include <string>

#include <rubetek/utility/noncopyable.hpp>


namespace rubetek {
namespace dll {

	typedef
	#ifdef SMART_HOUSE_WINDOWS
		HMODULE
	#else
		void*
	#endif
		handle;

	struct module
		: utility::noncopyable
	{
								explicit	module			(std::string const& name)					;
								~			module			()											;

		template <typename Fn>	void		link_function	(Fn& fn, std::string const& name)	const	;

								void		upload			()											;

	private:
		handle h_;
	};

	handle	load				(std::string const& name)			;
	void	free				(handle)							;
	void*	function_address	(handle, std::string const& name)	;

}}


namespace rubetek {
namespace dll {

	inline module::module(std::string const& name)
		: h_(load(name))
	{}
	inline module::~module()
	{
		try
		{
			upload();
		}
		catch (...)
		{
			assert(0);
		}
	}

	template <typename Fn>
	void module::link_function(Fn& fn, std::string const& name) const
	{
		if (!h_) throw std::runtime_error("mudule already uploaded");
		fn = reinterpret_cast<Fn>(function_address(h_, name));
	}

	void inline module::upload()
	{
		if (!h_) throw std::runtime_error("mudule already uploaded");
		//NO_RELOAD//free(h_);
		h_ = 0;
	}


	inline handle load(std::string const& name)
	{
		handle h =
		#ifdef SMART_HOUSE_WINDOWS
			LoadLibraryA(name.c_str());
		#else
			dlopen(name.c_str(), RTLD_NOW);
		#endif
		if (!h)
		{
			std::string const error_description =
			#ifdef SMART_HOUSE_WINDOWS
				"<" + name + "> error code: " + std::to_string(GetLastError());
			#else
				dlerror();
			#endif
			throw std::runtime_error("can't load library " + error_description);
		}
		return h;
	}
	inline void free(handle h)
	{
		bool success =
		#ifdef SMART_HOUSE_WINDOWS
			FreeLibrary(h) != 0;
		#else
			dlclose(h) == 0;
		#endif
		if (!success)
		{
			std::string const error_description =
			#ifdef SMART_HOUSE_WINDOWS
				"error code: " + std::to_string(GetLastError());
			#else
				dlerror();
			#endif
			throw std::runtime_error("can't upload library " + error_description);
		}
	}
	inline void* function_address(handle h, std::string const& name)
	{
		void* fn =
		#ifdef SMART_HOUSE_WINDOWS
			GetProcAddress
		#else
			dlsym
		#endif
		(h, name.c_str());
		if (!fn) throw std::runtime_error("can't find function: " + name);
		return fn;
	}

}}
