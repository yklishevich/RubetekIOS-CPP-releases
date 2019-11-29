#pragma once

#include <msw/std/memory.hpp>

#include <rubetek/net/web_client.hpp>
#include <rubetek/net/ssl_web_client.hpp>


namespace rubetek
{
    struct ssl_hybrid_web_client
        : msw::noncopyable
    {
        typedef web_client::query              query              ;
        typedef web_client::on_received        on_received        ;
        typedef web_client::on_connected       on_connected       ;
        typedef web_client::on_disconnected    on_disconnected    ;
        typedef web_client::on_connection_fail on_connection_fail ;
        typedef ssl_web_client::on_verify      on_verify          ;

             ssl_hybrid_web_client (
                                         boost::asio::io_service&
                                       , query
                                       , on_verify
                                       , std::string                   resource
                                       , on_received
                                       , on_connected                              = nullptr
                                       , on_disconnected                           = nullptr
                                       , on_connection_fail                        = nullptr
                                       , bool                          use_ssl     = true
                                       , boost::optional<msw::ip_dscp> option_dscp = msw::web_client::default_option_dscp()
                                   )                                     ;

            ssl_hybrid_web_client (
                                        boost::asio::io_service&
                                        , query
                                        , std::string                   resource
                                        , on_received
                                        , on_connected                              = nullptr
                                        , on_disconnected                           = nullptr
                                        , on_connection_fail                        = nullptr
                                        , bool                          use_ssl     = true
                                        , boost::optional<msw::ip_dscp> option_dscp = msw::web_client::default_option_dscp()
                                    )                                     ;

        void send                  (msw::range<msw::byte const>)         ;
        bool connected             ()                              const ;
        void set_verify_mode       (boost::asio::ssl::verify_mode)       ;
        void load_verify_file      (std::string const&)                  ;
        void default_verify_paths  ()                                    ;
        void use_certificate_file(std::string const&, boost::asio::ssl::context_base::file_format);
        void use_private_key_file(std::string const&, boost::asio::ssl::context_base::file_format);
        void reset                 ()                                    ;

    private:
        std::unique_ptr<web_client>     web_client_     ;
        std::unique_ptr<ssl_web_client> ssl_web_client_ ;
    };
}


namespace rubetek
{
    inline ssl_hybrid_web_client::ssl_hybrid_web_client
        (
            boost::asio::io_service&      io_service
            , query                         query
            , on_verify                     on_verify
            , std::string                   resource
            , on_received                   on_received
            , on_connected                  on_connected
            , on_disconnected               on_disconnected
            , on_connection_fail            on_connection_fail
            , bool                          use_ssl
            , boost::optional<msw::ip_dscp> option_dscp
        )
    {
        if (use_ssl)
        {
            ssl_web_client_ = msw::make_unique<ssl_web_client>
                (
                    io_service
                    , query
                    , on_verify
                    , resource
                    , on_received
                    , on_connected
                    , on_disconnected
                    , on_connection_fail
                    , option_dscp
                );
        }
        else
        {
            web_client_ = msw::make_unique<web_client>
                (
                    io_service
                    , query
                    , resource
                    , on_received
                    , on_connected
                    , on_disconnected
                    , on_connection_fail
                    , option_dscp
                );
        }
    }

    inline ssl_hybrid_web_client::ssl_hybrid_web_client
    (
          boost::asio::io_service&      io_service
        , query                         query
        , std::string                   resource
        , on_received                   on_received
        , on_connected                  on_connected
        , on_disconnected               on_disconnected
        , on_connection_fail            on_connection_fail
        , bool                          use_ssl
        , boost::optional<msw::ip_dscp> option_dscp
    )
    {
        if (use_ssl)
        {
            ssl_web_client_ = msw::make_unique<ssl_web_client>
            (
                io_service
                , query
                , resource
                , on_received
                , on_connected
                , on_disconnected
                , on_connection_fail
                , option_dscp
            );
        }
        else
        {
            web_client_ = msw::make_unique<web_client>
            (
                  io_service
                , query
                , resource
                , on_received
                , on_connected
                , on_disconnected
                , on_connection_fail
                , option_dscp
            );
        }
    }
    inline void ssl_hybrid_web_client::send(msw::range<msw::byte const> packet)
    {
        if (web_client_) web_client_    ->send(packet);
        else             ssl_web_client_->send(packet);
    }
    inline bool ssl_hybrid_web_client::connected() const
    {
        return web_client_ ? web_client_->connected() : ssl_web_client_->connected();
    }
    inline void ssl_hybrid_web_client::set_verify_mode(boost::asio::ssl::verify_mode mode)
    {
        if (ssl_web_client_) ssl_web_client_->set_verify_mode(mode);
    }
    inline void ssl_hybrid_web_client::load_verify_file(std::string const& filename)
    {
        if (ssl_web_client_) ssl_web_client_->load_verify_file(filename);
    }
    inline void ssl_hybrid_web_client::default_verify_paths()
    {
        if (ssl_web_client_) ssl_web_client_->default_verify_paths();
    }
    inline void ssl_hybrid_web_client::use_certificate_file(std::string const& filename, boost::asio::ssl::context_base::file_format format)
    {
        if (ssl_web_client_) ssl_web_client_->use_certificate_file(filename, format);
    }
    inline void ssl_hybrid_web_client::use_private_key_file(std::string const& filename, boost::asio::ssl::context_base::file_format format)
    {
        if (ssl_web_client_) ssl_web_client_->use_private_key_file(filename, format);
    }
    inline void ssl_hybrid_web_client::reset()
    {
        if (web_client_) web_client_    ->reset();
        else             ssl_web_client_->reset();
    }
}
