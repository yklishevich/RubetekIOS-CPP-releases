#pragma once

#include <msw/buffer.hpp>
#include <msw/noncopyable.hpp>
#include <msw/proto/spp/unpack.hpp>
#include <msw/callback/byte_range.hpp>
#include <msw/lexical_cast/bytes_as_string.hpp>

#include <rubetek/log.hpp>
#include <rubetek/essence/property.hpp>
#include <rubetek/client/device_info.hpp>
#include <rubetek/proto/ihap/transceiver_base_new.hpp>
#include <rubetek/client/controller/ihap_transceiver_callback.hpp>

#include "ihap_version_constants.h"


#ifdef RUBETEK_CLIENT_IHAP_LOG
#   define RUBETEK_CLIENT_IHAP_ON_LOG(...) logger_.debug(__VA_ARGS__);
#else
#   define RUBETEK_CLIENT_IHAP_ON_LOG(...)
#endif


namespace rubetek {
namespace client  {

    struct ihap_transceiver_new
        : ihap_transceiver_base_new
    {
        typedef msw::callback::byte_const_range ihap_packet_ready;

                explicit ihap_transceiver_new              (ihap_packet_ready, ihap_transceiver_callback*);

                void     ihap_reset                        ()                                             ;
                void     reset_large_block_transaction     ()                                             ;

                void     send_version                      ()                                             ;
                void     query_controller_info             ()                                             ;
                void     remove                            ()                                             ;
                void     activate                          (
                                                                 device_info const&
                                                               , unsigned           project_crc
                                                               , std::string const& project_alias
                                                           )                                              ;
                void     inactivate                        ()                                             ;
                void     send_password                     (std::string const&)                           ;
                void     send_project                      (std::string const&)                           ;
                void     send_project_alias                (std::string const&)                           ;
                void     send_notification_id              (std::string const&)                           ;
                void     refresh                           ()                                             ;
                void     become_known                      ()                                             ;
                void     remove_client                     (unique_client_id::const_reference)            ;

    private:

                void     internal_process_ihap_packet      (proto::ihap::packet const&)             final ;
                void     base_on_property                  (property_address, property_list const&) final ;
                void     push_to_large_block               (msw::range<msw::byte const>)                  ;

                void     internal_process_ihap_packet_impl (proto::ihap::packet const&)                   ;

        logger                     const logger_      ;
        msw::buffer<msw::byte>           large_block_ ;
        ihap_transceiver_callback*       callback_    ;
    };

}}


namespace rubetek {
namespace client {

    inline ihap_transceiver_new::ihap_transceiver_new(ihap_packet_ready ihap_packet_ready, ihap_transceiver_callback* callback)
        : ihap_transceiver_base_new ( ihap_packet_ready )
        , logger_                   (
                                         "ihap"
                                         , this
#ifdef RUBETEK_CLIENT_IHAP_LOG
                                         , log::level::debug
#endif
                                    )
        , callback_                 ( callback )
    {}

    inline void ihap_transceiver_new::ihap_reset()
    {
        ihap_transceiver_base_new::ihap_reset();
        reset_large_block_transaction();
    }
    inline void ihap_transceiver_new::reset_large_block_transaction()
    {
        large_block_.clear();
        large_block_.shrink_to_fit();
    }

    inline void ihap_transceiver_new::internal_process_ihap_packet(proto::ihap::packet const& pkt)
    {
        try
        {
            internal_process_ihap_packet_impl(pkt);
        }
        catch (std::exception const& e)
        {
            logger_.error("ihap packet: ", e.what());
            throw;
        }
    }
    inline void ihap_transceiver_new::base_on_property(property_address addr, property_list const& list)
    {
        callback_->on_property(addr, list);
    }
    inline void ihap_transceiver_new::push_to_large_block(msw::range<msw::byte const> packet)
    {
        if (large_block_.free_space() < packet.size()) throw std::runtime_error("exceeded the size of the downloaded block");
        large_block_.push_back_silent(packet);
    }

