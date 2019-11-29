#pragma once

#include <msw/config.hpp>
#include <msw/std/thread.hpp>
#include <msw/noncopyable.hpp>
#include <msw/dimension/trigger.hpp>
#include <msw/system_signal/signal.hpp>
#include <msw/entry_point/exception_handler/default_ex.hpp>


namespace msw         {
namespace application {

    template <typename App>
    struct wrapper
        : noncopyable
    {
        template <typename ...Args>
        explicit wrapper(Args&&... args)
            : app_  ( std::forward<Args>(args)... )
            , exec_ ( true                        )
            , thread_
            (
                [this]()
                {
                    execute();
                }
            )
        {}
#ifdef MSW_GCC
        wrapper(wrapper<App> const&)
        {}
#endif

        ~wrapper()
        {
            if (exec_) thread_.detach ();
            else       thread_.join   ();
        }
        void process_signal(system_signal s)
        {
            app_.process_signal(s);
        }
        bool executing() const
        {
            return static_cast<bool>(exec_);
        }
    private:
        void execute()
        {
            try
            {
                app_.execute();
            }
            catch (std::exception const& e)
            {
                entry_point::default_exception_handler_ex(e, "application");
            }
            catch (...)
            {
                entry_point::default_unknown_exception_handler_ex("application");
            }
            exec_.reset();
        }

        App         app_    ;
        trigger     exec_   ;
        msw::thread thread_ ;
    };

}}
