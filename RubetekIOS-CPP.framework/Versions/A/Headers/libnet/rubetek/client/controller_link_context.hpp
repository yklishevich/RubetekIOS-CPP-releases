#pragma once

#include <string>

#include <rubetek/config.hpp>
#include <rubetek/client/device_info.hpp>


namespace rubetek {
namespace client  {

    struct controller_link_context
    {
        std::string work_path      ;
        device_info device         ;
        std::string server_address ;
        bool        use_ssl        ;

        controller_link_context();
        controller_link_context
        (
              std::string const& work_path
            , device_info const& device_info
            , std::string const& server_address
            , bool               use_ssl
        );
    };

}}


#ifdef RUBETEK_HEADER_ONLY
#   include <rubetek/client/impl/controller_link_context.cpp>
#endif
