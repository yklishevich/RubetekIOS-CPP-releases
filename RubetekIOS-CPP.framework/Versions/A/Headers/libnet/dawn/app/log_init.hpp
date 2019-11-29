#pragma once

#include <msw/throw_runtime_error.hpp>

#include <dawn/log/log.hpp>
#include <dawn/log/logger.hpp>
#include <dawn/log/logger_io.hpp>
#include <dawn/log/log_io_tags.hpp>
#include <dawn/app/log_settings.hpp>


namespace dawn {
namespace app  {

    template <typename Settings>
    void log_init(Settings const& settings, std::string const& syslog_ident = "", log::config::typed_message_handler_type handler = nullptr)
    {
        if (settings.log_to_syslog && syslog_ident.empty()) msw::throw_runtime_error("log to syslog fail: identifier is empty");
        log::config cfg(settings.log_level);
        cfg.console               = settings.log_to_console                               ;
        cfg.syslog                = settings.log_to_syslog ? syslog_ident : std::string() ;
        cfg.syslog_facility       = settings.syslog_facility                              ;
        cfg.file                  = settings.log_to_file                                  ;
        cfg.file_append           = settings.log_file_append                              ;
        cfg.typed_message_handler = handler                                               ;
        log::init(cfg);
        logger_io<log_io_tags::udp> ::set_mode( settings.log_io_udp );
        logger_io<log_io_tags::tcp> ::set_mode( settings.log_io_tcp );
        logger_io<log_io_tags::web> ::set_mode( settings.log_io_web );
    }

}}
