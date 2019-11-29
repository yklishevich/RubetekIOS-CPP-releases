#pragma once

#include <string>

#include <boost/property_tree/ptree.hpp>

#include <dawn/app/log_init.hpp>
#include <dawn/app/log_settings_read.hpp>


namespace dawn {
namespace app  {

    inline void log_init_from_pt(boost::property_tree::ptree const pt, std::string const& syslog_ident = "")
    {
        log_settings settings;
        log_settings_read(pt, settings);
        log_init(settings, syslog_ident);
    }

}}
