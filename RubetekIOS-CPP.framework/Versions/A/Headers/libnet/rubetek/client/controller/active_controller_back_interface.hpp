#pragma once

#include <string>

#include <msw/range.hpp>
#include <msw/noncopyable.hpp>

#include <rubetek/essence/property.hpp>
#include <rubetek/client/was_removed.hpp>


namespace rubetek {
namespace client  {

    struct active_controller_back_interface
        : msw::noncopyable
    {
        virtual void active_controller_on_query_password         (option_was_removed)                        = 0 ;
        virtual void active_controller_on_no_project             ()                                          = 0 ;
        virtual void active_controller_on_no_required_resolution ()                                          = 0 ;
        virtual void active_controller_on_update_project         ()                                          = 0 ;
        virtual void active_controller_on_loading_progress       (int)                                       = 0 ;
        virtual void active_controller_on_project                (msw::range<msw::byte const>, unsigned crc) = 0 ;
        virtual void active_controller_on_ready                  ()                                          = 0 ;
        virtual void active_controller_on_property               (property_address, property_list const&)    = 0 ;
        virtual void active_controller_on_module_command         (std::string const&)                        = 0 ;
    protected:
        ~active_controller_back_interface() {}
    };

}}
