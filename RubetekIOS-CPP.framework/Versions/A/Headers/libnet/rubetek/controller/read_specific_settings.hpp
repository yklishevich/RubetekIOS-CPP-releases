#pragma once

#include <boost/property_tree/ptree.hpp>

#include <rubetek/controller/specific_settings.hpp>
#include <rubetek/controller/modules_specific_settings.hpp>


namespace rubetek    {
namespace controller {

    inline specific_settings read_specific_settings(boost::property_tree::ptree const& pt)
    {
        specific_settings settings;
        for (boost::property_tree::ptree::const_iterator i = pt.begin(); i != pt.end(); ++i)
            settings.push_back(std::make_pair(i->first, i->second.get_value<std::string>()));
        return settings;
    }

    inline modules_specific_settings read_specific_settings_of_modules(boost::property_tree::ptree const& pt)
    {
        modules_specific_settings specific_settings_of_modules;

        std::pair<boost::property_tree::ptree::const_assoc_iterator, boost::property_tree::ptree::const_assoc_iterator> range = pt.equal_range("module_settings");
        for (boost::property_tree::ptree::const_assoc_iterator i = range.first; i != range.second; ++i)
        {
            specific_settings_of_modules.push_back(std::make_pair(i->second.get_value<std::string>(), read_specific_settings(i->second)));
        }
        return specific_settings_of_modules;
    }

}}
