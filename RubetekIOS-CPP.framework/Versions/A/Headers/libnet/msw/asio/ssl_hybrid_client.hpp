#pragma once

#include <memory>

#include <msw/asio/ssl_client.hpp>
#include <msw/asio/tcp_session.hpp>


namespace msw
{
    struct ssl_hybrid_client
        : ssl_client
    {
        typedef ssl_client::on_verify              on_verify              ;
        typedef ssl_client::on_handshake_timeout   on_handshake_timeout   ;
        typedef ssl_client::on_handshake_completed on_handshake_completed ;

             ssl_hybrid_client      (
                                          boost::asio::io_service&
                                        , query
                                        , on_connecting
                                        , on_connection_timeout
                                        , on_connected
                                        , on_verify
                                        , on_handshake_timeout
                                        , on_handshake_completed
                                        , on_received
                                        , on_sent                                      = nullptr
                                        , on_disconnected                              = nullptr
                                        , duration                  reconnect_delay    = default_reconnect_delay    ()
                                        , boost::optional<duration> connection_timeout = default_connection_timeout ()
                                        , boost::optional<duration> handshake_timeout  = default_handshake_timeout  ()
                                    )                               ;
        bool connected              ()                  const       ;
        bool secured                ()                  const       ;
        void handshake              ()                              ;
        void send                   (range<byte const>)             ;
        void reset                  ()                              ;
    private:
        void connection_established (tcp_socket&&)            final ;

        std::unique_ptr<tcp_session> tcp_session_;
    };
}


namespace msw
{
    inline ssl_hybrid_client::ssl_hybrid_client
    (
          boost::asio::io_service&  io_service
        , query                     query
        , on_connecting             on_connecting
        , on_connection_timeout     on_connection_timeout
        , on_connected              on_connected
        , on_verify                 on_verify
        , on_handshake_timeout      on_handshake_timeout
        , on_handshake_completed    on_handshake_completed
        , on_received               on_received
        , on_sent                   on_sent
        , on_disconnected           on_disconnected
        , duration                  reconnect_delay
        , boost::optional<duration> connection_timeout
        , boost::optional<duration> handshake_timeout
    )
        : ssl_client
        (
              io_service
            , query
            , on_connecting
            , on_connection_timeout
            , on_connected
            , on_verify
            , on_handshake_timeout
            , on_handshake_completed
            , on_received
            , on_sent
            , on_disconnected
            , reconnect_delay
            , connection_timeout
            , handshake_timeout
        )
    {}
    inline bool ssl_hybrid_client::connected() const
    {
        return static_cast<bool>(tcp_session_) || secured();
    }
    inline bool ssl_hybrid_client::secured() const
    {
        return ssl_client::connected();
    }
    inline void ssl_hybrid_client::handshake()
    {
        tcp_session_->eject_socket
        (
            [&](tcp_socket tcp_socket)
            {
                tcp_session_.reset();
                start_handshaker(std::move(tcp_socket));
            }
        );
    }
    inline void ssl_hybrid_client::connection_established(tcp_socket&& tcp_socket)
    {
        create_session<tcp_session>(tcp_session_, std::move(tcp_socket));
    }
    inline void ssl_hybrid_client::send(range<byte const> packet)
    {
        if (tcp_session_) tcp_session_->send(packet);
        else ssl_client::send(packet);
    }
    inline void ssl_hybrid_client::reset()
    {
        tcp_session_.reset();
        ssl_client::reset();
    }
}
