#pragma once

#include <list>
#include <string>

#include <boost/optional.hpp>

#include <rubetek/essence/unique_id/channel.hpp>
#include <rubetek/essence/unique_id/controller.hpp>
#include <rubetek/client/controller_availability.hpp>


namespace rubetek {
namespace client  {

    struct controller_info
    {
        unique_controller_id::type id                ;
        std::string                name              ;
        std::string                version           ;
        std::string                system_info       ;
        boost::optional<bool>      demo_mode         ;
        std::string                local_ip_address  ;
        std::string                remote_ip_address ;
        std::string                remote_channel_id ;
        std::string                server_address    ;
        controller_availability    accessibility     ;
        bool                       active            ;
    };
    typedef std::list<controller_info> unknown_controller_list;

    struct known_controller_info
    {
        controller_info info  ;
        std::string     alias ;
    };
    typedef std::list<known_controller_info> known_controller_list;

}}
