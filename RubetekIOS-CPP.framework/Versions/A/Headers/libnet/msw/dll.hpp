#pragma once 

#include <string>

#include <msw/config.hpp>
#include <msw/noncopyable.hpp>
#include <msw/system_error/throw.hpp>

#ifdef MSW_WINAPI_DLL
#   include <msw/include/windows.hpp>
#else
#   include <dlfcn.h>
#endif


namespace msw {
namespace dll {

    typedef
    #ifdef MSW_WINAPI_DLL
        HMODULE
    #else
        void*
    #endif
        handle;

    struct module
        : noncopyable
    {
                               explicit module        (std::string const& name)               ;
                               ~        module        ()                                      ;

        template <typename Fn> void     link_function (Fn& fn, std::string const& name) const ;

    private:
        handle h_;
    };

    handle load             (char const* name)         ;
    bool   free             (handle)                   ;
    void*  function_address (handle, char const* name) ;

}}


namespace msw {
namespace dll {

    inline module::module(std::string const& name)
        : h_(load(name.c_str()))
    {
        if (!h_)
        {
            system_error::throw_exception(
#ifdef MSW_WINAPI_DLL
                "can't load library '" + name + "'"
#else
                dlerror()
#endif
                );
        }
    }
    inline module::~module()
    {
        free(h_);
    }

    template <typename Fn>
    void module::link_function(Fn& fn, std::string const& name) const
    {
        fn = reinterpret_cast<Fn>(function_address(h_, name.c_str()));
        if (!fn)
        {
            system_error::throw_exception(
#ifdef MSW_WINAPI_DLL
                "can't find function '" + name + "'"
#else
                dlerror()
#endif
                );
        }
    }

    inline handle load(char const* name)
    {
        return
#ifdef MSW_WINAPI_DLL
            ::LoadLibraryA(name);
#else
            ::dlopen(name, RTLD_NOW);
#endif
    }
    inline bool free(handle h)
    {
        return
#ifdef MSW_WINAPI_DLL
            ::FreeLibrary(h) != 0;
#else
            ::dlclose(h) == 0;
#endif
    }
    inline void* function_address(handle h, char const* name)
    {
        return reinterpret_cast<void*>(
#ifdef MSW_WINAPI_DLL
            ::GetProcAddress
#else
            ::dlsym
#endif
            (h, name));
    }

}}
