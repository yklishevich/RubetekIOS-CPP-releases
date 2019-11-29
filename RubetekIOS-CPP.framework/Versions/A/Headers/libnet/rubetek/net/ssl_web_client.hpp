#pragma once

#include <string>

#include <msw/noncopyable.hpp>
#include <msw/callback/event.hpp>
#include <msw/asio/ssl_web_client.hpp>
#include <msw/asio/safe_callback.hpp>
#include <msw/asio/ssl_extract_verifying_subject_name.hpp>
#include <msw/lexical_cast/network/query/to_string.hpp>
#include <msw/lexical_cast/network/endpoint/to_string.hpp>

#include <rubetek/log.hpp>


namespace rubetek
{
    struct ssl_web_client
        : msw::noncopyable
    {
        typedef msw::ssl_web_client::query          query              ;
        typedef msw::ssl_web_client::on_ssl_verify  on_verify          ;
        typedef msw::ssl_web_client::on_received    on_received        ;
        typedef msw::callback::event                on_connected       ;
        typedef msw::callback::event                on_disconnected    ;
        typedef msw::callback::event                on_connection_fail ;

             ssl_web_client       (
                                        boost::asio::io_service&
                                      , query
                                      , std::string                   resource
                                      , on_received
                                      , on_connected                              = nullptr
                                      , on_disconnected                           = nullptr
                                      , on_connection_fail                        = nullptr
                                      , boost::optional<msw::ip_dscp> option_dscp = msw::ssl_web_client::default_option_dscp()
                                  )                                     ;

             ssl_web_client       (
                                        boost::asio::io_service&
                                        , query
                                        , on_verify
                                        , std::string                   resource
                                        , on_received
                                        , on_connected                              = nullptr
                                        , on_disconnected                           = nullptr
                                        , on_connection_fail                        = nullptr
                                        , boost::optional<msw::ip_dscp> option_dscp = msw::ssl_web_client::default_option_dscp()
                                   )                                     ;

        void send                 (msw::range<msw::byte const>)         ;
        bool connected            ()                              const ;
        void set_verify_mode      (boost::asio::ssl::verify_mode)       ;
        void load_verify_file     (std::string const&)                  ;
        void default_verify_paths ()                                    ;
        void use_certificate_file(std::string const&, boost::asio::ssl::context_base::file_format);
        void use_private_key_file(std::string const&, boost::asio::ssl::context_base::file_format);
        void reset                ()                                    ;

    private:
        logger                      const logger_         ;
        logger_io<log_io_tags::web> const logger_io_      ;
        msw::ssl_web_client               ssl_web_client_ ;
    };
}


namespace rubetek
{
    inline ssl_web_client::ssl_web_client
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
        : ssl_web_client
        (
            io_service
            , query
            , msw::ssl_extract_verifying_subject_name
            (
                static_cast<msw::ssl_extract_verifying_subject_name::handler_preverified>
                (
                    [this](bool preverified, std::string subject_name) -> bool
                    {
                        logger_.debug("verifying ", subject_name, ", preverified status: ", (preverified ? "yes" : "no"));
                        return true;
                    }
                )
            )
            , resource
            , on_received
            , on_connected
            , on_disconnected
            , on_connection_fail
            , option_dscp
        )
    {}

    inline ssl_web_client::ssl_web_client
    (
          boost::asio::io_service&      io_service
        , query                         query
        , on_verify                     on_verify
        , std::string                   resource
        , on_received                   on_received
        , on_connected                  on_connected
        , on_disconnected               on_disconnected
        , on_connection_fail            on_connection_fail
        , boost::optional<msw::ip_dscp> option_dscp
    )
        : logger_    ( "web client (ssl)", msw::network_query_to_string(query) )
        , logger_io_ ( logger_                                                 )
        , ssl_web_client_
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
            , on_verify
            , [this, on_connection_fail]()
            {
                logger_.debug("waiting time for ssl handshake has expired");
                if (on_connection_fail) MSW_ASIO_SAFE_CALLBACK(on_connection_fail);
            }
            , [this, on_connected, on_connection_fail](boost::system::error_code ec)
            {
                if (ec)
                {
                    logger_.warning("ssl handshake fail: ", ec.message());
                    if (on_connection_fail) MSW_ASIO_SAFE_CALLBACK(on_connection_fail);
                }
                else
                {
                    logger_.debug("ssl handshake successfully completed");
                }
            }
            , [this, on_connection_fail]()
            {
                logger_.debug("waiting time for web handshake has expired");
                if (on_connection_fail) MSW_ASIO_SAFE_CALLBACK(on_connection_fail);
            }
            , [this, on_connected](boost::system::error_code ec)
            {
                if (ec) logger_.warning("web handshake fail: ", ec.message());
                else
                {
                    logger_.debug("web handshake successfully completed");
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
            , msw::ssl_web_client::default_reconnect_delay    ()
            , msw::ssl_web_client::default_connection_timeout ()
            , msw::ssl_client::default_handshake_timeout      ()
            , msw::web_client::default_handshake_timeout      ()
            , option_dscp
        )
    {
        logger_.info("resource: ", resource);
    }
    inline void ssl_web_client::send(msw::range<msw::byte const> packet)
    {
        logger_io_.send(packet);
        ssl_web_client_.send(packet);
    }
    inline bool ssl_web_client::connected() const
    {
        return ssl_web_client_.connected();
    }
    inline void ssl_web_client::set_verify_mode(boost::asio::ssl::verify_mode mode)
    {
        ssl_web_client_.set_verify_mode(mode);
    }
    inline void ssl_web_client::load_verify_file(std::string const& filename)
    {
        ssl_web_client_.load_verify_file(filename);
    }
    inline void ssl_web_client::default_verify_paths()
    {
        ssl_web_client_.default_verify_paths();
    }
    inline void ssl_web_client::use_certificate_file(std::string const& filename, boost::asio::ssl::context_base::file_format format)
    {
        ssl_web_client_.use_certificate_file(filename, format);
    }
    inline void ssl_web_client::use_private_key_file(std::string const& filename, boost::asio::ssl::context_base::file_format format)
    {
        ssl_web_client_.use_private_key_file(filename, format);
    }
    inline void ssl_web_client::reset()
    {
        logger_.info("reset");
        ssl_web_client_.reset();
    }
}
