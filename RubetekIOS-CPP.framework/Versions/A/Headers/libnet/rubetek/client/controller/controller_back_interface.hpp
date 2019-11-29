#pragma once

#include <msw/noncopyable.hpp>

#include <rubetek/essence/unique_id/controller.hpp>


namespace rubetek {
namespace client  {

    struct controller_back_interface
        : msw::noncopyable
    {
        virtual void controller_on_change_status            (unique_controller_id::const_reference) = 0 ;
        virtual void controller_on_switch_links             (unique_controller_id::const_reference) = 0 ;
        virtual void controller_on_change_remote_connection (unique_controller_id::const_reference) = 0 ;
        virtual void controller_on_make_known               (unique_controller_id::const_reference) = 0 ;
        virtual void controller_on_removed_from_controller  (unique_controller_id::const_reference) = 0 ;
        virtual void controller_on_set_offline              (unique_controller_id::const_reference) = 0 ;
        virtual void controller_on_rename                   (unique_controller_id::const_reference) = 0 ;
    protected:
        ~controller_back_interface() {}
    };

}}
