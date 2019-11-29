#pragma once

#include <string>
#include <fstream>

#include <msw/throw_runtime_error.hpp>

#include <lama/boost/include/property_tree/ptree.hpp>
#include <lama/boost/include/property_tree/info_parser.hpp>


namespace rubetek    {
namespace controller {

    inline boost::property_tree::ptree read_config_file(std::string const& filename)
    {
        std::ifstream config_file(filename);
        if (!config_file.good()) msw::throw_runtime_error("can't open config file: '", filename, "'");
        boost::property_tree::ptree pt;
        boost::property_tree::read_info(config_file, pt);
        return std::move(pt);
    }

}}
