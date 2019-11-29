#pragma once

#include <msw/buffer.hpp>
#include <msw/noncopyable.hpp>
#include <msw/callback/byte_range.hpp>

#include <rubetek/log.hpp>
#include <rubetek/essence/property.hpp>
#include <rubetek/essence/device_properties.hpp>
#include <rubetek/utility/package/property.hpp>
#include <rubetek/utility/package/list.hpp>
#include <rubetek/utility/package/device_properties.hpp>
#include <rubetek/proto/thrift/serialize.hpp>

#include "ihap_types.h"


#ifdef RUBETEK_IHAP_TRANSCEIVER_BASE_LOG
#   define RUBETEK_IHAP_TRANSCEIVER_BASE_ON_LOG(...) logger_.debug(__VA_ARGS__);
#else
#   define RUBETEK_IHAP_TRANSCEIVER_BASE_ON_LOG(...)
#endif


namespace rubetek
{
    struct ihap_transceiver_base_new
        : msw::noncopyable
    {
        typedef msw::callback::byte_const_range ihap_packet_ready;

        explicit     ihap_transceiver_base_new    (ihap_packet_ready)                             ;

        void         process_ihap_packet          (msw::range<msw::byte const>)                   ;
        void         send_property                (property_address, property_list const&)        ;
        void         send_device_properties       (device_properties_list const&)                 ;
        void         send_module_command          (msw::range<msw::byte const>)                   ;

        void         ihap_reset                   ()                                              ;

    protected:
        void         send                         (proto::ihap::packet_type::type)                ;
        template <typename Pay>
        void         send                         (proto::ihap::packet_type::type, Pay const&)    ;

    private:
        void         process_property             (msw::range<msw::byte const>)                   ;
        void         send_large_module_command    (msw::range<msw::byte const>)                   ;

        virtual void internal_process_ihap_packet (proto::ihap::packet const&)                = 0 ;
        virtual void base_on_property             (property_address, property_list const&)    = 0 ;

    protected:
        logger            const logger_            ;
    private:
        ihap_packet_ready       ihap_packet_ready_ ;
        std::string             module_command_    ;
    };
}


namespace rubetek
{
    inline ihap_transceiver_base_new::ihap_transceiver_base_new(ihap_packet_ready ihap_packet_ready)
        : logger_            (
                                  "ihap base"
                                  , this
#ifdef RUBETEK_IHAP_TRANSCEIVER_BASE_LOG
                                  , log::level::debug
#endif
                             )
        , ihap_packet_ready_ (ihap_packet_ready)
    {}

    inline void ihap_transceiver_base_new::process_ihap_packet(msw::range<msw::byte const> packet)
    {
        try
        {
            RUBETEK_IHAP_TRANSCEIVER_BASE_ON_LOG("process ihap packet");
            proto::ihap::packet pkt;
            thrift_deserialize<proto::ihap::packet>(msw::bytes_as_string(packet), pkt);
            if (pkt.type == proto::ihap::packet_type::version)
            {
                RUBETEK_IHAP_TRANSCEIVER_BASE_ON_LOG("received version packet");
            }
            else if (pkt.type == proto::ihap::packet_type::property)
            {
                RUBETEK_IHAP_TRANSCEIVER_BASE_ON_LOG("received property");
                proto::ihap::property s;
                thrift_deserialize<proto::ihap::property>(pkt.payload, s);
                process_property(msw::make_range<msw::byte>(s.payload));
            }
            else if (pkt.type == proto::ihap::packet_type::module_command_large)
            {
                RUBETEK_IHAP_TRANSCEIVER_BASE_ON_LOG("received module command large");
                proto::ihap::module_command s;
                thrift_deserialize<proto::ihap::module_command>(pkt.payload, s);
                module_command_.append(s.payload);
            }
            else if (pkt.type == proto::ihap::packet_type::module_command_end)
            {
                RUBETEK_IHAP_TRANSCEIVER_BASE_ON_LOG("received module command end");
                pkt.__set_type(proto::ihap::packet_type::module_command);
                {
                    proto::ihap::module_command mc;
                    mc.__set_payload(module_command_);
                    module_command_.clear();
                    pkt.__set_payload(thrift_serialize(mc));
                }
                internal_process_ihap_packet(pkt);
            }
            else internal_process_ihap_packet(pkt);
            RUBETEK_IHAP_TRANSCEIVER_BASE_ON_LOG("process ihap packet exit");
        }
        catch (std::exception const& e)
        {
            logger_.error("process incomin packet: ", e.what());
        }
    }

