#pragma once

#include <msw/throw_runtime_error.hpp>

#include <dawn/lexical_cast.hpp>

#include <rubetek/utility/ctti.hpp>
#include <rubetek/utility/string_map.hpp>
#include <rubetek/utility/multiform/package.hpp>
#include <rubetek/utility/multiform/multiform.hpp>
#include <rubetek/module/settings/setting.hpp>
#include <rubetek/module/settings/settings_package.hpp>
#include <rubetek/essence/property/address.hpp>


namespace rubetek {
namespace module  {

    struct settings
    {
                                          explicit                settings (settings_list, property_address = "")           ;
                                          bool                    has      (std::string const& name)                  const ;
        template <typename T>             T                       get      (std::string const& name)                  const ;
        template <typename T>             T                       get      (std::string const& name, T default_value) const ;
        template <typename T, typename D> D                       get_cast (std::string const& name)                  const ;
        template <typename T, typename D> D                       get_cast (std::string const& name, D default_value) const ;
                                          property_address const& id       ()                                         const ;

    private:

        settings& operator = (settings const&) = delete;

        struct map
            : utility::string_map<multiform>
        {
            map();
        };

        property_address const id_  ;
        map                    map_ ;
    };

}}


namespace rubetek {
namespace module  {

    inline settings::settings(settings_list list, property_address id)
        : id_(id)
    {
        for (setting const& s : list)
            map_.add(s.name, s.value);
    }

    bool settings::has(std::string const& name) const
    {
        return map_.has(name);
    }
    template <typename T>
    T settings::get(std::string const& name) const
    {
        T target = T();
        try
        {
            target = map_.get(name).unpack<T>();
        }
        catch (std::exception const& e)
        {
            msw::throw_runtime_error("error unpacking setting '", name, "': ", e.what());
        }
        return target;
    }
    template <typename T>
    T settings::get(std::string const& name, T default_value) const
    {
        return map_.has(name) ? get<T>(name) : default_value;
    }
    template <typename T, typename D>
    D settings::get_cast(std::string const& name) const
    {
        return dawn::lexical_cast<D>(get<T>(name));
    }
    template <typename T, typename D>
    D settings::get_cast(std::string const& name, D default_value) const
    {
        return map_.has(name) ? get_cast<T, D>(name) : default_value;
    }
    inline property_address const& settings::id() const
    {
        return id_;
    }

    inline settings::map::map()
        : utility::string_map<multiform>("setting")
    {}

}}
