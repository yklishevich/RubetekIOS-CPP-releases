#pragma once

#include <string>

#include <msw/throw_runtime_error.hpp>

#include <rubetek/config.hpp>
#include <rubetek/client/project_error.hpp>
#include <rubetek/client/controller_error.hpp>
#include <rubetek/client/controller_availability.hpp>
#include <rubetek/client/server_connection_status.hpp>
#ifndef RUBETEK_HEADER_ONLY
#   include <rubetek/client/to_string.hpp>
#endif


namespace rubetek {
namespace client  {

    RUBETEK_INLINE_OF_IMPL std::string to_string(project_error e)
    {
        if ( e == project_error::no_on_server           ) return "no on server"           ;
        if ( e == project_error::no_on_controller       ) return "no on controller"       ;
        if ( e == project_error::no_required_resolution ) return "no required resolution" ;
        msw::throw_runtime_error("unknown project error: ", static_cast<msw::uint>(e));
#include <msw/throw_hide_warning.hpp>
    }

    RUBETEK_INLINE_OF_IMPL std::string to_string(controller_error e)
    {
        if ( e == controller_error::internal        ) return "internal"        ;
        if ( e == controller_error::update_required ) return "update required" ;
        msw::throw_runtime_error("unknown controller error: ", static_cast<msw::uint>(e));
#include <msw/throw_hide_warning.hpp>
    }

    RUBETEK_INLINE_OF_IMPL std::string to_string(controller_availability s)
    {
        if ( s == controller_availability::none   ) return "none"   ;
        if ( s == controller_availability::local  ) return "local"  ;
        if ( s == controller_availability::remote ) return "remote" ;
        msw::throw_runtime_error("unknown controller availability: ", static_cast<msw::uint>(s));
#include <msw/throw_hide_warning.hpp>
    }

    RUBETEK_INLINE_OF_IMPL std::string to_string(server_connection_status s)
    {
        if ( s == server_connection_status::connecting  ) return "connecting"  ;
        if ( s == server_connection_status::connected   ) return "connected"   ;
        if ( s == server_connection_status::no_internet ) return "no internet" ;
        msw::throw_runtime_error("unknown server connection status: ", static_cast<msw::uint>(s));
#include <msw/throw_hide_warning.hpp>
    }

}}
