#pragma once

#include <dawn/app/log_settings_read.hpp>

#include <rubetek/application/log_settings.hpp>


namespace rubetek
{
    inline void read_log_settings(boost::property_tree::ptree const& pt, log_settings& settings)
    {
        dawn::app::log_settings_read(pt, settings);
        settings.log_io_ihlp  = log_io_mode_from_string( pt.get<std::string> ( "log_io_ihlp"  , "disable" ) );
        settings.log_io_ihmlp = log_io_mode_from_string( pt.get<std::string> ( "log_io_ihmlp" , "disable" ) );
    }
}
