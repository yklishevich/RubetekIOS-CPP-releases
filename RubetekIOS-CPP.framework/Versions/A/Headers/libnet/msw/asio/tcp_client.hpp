#pragma once

#include <msw/asio/tcp_session.hpp>
#include <msw/zzz/asio/tcp_client_base.hpp>


namespace msw
{
    struct tcp_client
        : zzz::tcp_client_base<tcp_session>
    {
        template <typename QueryEndpoint>
             tcp_client             (
                                          boost::asio::io_service&
                                        , QueryEndpoint
                                        , on_connecting
                                        , on_connection_timeout
                                        , on_connected
                                        , on_received
                                        , on_sent                                      = nullptr
                                        , on_disconnected                              = nullptr
                                        , duration                  reconnect_delay    = default_reconnect_delay    ()
                                        , boost::optional<duration> connection_timeout = default_connection_timeout ()
                                        , boost::optional<ip_dscp>  option_dscp        = default_option_dscp        ()
                                    )                    ;
    private:
        void connection_established (tcp_socket&&) final ;
    };
}


namespace msw
{
    template <typename QueryEndpoint>
    inline tcp_client::tcp_client
    (
          boost::asio::io_service&  io_service
        , QueryEndpoint             query_endpoint
        , on_connecting             on_connecting
        , on_connection_timeout     on_connection_timeout
        , on_connected              on_connected
        , on_received               on_received
        , on_sent                   on_sent
        , on_disconnected           on_disconnected
        , duration                  reconnect_delay
        , boost::optional<duration> connection_timeout
        , boost::optional<ip_dscp>  option_dscp
    )
        : zzz::tcp_client_base<tcp_session>
        (
              io_service
            , query_endpoint
            , on_connecting
            , on_connection_timeout
            , on_connected
            , on_received
            , on_sent
            , on_disconnected
            , reconnect_delay
            , connection_timeout
            , option_dscp
        )
    {}
    inline void tcp_client::connection_established(tcp_socket&& socket)
    {
        create_session(std::move(socket));
    }
}
