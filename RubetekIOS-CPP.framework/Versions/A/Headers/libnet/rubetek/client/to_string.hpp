#pragma once

#include <string>

#include <rubetek/client/project_error.hpp>
#include <rubetek/client/controller_error.hpp>
#include <rubetek/client/controller_availability.hpp>
#include <rubetek/client/server_connection_status.hpp>


namespace rubetek {
namespace client  {

    std::string to_string( project_error            );
    std::string to_string( controller_error         );
    std::string to_string( controller_availability  );
    std::string to_string( server_connection_status );

}}


#ifdef RUBETEK_HEADER_ONLY
#   include <rubetek/client/impl/to_string.cpp>
#endif
