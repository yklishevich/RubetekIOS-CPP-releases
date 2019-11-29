#pragma once

#include <string>

#include <dawn/log/log.hpp>
#include <dawn/log/log_io_mode.hpp>


namespace dawn {
namespace app  {

    struct log_settings
    {
        log::level  log_level       ;
        bool        log_to_console  ;
        bool        log_to_syslog   ;
        int         syslog_facility ;
        std::string log_to_file     ;
        bool        log_file_append ;
        log_io_mode log_io_udp      ;
        log_io_mode log_io_tcp      ;
        log_io_mode log_io_web      ;

        log_settings()
            : log_level       ( log::level::debug    )
            , log_to_console  ( false                )
            , log_to_syslog   ( false                )
            , syslog_facility ( 0                    )
            , log_file_append ( false                )
            , log_io_udp      ( log_io_mode::disable )
            , log_io_tcp      ( log_io_mode::disable )
            , log_io_web      ( log_io_mode::disable )
        {}
    };

}}
