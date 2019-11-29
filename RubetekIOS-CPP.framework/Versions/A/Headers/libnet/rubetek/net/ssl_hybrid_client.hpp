#pragma once

#include <msw/noncopyable.hpp>
#include <msw/callback/event.hpp>
#include <msw/asio/ssl_hybrid_client.hpp>
#include <msw/lexical_cast/network/query/to_string.hpp>
#include <msw/lexical_cast/network/endpoint/to_string.hpp>
#include <msw/asio/ssl_extract_verifying_subject_name.hpp>
#include <msw/asio/safe_callback.hpp>

#include <rubetek/log.hpp>


namespace rubetek
{
    struct ssl_hybrid_client
        : msw::noncopyable
    {
        typedef msw::ssl_hybrid_client::query       query              ;
        typedef msw::ssl_hybrid_client::on_received on_received        ;
        typedef msw::callback::event                on_connected       ;
        typedef msw::callback::event                on_disconnected    ;
        typedef msw::callback::event                on_connection_fail ;

             ssl_hybrid_client (
                                     boost::asio::io_service&
                                   , query
                                   , on_received
                                   , on_connected                     = nullptr
                                   , on_disconnected                  = nullptr
                                   , on_connection_fail               = nullptr
                                   , bool                     use_ssl = true
                               )                                                      ;

        void send              (msw::range<msw::byte const>)                          ;
        bool connected         ()                                               const ;
        void reset             ()                                                     ;

    private:
        logger                      const logger_            ;
        logger_io<log_io_tags::tcp> const logger_io_         ;
        bool                        const use_ssl_           ;
        msw::ssl_hybrid_client            ssl_hybrid_client_ ;
    };
}


namespace rubetek
{
    inline ssl_hybrid_client::ssl_hybrid_client
    (
          boost::asio::io_service& io_service
        , query                    query
        , on_received              on_received
        , on_connected             on_connected
        , on_disconnected          on_disconnected
        , on_connection_fail       on_connection_fail
        , bool                     use_ssl
    )
        : logger_    ( use_ssl ? "ssl client" : "tcp client", msw::network_query_to_string(query) )
        , logger_io_ ( logger_                                                                    )
        , use_ssl_   ( use_ssl                                                                    )
        , ssl_hybrid_client_
        (
            io_service
            , query
            , [this](msw::tcp_session::endpoint_type endpoint)
            {
                logger_.debug("connecting to: ", msw::network_endpoint_to_string(endpoint));
            }
            , [this, on_connection_fail]()
            {
                logger_.debug("waiting time for connection has expired");
                if (on_connection_fail) MSW_ASIO_SAFE_CALLBACK(on_connection_fail);
            }
            , [this, on_connected, on_connection_fail, use_ssl](boost::system::error_code ec)
            {
                if (ec)
                {
                    logger_.debug("connect fail: ", ec.message());
                    if (on_connection_fail) MSW_ASIO_SAFE_CALLBACK(on_connection_fail);
                }
                else
                {
                    logger_.debug("successfully connected");
                    if (use_ssl)
                    {
                        logger_.debug("start handshake");
                        ssl_hybrid_client_.handshake();
                    }
                    else if (on_connected) MSW_ASIO_SAFE_CALLBACK(on_connected);
                }
            }
            , msw::ssl_extract_verifying_subject_name
            (
                static_cast<msw::ssl_extract_verifying_subject_name::handler>
                (
                    [this](std::string subject_name)
                    {
                        logger_.debug("verifying ", subject_name);
                    }
                )
            )
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
        )
    {}
    inline void ssl_hybrid_client::send(msw::range<msw::byte const> packet)
    {
        logger_io_.send(packet);
        ssl_hybrid_client_.send(packet);
    }
    inline bool ssl_hybrid_client::connected() const
    {
        return use_ssl_ ? ssl_hybrid_client_.secured() : ssl_hybrid_client_.connected();
    }
    inline void ssl_hybrid_client::reset()
    {
        return ssl_hybrid_client_.reset();
    }
}
