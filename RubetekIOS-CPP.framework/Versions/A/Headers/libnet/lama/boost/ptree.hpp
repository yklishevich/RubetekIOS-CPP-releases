#pragma once

#include <string>
#include <ostream>

#include <boost/optional.hpp>

#include <msw/format.hpp>
#include <msw/std/regex.hpp>
#include <lama/boost/include/property_tree/ptree.hpp>


namespace lama        {
namespace boost_ptree {

    inline std::string get_value(boost::property_tree::ptree::value_type const& v)
    {
        return v.second.get_value(v.first);
    }

    namespace zzz
    {
        inline void print(boost::property_tree::ptree const& ptree, std::ostream& s = std::cout, int max_level = -1, int level = 0, int tab_size = 2)
        {
            std::string tab;
            for (int n = 0; n != (level * tab_size); n++) tab.push_back(' ');
            level++;
            if ((max_level != -1) && (level > max_level)) return;
            for (boost::property_tree::ptree::value_type v : ptree)
            {
                msw::format_s(s, tab, v.first, ": ", get_value(v), '\n');
                print(v.second, s, max_level, level, tab_size);
            }
        }
    }
    inline void print(boost::property_tree::ptree const& ptree, std::ostream& s = std::cout, int max_level = -1, int tab_size = 2)
    {
        zzz::print(ptree, s, max_level, 0, tab_size);
    }

    inline boost::property_tree::ptree::const_iterator find_by_value(boost::property_tree::ptree const& ptree, std::string const& value)
    {
        for (auto i = ptree.begin(); i != ptree.end(); ++i)
            if (get_value(*i) == value)
                return i;
        return ptree.end();
    }

    template <typename T = std::string>
    boost::optional<T> find(boost::property_tree::ptree ptree, std::string const& key)
    {
        auto i = ptree.find(key);
        if (i == ptree.not_found()) return boost::none;
        return i->second.get_value<T>();
    }

    template <typename OnReady>
    void for_each(boost::property_tree::ptree ptree, OnReady on_ready, msw::regex const& e)
    {
        for (boost::property_tree::ptree::value_type v : ptree)
        {
            msw::smatch m;
            if (msw::regex_match(v.first, m, e)) on_ready(m, get_value(v));
            for_each(v.second, on_ready, e);
        }
    }

}}
