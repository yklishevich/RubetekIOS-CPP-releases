#pragma once

#include <string>

#include <msw/noncopyable.hpp>
#include <msw/callback/event.hpp>
#include <msw/asio/web_client.hpp>
#include <msw/asio/safe_callback.hpp>
#include <msw/lexical_cast/network/query/to_string.hpp>
#include <msw/lexical_cast/network/endpoint/to_string.hpp>

#include <rubetek/log.hpp>


namespace rubetek
{
    struct web_client
        : msw::noncopyable
    {
        typedef msw::web_client::query       query              ;
        typedef msw::web_client::on_received on_received        ;
        typedef msw::callback::event         on_connected       ;
        typedef msw::callback::event         on_disconnected    ;
        typedef msw::callback::event         on_connection_fail ;

             web_client (
                              boost::asio::io_service&
                            , query
                            , std::string                   resource
                            , on_received
                            , on_connected                              = nullptr
                            , on_disconnected                           = nullptr
                            , on_connection_fail                        = nullptr
                            , boost::optional<msw::ip_dscp> option_dscp = msw::web_client::default_option_dscp()
                        ) ;
             web_client (
                              boost::asio::io_service&
                            , query
                            , on_received
                            , on_connected                              = nullptr
                            , on_disconnected                           = nullptr
                            , on_connection_fail                        = nullptr
                            , boost::optional<msw::ip_dscp> option_dscp = msw::web_client::default_option_dscp()
                        ) ;

        void send      (msw::range<msw::byte const>)       ;
        bool connected ()                            const ;
        void reset     ()                                  ;

    private:
        logger                      const logger_     ;
        logger_io<log_io_tags::web> const logger_io_  ;
        msw::web_client                   web_client_ ;
    };
}


namespace rubetek
{
    inline web_client::web_client
    (
          boost::asio::io_service&      io_service
        , query                         query
        , std::string                   resource
        , on_received                   on_received
        , on_connected                  on_connected
        , on_disconnected               on_disconnected
        , on_connection_fail            on_connection_fail
        , boost::optional<msw::ip_dscp> option_dscp
    )
        : logger_    ( "web client", msw::network_query_to_string(query) )
        , logger_io_ ( logger_                                           )
        , web_client_
        (
            io_service
            , query
            , resource
            , [this](msw::tcp_session::endpoint_type endpoint)
            {
                logger_.debug("connecting to: ", msw::network_endpoint_to_string(endpoint));
            }
            , [this, on_connection_fail]()
            {
                logger_.debug("waiting time for connection has expired");
                if (on_connection_fail) MSW_ASIO_SAFE_CALLBACK(on_connection_fail);
            }
            , [this, on_connection_fail](boost::system::error_code ec)
            {
                if (ec)
                {
                    logger_.debug("connect fail: ", ec.message());
                    if (on_connection_fail) MSW_ASIO_SAFE_CALLBACK(on_connection_fail);
                }
                else
                {
                    logger_.debug("successfully connected to host");
                }
            }
            , [this, on_connection_fail]()
            {
                logger_.debug("waiting time for handshake has expired");
                if (on_connection_fail) MSW_ASIO_SAFE_CALLBACK(on_connection_fail);
            }
            , [this, on_connected](boost::system::error_code ec)
            {
                if (ec) logger_.warning("handshake fail: ", ec.message());
                else
                {
                    logger_.debug("handshake successfully completed");
                    if (on_connected) MSW_ASIO_SAFE_CALLBACK(on_connected);
                }
            }
            , [this, on_received](msw::range<msw::byte> packet)
            {
                logger_io_.received(packet);
                MSW_ASIO_SAFE_CALLBACK(on_received, packet);
            }
            , [this](msw::range<msw::byte> packet)
            {
                logger_io_.sent(packet);
            }
            , [this, on_disconnected](boost::system::error_code ec)
            {
                logger_.debug("disconnected: ", ec.message());
                if (on_disconnected) MSW_ASIO_SAFE_CALLBACK(on_disconnected);
            }
            , msw::web_client::default_reconnect_delay    ()
            , msw::web_client::default_connection_timeout ()
            , msw::web_client::default_handshake_timeout  ()
            , option_dscp
        )
    {
        logger_.info("resource: ", resource);
    }
    inline web_client::web_client
    (
          boost::asio::io_service&      io_service
        , query                         query
        , on_received                   on_received
        , on_connected                  on_connected
        , on_disconnected               on_disconnected
        , on_connection_fail            on_connection_fail
        , boost::optional<msw::ip_dscp> option_dscp
    )
        : web_client
        (
              io_service
            , query
            , "/"
            , on_received
            , on_connected
            , on_disconnected
            , on_connection_fail
            , option_dscp
        )
    {}
    inline void web_client::send(msw::range<msw::byte const> packet)
    {
        logger_io_.send(packet);
        web_client_.send(packet);
    }
    inline bool web_client::connected() const
    {
        return web_client_.connected();
    }
    inline void web_client::reset()
    {
        logger_.info("reset");
        web_client_.reset();
    }
}
