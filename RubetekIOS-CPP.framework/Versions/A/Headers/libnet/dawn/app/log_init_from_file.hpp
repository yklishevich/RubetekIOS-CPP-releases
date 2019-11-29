#pragma once

#include <string>

#include <boost/property_tree/ptree.hpp>

#include <dawn/read_config_file.hpp>
#include <dawn/app/log_init_from_pt.hpp>


namespace dawn {
namespace app  {

    inline boost::property_tree::ptree log_init_from_file(std::string const& filename, std::string const& syslog_ident = "")
    {
        boost::property_tree::ptree const pt = read_config_file(filename);
        log_init_from_pt(pt, syslog_ident);
        return pt;
    }

}}
