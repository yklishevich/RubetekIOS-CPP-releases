#pragma once

#include <msw/throw_runtime_error.hpp>

#include <rubetek/essence/property.hpp>
#include <rubetek/module/device/interface.hpp>
#include <rubetek/utility/multiform/multiform.hpp>
#include <rubetek/utility/multiform/package.hpp>
#include <rubetek/utility/multiform/from_variant.hpp>
#include <rubetek/utility/multiform/to_property_value.hpp>


namespace rubetek      {
namespace module_shell {

    struct device_session
        : rubetek::module::device_controller_interface
    {
                                                device_session        ()                                        ;

        void                                    open                  (rubetek::module::controller_device_link) ;
        rubetek::module::controller_device_link eject_link            ()                                        ;

        bool                                    accessible            ()                                        ;
        void                                    write_property        (property const&)                         ;
        property::value_type                    read_property_value   (property_id_type)                        ;
        void                                    idle                  ()                                        ;

    private:

        rubetek::module::controller_device_link link                  ()                                        ;
        void                                    property_value_readed (utility::raw_memory)                     ;

        rubetek::module::controller_device_link controller_device_link_ ;
        property::value_type                    readed_property_value_  ;
    };

}}


namespace rubetek      {
namespace module_shell {

    inline device_session::device_session()
        : controller_device_link_(nullptr)
    {}

    inline void device_session::open(rubetek::module::controller_device_link controller_device_link)
    {
        MSW_ASSERT(!controller_device_link_);
        MSW_ASSERT(controller_device_link);
        controller_device_link_ = controller_device_link;
    }
    inline rubetek::module::controller_device_link device_session::eject_link()
    {
        auto const link = controller_device_link_;
        controller_device_link_ = nullptr;
        return link;
    }

    inline bool device_session::accessible()
    {
        return link()->accessible();
    }
    inline void device_session::write_property(property const& prop)
    {
        multiform mf(multiform_from_variant(prop.value));
        link()->write_property(prop.id, utility::raw_memory(pack(mf)));
    }
    inline property::value_type device_session::read_property_value(property_id_type id)
    {
        readed_property_value_ = property::value_type();
        link()->read_property_value(id);
        if (readed_property_value_.empty()) msw::throw_runtime_error("can't read property '", id,"' value from device");
        return readed_property_value_;
    }
    inline void device_session::idle()
    {
        link()->idle();
    }

    inline rubetek::module::controller_device_link device_session::link()
    {
        MSW_ASSERT(controller_device_link_);
        return controller_device_link_;
    }
    inline void device_session::property_value_readed(utility::raw_memory raw_memory)
    {
        multiform mf = unpack<multiform>(buffer(raw_memory.begin(), raw_memory.end()));
        readed_property_value_ = to_property_value(mf);
    }

}}