    inline void ihap_transceiver_new::send_version()
    {
        RUBETEK_CLIENT_IHAP_ON_LOG("send version");
        proto::Version s;
        s.__set_value(proto::g_ihap_version_constants.ihap_version);
        send(proto::ihap::packet_type::version, s);
    }
    inline void ihap_transceiver_new::query_controller_info()
    {
        RUBETEK_CLIENT_IHAP_ON_LOG("query controller info");
        send(proto::ihap::packet_type::controller_info);
    }
    inline void ihap_transceiver_new::remove()
    {
        RUBETEK_CLIENT_IHAP_ON_LOG("remove");
        send(proto::ihap::packet_type::remove);
    }
    inline void ihap_transceiver_new::activate
    (
        device_info const&   device
        , unsigned           project_crc
        , std::string const& project_alias
    )
    {
        RUBETEK_CLIENT_IHAP_ON_LOG
        (
              "device_name: "               , device.name
            , " language: "                 , device.language
            , " notification_id: "          , device.notification_id
            , " operation_system: "         , device.operation_system
            , " client_version: "           , device.client_version
            , " operation_system_version: " , device.operation_system_version
            , " project_alias: "            , project_alias
            , " project_crc: "              , project_crc
        );
        proto::ihap::activate s;
        s.__set_device_name              ( device.name                     );
        s.__set_language                 ( device.language                 );
        s.__set_notification_id          ( device.notification_id          );
        s.__set_operation_system         ( device.operation_system         );
        s.__set_client_version           ( device.client_version           );
        s.__set_project_alias            ( project_alias                   );
        s.__set_project_crc              ( project_crc                     );
        s.__set_operation_system_version ( device.operation_system_version );
        send(proto::ihap::packet_type::activate, s);
    }
    inline void ihap_transceiver_new::inactivate()
    {
        RUBETEK_CLIENT_IHAP_ON_LOG("inactivate");
        send(proto::ihap::packet_type::inactivate);
        reset_large_block_transaction();
    }
    inline void ihap_transceiver_new::send_password(std::string const& pwd)
    {
        RUBETEK_CLIENT_IHAP_ON_LOG("send password: ", pwd);
        proto::ihap::password s;
        s.__set_value(pwd);
        send(proto::ihap::packet_type::password, s);
    }
    inline void ihap_transceiver_new::send_project(std::string const& prj)
    {
        RUBETEK_CLIENT_IHAP_ON_LOG("send project");
        proto::ihap::project_content s;
        s.__set_value(prj);
        send(proto::ihap::packet_type::project, s);
    }
    inline void ihap_transceiver_new::send_project_alias(std::string const& project_alias)
    {
        RUBETEK_CLIENT_IHAP_ON_LOG("send project alias", project_alias);
        proto::ihap::project_alias s;
        s.__set_value(project_alias);
        send(proto::ihap::packet_type::project_alias, s);
    }
    inline void ihap_transceiver_new::send_notification_id(std::string const& id)
    {
        RUBETEK_CLIENT_IHAP_ON_LOG("send notification id");
        proto::ihap::notification_id s;
        s.__set_value(id);
        send(proto::ihap::packet_type::notification_id, s);
    }
    inline void ihap_transceiver_new::refresh()
    {
        RUBETEK_CLIENT_IHAP_ON_LOG("refresh");
        send(proto::ihap::packet_type::refresh);
    }
    inline void ihap_transceiver_new::become_known()
    {
        RUBETEK_CLIENT_IHAP_ON_LOG("become known");
        send(proto::ihap::packet_type::become_known);
    }
    inline void ihap_transceiver_new::remove_client(unique_client_id::const_reference id)
    {
        RUBETEK_CLIENT_IHAP_ON_LOG("remove client");
        proto::ihap::remove_client s;
        s.__set_value(id);
        send(proto::ihap::packet_type::remove_client, s);
    }

