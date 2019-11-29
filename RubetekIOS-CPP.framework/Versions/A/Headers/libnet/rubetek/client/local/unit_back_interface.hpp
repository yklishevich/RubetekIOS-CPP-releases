#pragma once

#include <msw/noncopyable.hpp>

#include <rubetek/local_link/selector.hpp>
#include <rubetek/essence/unique_id/controller.hpp>


namespace rubetek {
namespace client  {

    struct local_unit_back_interface
        : msw::noncopyable
    {
        virtual bool local_unit_on_check_project     (unique_controller_id::const_reference)                                   = 0 ;
        virtual void local_unit_on_remove_project    (unique_controller_id::const_reference)                                   = 0 ;
        virtual void local_unit_on_add_controller    (unique_controller_id::const_reference, local_link_selector*, bool known) = 0 ;
        virtual void local_unit_on_remove_controller (unique_controller_id::const_reference)                                   = 0 ;
    protected:
        ~local_unit_back_interface() {}
    };

}}
