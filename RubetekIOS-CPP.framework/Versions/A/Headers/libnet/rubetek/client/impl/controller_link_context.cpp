#include <string>

#include <rubetek/config.hpp>
#ifndef RUBETEK_HEADER_ONLY
#   include <rubetek/client/controller_link_context.hpp>
#endif


namespace rubetek {
namespace client  {

    RUBETEK_INLINE_OF_IMPL controller_link_context::controller_link_context()
        : use_ssl(true)
    {}
    RUBETEK_INLINE_OF_IMPL controller_link_context::controller_link_context
    (
          std::string const& work_path
        , device_info const& device
        , std::string const& server_address
        , bool               use_ssl
    )
        : work_path      ( work_path      )
        , device         ( device         )
        , server_address ( server_address )
        , use_ssl        ( use_ssl        )
    {}

}}
