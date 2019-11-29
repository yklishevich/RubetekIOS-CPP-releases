#pragma once

#include <memory>
#include <string>
#include <type_traits>

#include <rubetek/config.hpp>

#ifdef MSW_MSVC
#   pragma warning (disable: 4503)
#endif

#include <msw/algorithm/copy.hpp>
#include <msw/make_range_native.hpp>

#include <lama/thrift/include/thrift.hpp>
#include <lama/thrift/include/protocol/tprotocol.hpp>

#include <rubetek/imc_types.hpp>
#include <rubetek/module/interface.hpp>
#include <rubetek/module/shell.hpp>
#include <rubetek/module/profile.hpp>
#include <rubetek/module/log/log.hpp>
#include <rubetek/module/version.hpp>
#include <rubetek/module/context2.hpp>
#include <rubetek/module/reload_clients_list/enable.hpp>
#include <rubetek/module/command/enable_send_command.hpp>

#include "imc_constants.h"


namespace rubetek {
namespace module  {

    version_type           version ()                                            ;
    controller_module_link init    (context_list const&, module_controller_link) ;
    void                   free    ()                                            ;

}}

inline void set_init_error(std::string const& msg, char* error, int error_len)
{
    if (error_len > static_cast<int>(msg.size()))
    {
        msw::copy(msg, msw::make_range_native(error, error_len));
        error[msg.size()] = 0;
    }
    else if (error_len) error[0] = 0;
}


extern "C" rubetek::module::version_type module_version()
{
    static_assert(std::is_pod<rubetek::module::version_type>::value, "version_type should be POD");
    try
    {
        return rubetek::module::version();
    }
    catch(...)
    {
        return rubetek::module::version_type();
    }
}

extern "C" rubetek::module::controller_module_link module_init(rubetek::module::profile const& profile, char* error, int error_len)
{
    rubetek::module::controller_module_link link = 0;
    try
    {
        rubetek::module::log::init_logger(profile.log_controller_link);
        rubetek::module::send_command_impl::controller_link = profile.controller_link;
        rubetek::module::reload_clients_list_impl::controller_link = profile.controller_link;
        link = rubetek::module::init(profile.context, profile.controller_link);
    }
    catch(std::exception const& e)
    {
        set_init_error(e.what(), error, error_len);
        link = 0;
    }
    catch(...)
    {
        set_init_error("unknown exception", error, error_len);
        link = 0;
    }
    return link;
}

extern "C" bool module_free()
{
    try
    {
        rubetek::module::free();
    }
    catch(...)
    {
        return false;
    }
    return true;
}


#define SMART_HOUSE_MODULE(NAME) \
namespace rubetek {\
namespace module {\
    typedef module_shell<NAME> module_type;\
    namespace { std::unique_ptr<module_type> m; }\
    inline version_type version()\
    {\
        return module_type::version();\
    }\
    inline controller_module_link init(context_list const& context, module_controller_link link)\
    {\
        assert(!m);\
        m.reset(new module_type(context, link));\
        return m.get();\
    }\
    inline void free()\
    {\
        assert(m);\
        m.reset();\
    }\
}}
