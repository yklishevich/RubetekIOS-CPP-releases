#pragma once

#include <string>

#include <rubetek/config.hpp>


namespace rubetek {
namespace client  {

    struct device_info
    {
        std::string name                     ;
        std::string operation_system         ;
        std::string language                 ;
        std::string client_version           ;
        std::string notification_id          ;
        std::string operation_system_version ;

        device_info();
        device_info
        (
              std::string const& name
            , std::string const& operation_system
            , std::string const& language
            , std::string const& client_version
            , std::string const& notification_id
            , std::string const& operation_system_version
        );
    };

}}


#ifdef RUBETEK_HEADER_ONLY
#   include <rubetek/client/impl/device_info.cpp>
#endif