    inline void ihap_transceiver_base_new::send_property(property_address address, property_list const& list)
    {
        RUBETEK_IHAP_TRANSCEIVER_BASE_ON_LOG("send property");
        device_properties_list lst;
        lst.push_back({address, list});
        send_device_properties(lst);
    }
    inline void ihap_transceiver_base_new::send_device_properties(device_properties_list const& list)
    {
        RUBETEK_IHAP_TRANSCEIVER_BASE_ON_LOG("send device properties");
        msw::buffer<msw::byte> buf;
        buf.push_back(pack(list));
        proto::ihap::property property_packet;
        property_packet.__set_payload(msw::bytes_as_string(buf.all()));
        send(proto::ihap::packet_type::property, property_packet);
    }
    inline void ihap_transceiver_base_new::send_module_command(msw::range<msw::byte const> cmd)
    {
        RUBETEK_IHAP_TRANSCEIVER_BASE_ON_LOG("send module command: ", cmd.size(), " B");
        if (cmd.size() < msw::KB * 60)
        {
            proto::ihap::module_command packet;
            packet.__set_payload(msw::bytes_as_string(cmd));
            send(proto::ihap::packet_type::module_command, packet);
        }
        else send_large_module_command(cmd);
    }
    inline void ihap_transceiver_base_new::ihap_reset()
    {
        module_command_.clear();
    }

    inline void ihap_transceiver_base_new::send(proto::ihap::packet_type::type type)
    {
        RUBETEK_IHAP_TRANSCEIVER_BASE_ON_LOG("send");
        proto::ihap::packet pkt;
        pkt.__set_type(type);
        ihap_packet_ready_(msw::make_range<msw::byte>(thrift_serialize(pkt)));
    }
    template <typename Pay>
    void ihap_transceiver_base_new::send(proto::ihap::packet_type::type type, Pay const& pay)
    {
        RUBETEK_IHAP_TRANSCEIVER_BASE_ON_LOG("send pay");
        proto::ihap::packet pkt;
        pkt.__set_type(type);
        pkt.__set_payload(thrift_serialize(pay));
        ihap_packet_ready_(msw::make_range<msw::byte>(thrift_serialize(pkt)));
    }

    inline void ihap_transceiver_base_new::process_property(msw::range<msw::byte const> packet)
    {
        RUBETEK_IHAP_TRANSCEIVER_BASE_ON_LOG("process property");
        utility::raw_memory mem(packet.data().native(), packet.size().count());
        device_properties_list const list = unpack<device_properties_list>(mem);
        for (device_properties const& properties : list)
            base_on_property(properties.address, properties.properties);
        RUBETEK_IHAP_TRANSCEIVER_BASE_ON_LOG("process_property exit");
    }
    inline void ihap_transceiver_base_new::send_large_module_command(msw::range<msw::byte const> cmd)
    {
        RUBETEK_IHAP_TRANSCEIVER_BASE_ON_LOG("send large module command: ", cmd.size(), " B");
        while (cmd.has_items())
        {
            proto::ihap::module_command packet;
            packet.__set_payload(msw::bytes_as_string(cmd.pop_front_max(msw::KB * 60)));
            send(proto::ihap::packet_type::module_command_large, packet);
        }
        send(proto::ihap::packet_type::module_command_end);
    }
}
