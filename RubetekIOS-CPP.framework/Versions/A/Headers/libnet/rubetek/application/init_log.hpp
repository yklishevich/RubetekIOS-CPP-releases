#pragma once

#include <dawn/app/log_init.hpp>

#include <rubetek/log.hpp>
#include <rubetek/application/log_settings.hpp>


namespace rubetek
{
    inline void init_log(log_settings const& settings, std::string const& syslog_ident = "", log::config::typed_message_handler_type handler = nullptr)
    {
        dawn::app::log_init(settings, syslog_ident, handler);
        logger_io<log_io_tag_ihlp>  ::set_mode( settings.log_io_ihlp  );
        logger_io<log_io_tag_ihmlp> ::set_mode( settings.log_io_ihmlp );
    }
}
