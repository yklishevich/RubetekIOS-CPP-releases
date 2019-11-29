#pragma once

#include <rubetek/log.hpp>
#include <rubetek/config.hpp>
#include <rubetek/client/device_info.hpp>
#include <rubetek/client/controller/ihap_transceiver_new.hpp>
#include <rubetek/client/controller/ihap_transceiver_callback.hpp>


namespace rubetek {
namespace client  {

    struct ihap_transceiver
        : ihap_transceiver_new
    {
        typedef msw::callback::byte_const_range ihap_packet_ready;

                explicit ihap_transceiver              (ihap_packet_ready, ihap_transceiver_callback*) ;

                void     process_ihap_packet           (msw::range<msw::byte const>)                   ;
                void     send_property                 (property_address, property_list const&)        ;

                void     ihap_reset                    ()                                              ;
                void     reset_large_block_transaction ()                                              ;

                void     query_controller_info         ()                                              ;
                void     remove                        ()                                              ;
                void     activate                      (
                                                             device_info const&
                                                           , unsigned           project_crc
                                                           , std::string const& project_alias
                                                       )                                               ;
                void     inactivate                    ()                                              ;
                void     send_password                 (std::string const&)                            ;
                void     send_project                  (std::string const&)                            ;
                void     send_project_alias            (std::string const&)                            ;
                void     refresh                       ()                                              ;
                void     become_known                  ()                                              ;
                void     remove_client                 (unique_client_id::const_reference)             ;
    private:
        logger const logger_;
    };

}}

#ifdef RUBETEK_HEADER_ONLY
#   include <rubetek/client/controller/impl/ihap_transceiver.cpp>
#endif
