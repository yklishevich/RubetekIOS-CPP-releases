#pragma once

#include <string>

#include <msw/noncopyable.hpp>

#include <rubetek/essence/property.hpp>
#include <rubetek/essence/unique_id/controller.hpp>


namespace rubetek {
namespace client  {

    struct controllers_unit_back_interface
        : msw::noncopyable
    {
        virtual void        controllers_unit_on_any_controller_change_status        ()                                      = 0 ;
        virtual void        controllers_unit_on_became_known                        (unique_controller_id::const_reference) = 0 ;
        virtual void        controllers_unit_on_removed_from_controller             (unique_controller_id::const_reference) = 0 ;
        virtual std::string controllers_unit_on_get_alias                           (unique_controller_id::const_reference) = 0 ;
        virtual std::string controllers_unit_on_get_name                            (unique_controller_id::const_reference) = 0 ;
        virtual void        controllers_unit_on_disconnected_active_controller      ()                                      = 0 ;
        virtual void        controllers_unit_on_active_controller_change_connection ()                                      = 0 ;
        virtual void        controllers_unit_on_loading_progress                    (int)                                   = 0 ;
        virtual void        controllers_unit_on_rename                              (unique_controller_id::const_reference) = 0 ;
    protected:
        ~controllers_unit_back_interface() {}
    };

}}