    inline void ihap_transceiver_new::internal_process_ihap_packet_impl(proto::ihap::packet const& pkt)
    {
        RUBETEK_CLIENT_IHAP_ON_LOG("internal process ihap packet");
        switch (pkt.type)
        {
            case proto::ihap::packet_type::controller_info:
                {
                    RUBETEK_CLIENT_IHAP_ON_LOG("recv controller_info");
                    proto::ihap::controller_info s;
                    thrift_deserialize<proto::ihap::controller_info>(pkt.payload, s);
                    callback_->on_controller_info(s.name, s.version, s.system_info, s.demo_mode, s.server_address);
                }
                break;
            case proto::ihap::packet_type::rename:
                {
                    RUBETEK_CLIENT_IHAP_ON_LOG("recv rename");
                    proto::ihap::rename s;
                    thrift_deserialize<proto::ihap::rename>(pkt.payload, s);
                    callback_->on_controller_rename(s.value);
                }
                break;
            case proto::ihap::packet_type::remove:
                RUBETEK_CLIENT_IHAP_ON_LOG("recv remove");
                callback_->on_remove();
                break;
            case proto::ihap::packet_type::password:
                {
                    RUBETEK_CLIENT_IHAP_ON_LOG("recv password");
                    option_was_removed opt_was_removed;
                    if (!pkt.payload.empty()) // for support old controllers
                    {
                        proto::ihap::password_request s;
                        thrift_deserialize<proto::ihap::password_request>(pkt.payload, s);
                        if (!s.who_removed.empty())
                        {
                            opt_was_removed = was_removed();
                            opt_was_removed->client_id = s.who_removed;
                            opt_was_removed->time = 0;
                        }
                    }
                    callback_->on_query_password(opt_was_removed);
                }
                break;
            case proto::ihap::packet_type::no_project:
                RUBETEK_CLIENT_IHAP_ON_LOG("recv no_project");
                callback_->on_no_project();
                break;
            case proto::ihap::packet_type::project_size:
                {
                    RUBETEK_CLIENT_IHAP_ON_LOG("recv project_size");
                    proto::ihap::project_size s;
                    thrift_deserialize<proto::ihap::project_size>(pkt.payload, s);
                    callback_->on_update_project(s.value);
                }
                break;
            case proto::ihap::packet_type::large_begin:
                {
                    RUBETEK_CLIENT_IHAP_ON_LOG("recv large_begin");
                    proto::ihap::large_begin s;
                    thrift_deserialize<proto::ihap::large_begin>(pkt.payload, s);
                    large_block_.reserve(s.size);
                    push_to_large_block(msw::make_range<msw::byte>(s.payload));
                    send(proto::ihap::packet_type::large_block);
                    callback_->on_large_block_enriched(large_block_.size());
                }
                break;
            case proto::ihap::packet_type::large_block:
                if (large_block_.capacity().not_null())
                {
                    RUBETEK_CLIENT_IHAP_ON_LOG("recv large_block");
                    proto::ihap::large_block s;
                    thrift_deserialize<proto::ihap::large_block>(pkt.payload, s);
                    push_to_large_block(msw::make_range<msw::byte>(s.payload));
                    send(proto::ihap::packet_type::large_block);
                    callback_->on_large_block_enriched(large_block_.size());
                }
                else logger_.warning("unexpectedly large_block");
                break;
            case proto::ihap::packet_type::large_end:
                if (large_block_.capacity().not_null())
                {
                    RUBETEK_CLIENT_IHAP_ON_LOG("recv large_end");
                    proto::ihap::large_block_end s;
                    thrift_deserialize<proto::ihap::large_block_end>(pkt.payload, s);
                    push_to_large_block(msw::make_range<msw::byte>(s.payload));
                    callback_->on_large_block_enriched(large_block_.size());
                    callback_->on_large_block(large_block_.all());
                    reset_large_block_transaction();
                }
                else logger_.warning("unexpectedly large_end");
                break;
            case proto::ihap::packet_type::loading_cancel:
                RUBETEK_CLIENT_IHAP_ON_LOG("recv loading_cancel");
                reset_large_block_transaction();
                callback_->on_loading_cancel();
                break;
            break;
            case proto::ihap::packet_type::project:
                {
                    RUBETEK_CLIENT_IHAP_ON_LOG("recv project");
                    proto::ihap::project s;
                    thrift_deserialize<proto::ihap::project>(pkt.payload, s);
                    callback_->on_project(s.crc);
                }
                break;
            case proto::ihap::packet_type::ready:
                RUBETEK_CLIENT_IHAP_ON_LOG("recv ready");
                callback_->on_ready();
                break;
            case proto::ihap::packet_type::module_command:
                {
                    RUBETEK_CLIENT_IHAP_ON_LOG("recv module_command");
                    proto::ihap::module_command s;
                    thrift_deserialize<proto::ihap::module_command>(pkt.payload, s);
                    callback_->on_module_command(s.payload);
                }
                break;
            default: logger_.error("ihap: unknown command: ", pkt.type);
        };
        RUBETEK_CLIENT_IHAP_ON_LOG("internal_process_ihap_packet exit");
    }

}}
