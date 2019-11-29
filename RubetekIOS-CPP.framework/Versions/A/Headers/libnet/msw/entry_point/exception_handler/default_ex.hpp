#pragma once

#include <msw/config.hpp>

#ifdef MSW_DEFAULT_EXCEPTION_NOTIFIER_ON_MESSAGE_BOX
    #include <msw/entry_point/exception_handler/window.hpp>
#else
    #include <msw/entry_point/exception_handler/console.hpp>
#endif


namespace msw         {
namespace entry_point {

    int default_exception_handler_ex         (std::exception const&, std::string const& thread_name) ;
    int default_unknown_exception_handler_ex (std::string const& thread_name)                        ;

}}


namespace msw         {
namespace entry_point {

    inline int default_exception_handler_ex(std::exception const& e, std::string const& thread_name)
    {
        return
#ifdef MSW_DEFAULT_EXCEPTION_NOTIFIER_ON_MESSAGE_BOX
            window_exception_handler(e, thread_name);
#else
            console_exception_handler(e, thread_name);
#endif
    }

    inline int default_unknown_exception_handler_ex(std::string const& thread_name)
    {
        return
#ifdef MSW_DEFAULT_EXCEPTION_NOTIFIER_ON_MESSAGE_BOX
        window_unknown_exception_handler(thread_name);
#else
        console_unknown_exception_handler(thread_name);
#endif
    }

}}
