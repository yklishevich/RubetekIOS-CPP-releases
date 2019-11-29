#include <rubetek/log.hpp>
#include <rubetek/client/device_info.hpp>
#include <rubetek/client/controller/ihap_transceiver_new.hpp>
#include <rubetek/client/controller/ihap_transceiver_callback.hpp>


namespace rubetek {
namespace client  {

    inline ihap_transceiver::ihap_transceiver(ihap_packet_ready ihap_packet_ready, ihap_transceiver_callback* callback)
        : ihap_transceiver_new ( ihap_packet_ready, callback )
        , logger_              ( "ihap transceiver"          )
    {}

#define RUBETEK_CLIENT_IHAP_TRANS_FN(F, ...) ihap_transceiver_new::F(__VA_ARGS__);

    inline void ihap_transceiver::process_ihap_packet(msw::range<msw::byte const> pkt)
    {
        ihap_transceiver_new::process_ihap_packet(pkt);
    }

    inline void ihap_transceiver::send_property(property_address addr, property_list const& list)
    {
        RUBETEK_CLIENT_IHAP_TRANS_FN(send_property, addr, list)
    }
    inline void ihap_transceiver::ihap_reset()
    {
        RUBETEK_CLIENT_IHAP_TRANS_FN(ihap_reset)
    }
    inline void ihap_transceiver::reset_large_block_transaction()
    {
        RUBETEK_CLIENT_IHAP_TRANS_FN(reset_large_block_transaction)
    }

    inline void ihap_transceiver::query_controller_info()
    {
        RUBETEK_CLIENT_IHAP_TRANS_FN(query_controller_info)
    }
    inline void ihap_transceiver::remove()
    {
        RUBETEK_CLIENT_IHAP_TRANS_FN(remove)
    }
    inline void ihap_transceiver::activate
    (
          device_info const& device
        , unsigned           project_crc
        , std::string const& project_alias
    )
    {
        RUBETEK_CLIENT_IHAP_TRANS_FN(activate, device, project_crc, project_alias)
    }
    inline void ihap_transceiver::inactivate()
    {
        RUBETEK_CLIENT_IHAP_TRANS_FN(inactivate)
    }
    inline void ihap_transceiver::send_password(std::string const& pwd)
    {
        RUBETEK_CLIENT_IHAP_TRANS_FN(send_password, pwd)
    }
    inline void ihap_transceiver::send_project(std::string const& prj)
    {
        RUBETEK_CLIENT_IHAP_TRANS_FN(send_project, prj)
    }
    inline void ihap_transceiver::send_project_alias(std::string const& project_alias)
    {
        RUBETEK_CLIENT_IHAP_TRANS_FN(send_project_alias, project_alias)
    }
    inline void ihap_transceiver::refresh()
    {
        RUBETEK_CLIENT_IHAP_TRANS_FN(refresh)
    }
    inline void ihap_transceiver::become_known()
    {
        RUBETEK_CLIENT_IHAP_TRANS_FN(become_known)
    }
    inline void ihap_transceiver::remove_client(unique_client_id::const_reference id)
    {
        RUBETEK_CLIENT_IHAP_TRANS_FN(remove_client, id)
    }

#undef RUBETEK_CLIENT_IHAP_TRANS_FN

}}
