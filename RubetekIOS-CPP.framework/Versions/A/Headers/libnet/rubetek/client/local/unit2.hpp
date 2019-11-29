#pragma once

#include <list>

#include <msw/noncopyable.hpp>
#include <msw/include/boost/asio.hpp>
#include <msw/asio/socket_options.hpp>//
#include <msw/lexical_cast/network/endpoint/to_string.hpp>

#include <rubetek/log.hpp>
#include <rubetek/local_link/set.hpp>
#include <rubetek/client/local/beacon.hpp>
#include <rubetek/client/local/connection.hpp>
//#include <rubetek/socket/ihlp_session_set.hpp>
//#include <rubetek/socket/tcp_connector_set.hpp>
#include <rubetek/essence/unique_id/client.hpp>
#include <rubetek/essence/unique_id/controller.hpp>
#include <rubetek/client/local/unit_back_interface.hpp>


namespace rubetek {
namespace client  {

    struct local_unit
        : msw::noncopyable
    {
        local_unit  (
                          boost::asio::io_service&
                        , unique_client_id::const_reference
                        , local_unit_back_interface*
                    )                                       ;

    private:
        typedef std::shared_ptr<local_controller_connection> local_connection_ptr;

        logger                                     const logger_            ;
        unique_client_id::type                     const client_id_         ;
        local_unit_back_interface*                 const back_interface_    ;
        local_link_set<unique_controller_id::type>       local_links_       ;
        std::list<local_connection_ptr>                  local_connections_ ;
        beacon                                           beacon_            ;
    };

}}


namespace rubetek {
namespace client  {

    inline local_unit::local_unit
    (
          boost::asio::io_service&          io_service
        , unique_client_id::const_reference client_id
        , local_unit_back_interface*        back_interface
    )
        : logger_         ( "local unit", log::level::info )
        , client_id_      ( client_id                      )
        , back_interface_ ( back_interface                 )
        , local_links_
        (
            [this](unique_controller_id::const_reference id, local_link_selector* link_selector)
            {
                logger_.info("added controller: ", id);
                bool const i_have_you = back_interface_->local_unit_on_check_project(id);
                link_selector->set_on_received([this, id, link_selector, i_have_you](msw::range<msw::byte const> packet)
                {
                    bool const he_has_me = packet.as<bool>();
                    if (i_have_you && !he_has_me) back_interface_->local_unit_on_remove_project(id);
                    back_interface_->local_unit_on_add_controller(id, link_selector, he_has_me);
                });
                link_selector->send(msw::make_range<msw::byte>(i_have_you));
            }
            , [this](unique_controller_id::const_reference id)
            {
                logger_.info("removed controller: ", id);
                back_interface_->local_unit_on_remove_controller(id);
            }
        )
        , beacon_
        (
            io_service
            , [this](boost::asio::ip::tcp::endpoint endpoint)
            {
                for (local_connection_ptr c : local_connections_)
                    if (c->remote_endpoint() == endpoint) return;
                local_connections_.push_back(std::shared_ptr<local_controller_connection>());
                auto i = --local_connections_.end();
                logger_.debug("make connection: ", msw::network_endpoint_to_string(endpoint));
                *i = std::make_shared<local_controller_connection>
                (
                    beacon_.get_io_service()
                    , endpoint
                    , client_id_
                    , local_links_
                    , ihlp::timeouts
                    ({
                          msw::chrono::seconds( 5  )
                        , msw::chrono::seconds( 20 )
                    })
                    , [this, i]()
                    {
                        for (auto j = local_connections_.begin(); j != local_connections_.end(); j++)
                        {
                            if (j == i)
                            {
                                logger_.debug("erase connection: ", msw::network_endpoint_to_string((*i)->remote_endpoint()));
                                local_connections_.erase(i);
                                return;
                            }
                        }
                        logger_.error("erase connection: not found");
                    }
                );
            }
        )
    {}

}}
