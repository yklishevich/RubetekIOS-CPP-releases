#pragma once

#include <rubetek/config.hpp>

#include <msw/config.hpp>

#include <dawn/log/log.hpp>
#include <dawn/log/logger.hpp>
#include <dawn/log/logger_io.hpp>
#include <dawn/log/log_io_mode.hpp>
#include <dawn/log/log_io_tags.hpp>
#ifdef MSW_LINUX
#   include <dawn/log/syslog_facility_from_string.hpp>
#endif


namespace rubetek
{
    using dawn::log;
    using dawn::logger;
    using dawn::logger_io;
    using dawn::log_io_mode;
    namespace log_io_tags = dawn::log_io_tags;
    using dawn::log_io_mode_from_string;
#ifdef MSW_LINUX
    using dawn::syslog_facility_from_string;
#endif

    struct log_io_tag_ihlp {};
    struct log_io_tag_ihmlp{};
}
