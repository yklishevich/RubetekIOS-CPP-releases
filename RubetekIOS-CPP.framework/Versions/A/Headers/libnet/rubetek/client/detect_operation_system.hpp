#pragma once

#include <string>

#include <rubetek/config.hpp>


namespace rubetek {
namespace client  {

    std::string detect_operation_system();

}}


#ifdef RUBETEK_HEADER_ONLY
#   include <rubetek/client/impl/detect_operation_system.cpp>
#endif
