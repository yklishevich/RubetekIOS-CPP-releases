#pragma once

#include <string>
#include <functional>

#include <msw/range.hpp>
#include <msw/noncopyable.hpp>
#include <msw/callback/event.hpp>
#include <msw/lexical_cast/bytes_as_string.hpp>

#include <rubetek/log.hpp>
#include <rubetek/net/ihlp_connection.hpp>
#include <rubetek/essence/unique_id/client.hpp>
#include <rubetek/essence/unique_id/controller.hpp>
#include <rubetek/local_link/set.hpp>
#include <rubetek/local_link/session_interface.hpp>


namespace rubetek {
namespace client  {

    struct local_controller_connection
        : local_link_session_interface
    {
        typedef msw::callback::event                       on_close            ;
        typedef boost::asio::ip::tcp::endpoint             endpoint            ;
        typedef local_link_set<unique_controller_id::type> local_link_set_type ;

                    local_controller_connection  (
                                                       boost::asio::io_service&
                                                     , endpoint
                                                     , unique_client_id::const_reference
                                                     , local_link_set_type&
                                                     , ihlp::timeouts
                                                     , on_close
                                                 )                                         ;
                    ~local_controller_connection ()                                        ;
        endpoint    remote_endpoint              ()                            const       ;
    private:
        void        send                         (msw::range<msw::byte const>)       final ;
        std::string ip_address                   ()                            const final ;

        void        process_incoming_packet      (msw::range<msw::byte const>)             ;
        void        send_to_ihlp                 (msw::range<msw::byte const>)             ;

        logger                      const logger_          ;
        endpoint                    const remote_endpoint_ ;
        local_link_set_type&              local_link_set_  ;
        on_received                       on_received_     ;
        ihlp_connection<msw::wbyte>       ihlp_connection_ ;
    };

}}


namespace rubetek {
namespace client  {

    inline local_controller_connection::local_controller_connection
    (
          boost::asio::io_service&          io_service
        , endpoint                          rmt_endpoint
        , unique_client_id::const_reference client_id
        , local_link_set_type&              local_link_set
        , ihlp::timeouts                    ihlp_timeouts
        , on_close                          on_close
    )
        : logger_          (
                                  "local controller connection"
                                , msw::network_endpoint_to_string(rmt_endpoint)
                                , log::level::debug
                           )
        , remote_endpoint_ ( rmt_endpoint   )
        , local_link_set_  ( local_link_set )
        , ihlp_connection_
        (
            io_service
            , rmt_endpoint
            , ihlp_timeouts
            , [this, client_id]()
            {
                send(msw::make_range<msw::byte>(client_id));
            }
            , [this](msw::range<msw::byte const> packet)
            {
                if (on_received_) on_received_(packet);
                else on_received_ = local_link_set_.add_link(this, msw::bytes_as_string(packet));
                MSW_ASSERT(on_received_);
            }
            , on_close
        )
    {}
    inline local_controller_connection::~local_controller_connection()
    {
        if (on_received_) local_link_set_.remove_link(this);
    }

    inline local_controller_connection::endpoint local_controller_connection::remote_endpoint() const
    {
        return remote_endpoint_;
    }

    inline void local_controller_connection::send(msw::range<msw::byte const> packet)
    {
        if (ihlp_connection_.connected()) ihlp_connection_.send(packet);
        else logger_.error("send fail: ihlp connection disconnected");
    }
    inline std::string local_controller_connection::ip_address() const
    {
        return remote_endpoint_.address().to_string();
    }

}}
