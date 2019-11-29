#include <string>

#include <rubetek/config.hpp>
#include <rubetek/client/detect_operation_system.hpp>
#ifndef RUBETEK_HEADER_ONLY
#   include <rubetek/client/device_info.hpp>
#endif


namespace rubetek {
namespace client  {

    RUBETEK_INLINE_OF_IMPL device_info::device_info()
        : operation_system(detect_operation_system())
    {}
    RUBETEK_INLINE_OF_IMPL device_info::device_info
    (
          std::string const& name
        , std::string const& operation_system
        , std::string const& language
        , std::string const& client_version
        , std::string const& notification_id
        , std::string const& operation_system_version
    )
        : name                     ( name                     )
        , operation_system         ( operation_system         )
        , language                 ( language                 )
        , client_version           ( client_version           )
        , notification_id          ( notification_id          )
        , operation_system_version ( operation_system_version )
    {}

}}
