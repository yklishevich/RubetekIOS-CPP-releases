#pragma once

#include <string>
#include <cassert>

#include <msw/format.hpp>

#include <rubetek/module/log/level.hpp>
#include <rubetek/module/log/interface.hpp>


namespace rubetek {
namespace module  {
namespace log     {

#define RUBETEK_MODULE_LOG(L)\
    void L(std::string const&);\
    template <typename ...Args>\
    void L(Args&&...);
    RUBETEK_MODULE_LOG( debug    )
    RUBETEK_MODULE_LOG( info     )
    RUBETEK_MODULE_LOG( warning  )
    RUBETEK_MODULE_LOG( error    )
    RUBETEK_MODULE_LOG( critical )
#undef RUBETEK_MODULE_LOG

}}}


namespace rubetek {
namespace module  {
namespace log     {

    inline log_controller_link& logger()
    {
        static log_controller_link log_controller_link_;
        return log_controller_link_;
    }
    inline log_controller_link& get_logger()
    {
        assert(logger());
        return logger();
    }
    inline void init_logger(log_controller_link link)
    {
        assert(!logger());
        logger() = link;
    }

#define RUBETEK_MODULE_LOG(L)\
    inline void L(std::string const& msg)\
    {\
        get_logger()->write(msg.c_str(), level::L);\
    }\
    template <typename ...Args>\
    void L(Args&&... args)\
    {\
        get_logger()->write(msw::format(std::forward<Args>(args)...).c_str(), level::L);\
    }
    RUBETEK_MODULE_LOG( debug    )
    RUBETEK_MODULE_LOG( info     )
    RUBETEK_MODULE_LOG( warning  )
    RUBETEK_MODULE_LOG( error    )
    RUBETEK_MODULE_LOG( critical )
#undef RUBETEK_MODULE_LOG

}}}
