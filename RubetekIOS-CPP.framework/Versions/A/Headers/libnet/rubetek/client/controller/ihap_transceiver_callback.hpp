#pragma once

#include <string>

#include <msw/range.hpp>
#include <msw/noncopyable.hpp>

#include <rubetek/client/was_removed.hpp>


namespace rubetek {
namespace client  {

    struct ihap_transceiver_callback
        : msw::noncopyable
    {
        virtual void on_controller_info      (
                                                  std::string const& name
                                                , std::string const& version
                                                , std::string const& system_info
                                                , bool               demo_mode
                                                , std::string const& server_address
                                             )                                        = 0 ;
        virtual void on_controller_rename    (std::string const&)                     = 0 ;
        virtual void on_remove               ()                                       = 0 ;
        virtual void on_query_password       (option_was_removed)                     = 0 ;
        virtual void on_no_project           ()                                       = 0 ;
        virtual void on_update_project       (unsigned size)                          = 0 ;
        virtual void on_large_block          (msw::range<msw::byte const>)            = 0 ;
        virtual void on_large_block_enriched (msw::size<msw::byte>)                   = 0 ;
        virtual void on_loading_cancel       ()                                       = 0 ;
        virtual void on_project              (unsigned crc)                           = 0 ;
        virtual void on_ready                ()                                       = 0 ;
        virtual void on_property             (property_address, property_list const&) = 0 ;
        virtual void on_module_command       (std::string const&)                     = 0 ;
    };

}}
