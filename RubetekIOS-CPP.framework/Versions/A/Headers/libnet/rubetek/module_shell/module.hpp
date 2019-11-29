#pragma once

#include <string>

#include <msw/std/memory.hpp>

#include <rubetek/module/profile.hpp>
#include <rubetek/module_shell/lib.hpp>
#include <rubetek/module_shell/instance.hpp>


namespace rubetek      {
namespace module_shell {

    template <typename Lib = lib, typename Instance = instance<>>
    struct module
    {
        typedef Lib      lib_type      ;
        typedef Instance instance_type ;

        explicit       module          (std::string const& path, instance_back_interface* back_interface = nullptr, msw::range<msw::byte const> settings = msw::range<msw::byte const>()) ;
        ~              module          ()                                                                                                                                                 ;
        char const*    version         () const                                                                                                                                           ;
        bool           has_instance    () const                                                                                                                                           ;
        void           create_instance (instance_back_interface* back_interface, msw::range<msw::byte const> settings)                                                                    ;
        instance_type& get_instance    ()                                                                                                                                                 ;

    private:
        lib_type                       lib_      ;
        std::unique_ptr<instance_type> instance_ ;
    };

}}


namespace rubetek      {
namespace module_shell {

    template <typename Lib, typename Instance>
    module<Lib, Instance>::module(std::string const& path, instance_back_interface* back_interface, msw::range<msw::byte const> settings)
        : lib_ (path)
    {
        if (back_interface) create_instance(back_interface, settings);
    }
    template <typename Lib, typename Instance>
    module<Lib, Instance>::~module()
    {
        if (instance_)
        {
            if (instance_->has_link())
            {
                instance_->remove_devices();
                lib_.free();
            }
            instance_.reset();
        }
    }
    template <typename Lib, typename Instance>
    char const* module<Lib, Instance>::version() const
    {
        return lib_.version();
    }
    template <typename Lib, typename Instance>
    bool module<Lib, Instance>::has_instance() const
    {
        return static_cast<bool>(instance_);
    }
    template <typename Lib, typename Instance>
    void module<Lib, Instance>::create_instance(instance_back_interface* back_interface, msw::range<msw::byte const> settings)
    {
        if (instance_) throw std::runtime_error("instance already created");
        instance_ = msw::make_unique<instance_type>(back_interface);
        rubetek::module::profile profile;
        profile.context.settings    = utility::raw_memory(settings.data().native(), settings.size().count());
        profile.log_controller_link = instance_->log_link ();
        profile.controller_link     = instance_->back_link();
        instance_->init(lib_.init(profile));
    }
    template <typename Lib, typename Instance>
    typename module<Lib, Instance>::instance_type& module<Lib, Instance>::get_instance()
    {
        if (!instance_) throw std::runtime_error("instance is empty");
        return *instance_;
    }

}}
