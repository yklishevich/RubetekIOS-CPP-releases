#pragma once

#include <string>

#include <msw/buffer.hpp>

#include <rubetek/module/settings/setting.hpp>
#include <rubetek/controller/specific_settings.hpp>
#include <rubetek/module/settings/settings_package.hpp>
#include <rubetek/controller/modules_specific_settings.hpp>


namespace rubetek    {
namespace controller {

    inline msw::buffer<msw::byte> pack_module_settings(std::string const& module_name, specific_settings const& global_settings, modules_specific_settings const& specific_settings)
    {
        rubetek::module::settings_list settings_list;
        for (auto const& s : global_settings)
        {
            std::string const setting_name = "global." + s.first;
            settings_list.push_back(rubetek::module::setting(setting_name, s.second));
        }
        for (auto const& ss : specific_settings)
        {
            if (ss.first == module_name)
            {
                for (auto const& s : ss.second)
                    settings_list.push_back(rubetek::module::setting(s.first, s.second));
                break;
            }
        }
        msw::buffer<msw::byte> buf_settings;
        buf_settings.push_back(pack(settings_list));
        return std::move(buf_settings);
    }

}}
