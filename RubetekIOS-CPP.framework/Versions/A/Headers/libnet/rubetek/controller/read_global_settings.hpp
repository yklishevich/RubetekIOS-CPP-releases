#pragma once

#include <string>
#include <unordered_set>

#include <boost/property_tree/ptree.hpp>

#include <msw/throw_runtime_error.hpp>

#include <rubetek/controller/specific_settings.hpp>


namespace rubetek    {
namespace controller {

    inline specific_settings read_global_settings(boost::property_tree::ptree const& pt, std::unordered_set<std::string> const& except_list = {"module_settings"})
    {
        std::unordered_set<std::string> names_of_added_settings;
        specific_settings global_settings;
        for (boost::property_tree::ptree::const_iterator i = pt.begin(); i != pt.end(); ++i)
        {
            std::string const name = i->first;
            std::string const value = i->second.get_value<std::string>();
            if (!value.empty() && !except_list.count(i->first))
            {
                if (names_of_added_settings.find(name) != names_of_added_settings.end()) msw::throw_runtime_error("setting '", name, "' already exists");
                names_of_added_settings.insert(name);
                global_settings.push_back(std::make_pair(name, value));
            }
        }
        return global_settings;
    }

}}
