#pragma once

#include <dawn/app/log_settings.hpp>


namespace rubetek
{
    struct log_settings
        : dawn::app::log_settings
    {
        log_io_mode log_io_ihlp  ;
        log_io_mode log_io_ihmlp ;

        log_settings()
            : log_io_ihlp  (log_io_mode::disable)
            , log_io_ihmlp (log_io_mode::disable)
        {}
    };
}
