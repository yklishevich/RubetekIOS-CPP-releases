#pragma once

#include <string>

#include <syslog.h>

#include <msw/noncopyable.hpp>


namespace dawn
{
    struct sys_log
        : msw::noncopyable
    {
        explicit sys_log  (std::string const&, int facility) ;
                 ~sys_log ()                                 ;
        void     push     (int priority, std::string const&) ;
    private:
        std::string const name_;
    };
}


namespace dawn
{
    inline sys_log::sys_log(std::string const& name, int facility)
        : name_(name)
    {
        ::openlog(name_.c_str(), LOG_PID, facility);
    }
    inline sys_log::~sys_log()
    {
        ::closelog();
    }
    inline void sys_log::push(int priority, std::string const& msg)
    {
        ::syslog(priority, "%s", msg.c_str());
    }
}
