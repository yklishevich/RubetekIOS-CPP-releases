#pragma once

#include <string>

#include <msw/noncopyable.hpp>

#include <rubetek/config.hpp>
#include <rubetek/essence/unique_id/controller.hpp>


namespace rubetek {
namespace client  {

    struct configurator
        : msw::noncopyable
    {
        bool const default_auto_connect_by_location = false ;
        bool const default_auto_connect_to_recent   = true  ;


        explicit                    configurator            (std::string const& path)          ;

        void                       auto_connect_by_location (bool)                             ;
        bool                       auto_connect_by_location ()                           const ;
        void                       auto_connect_to_recent   (bool)                             ;
        bool                       auto_connect_to_recent   ()                           const ;
        void                       recent_controller        (unique_controller_id::type)       ;
        unique_controller_id::type recent_controller        ()                           const ;
        bool                       has_recent_controller    ()                           const ;
        void                       remove_recent_controller ()                                 ;

    private:
        void                       load                     ()                                 ;
        void                       save                     ()                           const ;

        std::string                const filename_                 ;
        bool                             auto_connect_by_location_ ;
        bool                             auto_connect_to_recent_   ;
        unique_controller_id::type       recent_controller_        ;
    };

}}


#ifdef RUBETEK_HEADER_ONLY
#   include <rubetek/client/client/impl/configurator.cpp>
#endif
