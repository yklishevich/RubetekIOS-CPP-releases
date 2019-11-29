#pragma once

#include <msw/range.hpp>

#include <rubetek/essence/property.hpp>
#include <rubetek/essence/unique_id/editor.hpp>


namespace rubetek      {
namespace module_shell {

    struct instance_back_interface
    {
        virtual void instance_back_send_log                 (module::log::level level, std::string const& msg)                   = 0;
        virtual void instance_back_send_accessible          (bool)                                                               = 0;
        virtual void instance_back_send_command             (msw::range<msw::byte const> editor_id, msw::range<msw::byte const>) = 0;
        virtual void instance_back_send_properties          (property_address, property_list const&)                             = 0;
        virtual void instance_back_send_reload_clients_list ()                                                                   = 0;
        virtual void instance_back_send_inter_module_packet (msw::range<msw::byte const>)                                        = 0;
    protected:
        ~instance_back_interface() {}
    };

}}
