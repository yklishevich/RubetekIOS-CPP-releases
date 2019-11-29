#pragma once

#include <string>

#include <boost/property_tree/ptree.hpp>
#include <boost/algorithm/string/case_conv.hpp>

#include <msw/config.hpp>

#include <dawn/log/log.hpp>
#include <dawn/decode_filename.hpp>
#include <dawn/app/log_settings.hpp>
#ifdef MSW_LINUX
#   include <dawn/log/syslog_facility_from_string.hpp>
#endif


namespace dawn {
namespace app  {

    template <typename Settings>
    void log_settings_read(boost::property_tree::ptree const& pt, Settings& settings)
    {
        settings.log_level       = log::level_from_string  ( pt.get<std::string> ("log_level"       , "info"    ) );
        settings.log_to_console  =                           pt.get<bool>        ("log_to_console"  , false       );
        settings.log_to_file     = decode_filename         ( pt.get<std::string> ("log_to_file"     , ""        ) );
        settings.log_file_append =                           pt.get<bool>        ("log_file_append" , false       );
        settings.log_io_udp      = log_io_mode_from_string ( pt.get<std::string> ("log_io_udp"      , "disable" ) );
        settings.log_io_tcp      = log_io_mode_from_string ( pt.get<std::string> ("log_io_tcp"      , "disable" ) );
        settings.log_io_web      = log_io_mode_from_string ( pt.get<std::string> ("log_io_web"      , "disable" ) );
#ifdef MSW_LINUX
        std::string syslog_facility = pt.get<std::string> ("syslog_facility", "none" );
        boost::algorithm::to_lower(syslog_facility);
        settings.log_to_syslog = syslog_facility != "none";
        if (settings.log_to_syslog) settings.syslog_facility = syslog_facility_from_string(syslog_facility);
#endif
    }

}}
