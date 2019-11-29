#include <string>

#include <msw/config.hpp>

#include <rubetek/config.hpp>
#ifndef RUBETEK_HEADER_ONLY
#   include <rubetek/client/detect_operation_system.hpp>
#endif


namespace rubetek {
namespace client  {

    RUBETEK_INLINE_OF_IMPL std::string detect_operation_system()
    {
        return
#ifdef  MSW_WINDOWS
            "windows"
#elif   MSW_ANDROID
            "android"
#elif   MSW_APPLE
            "ios"
#elif   MSW_LINUX
            "linux"
#endif
        ;
    }

}}
