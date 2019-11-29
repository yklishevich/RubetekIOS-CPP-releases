#pragma once

#include <memory>
#include <string>

#include <boost/optional.hpp>

#include <msw/std/memory.hpp>
#include <msw/asio/timer.hpp>
#include <msw/asio/ssl_client.hpp>
#include <msw/asio/web_client.hpp>
#include <msw/zzz/asio/on_error.hpp>
#include <msw/asio/ssl_handshaker.hpp>
#include <msw/asio/ssl_web_session.hpp>
#include <msw/asio/ssl_web_handshaker.hpp>
#include <msw/zzz/asio/tcp_client_base.hpp>


namespace msw
{
    struct ssl_web_client
        : zzz::tcp_client_base<ssl_web_session>
    {
        typedef ssl_client::on_verify               on_ssl_verify              ;
        typedef ssl_client::on_handshake_timeout    on_ssl_handshake_timeout   ;
        typedef ssl_client::on_handshake_completed  on_ssl_handshake_completed ;
        typedef web_client::on_handshake_timeout    on_web_handshake_timeout   ;
        typedef web_client::on_handshake_completed  on_web_handshake_completed ;
        typedef msw::chrono::steady_clock::duration duration                   ;

                                          ssl_web_client         (
                                                                       boost::asio::io_service&
                                                                     , query
                                                                     , std::string const& host
                                                                     , std::string const& resource
                                                                     , on_connecting
                                                                     , on_connection_timeout
                                                                     , on_connected
                                                                     , on_ssl_verify
                                                                     , on_ssl_handshake_timeout
                                                                     , on_ssl_handshake_completed
                                                                     , on_web_handshake_timeout
                                                                     , on_web_handshake_completed
                                                                     , on_received
                                                                     , on_sent                                         = nullptr
                                                                     , on_disconnected                                 = nullptr
                                                                     , duration                  reconnect_delay       = default_reconnect_delay               ()
                                                                     , boost::optional<duration> connection_timeout    = default_connection_timeout            ()
                                                                     , boost::optional<duration> ssl_handshake_timeout = ssl_client::default_handshake_timeout ()
                                                                     , boost::optional<duration> web_handshake_timeout = web_client::default_handshake_timeout ()
                                                                     , boost::optional<ip_dscp>  option_dscp           = default_option_dscp                   ()
                                                                 )                                                                       ;
                                          ssl_web_client         (
                                                                       boost::asio::io_service&
                                                                     , query
                                                                     , std::string const& resource
                                                                     , on_connecting
                                                                     , on_connection_timeout
                                                                     , on_connected
                                                                     , on_ssl_verify
                                                                     , on_ssl_handshake_timeout
                                                                     , on_ssl_handshake_completed
                                                                     , on_web_handshake_timeout
                                                                     , on_web_handshake_completed
                                                                     , on_received
                                                                     , on_sent                                         = nullptr
                                                                     , on_disconnected                                 = nullptr
                                                                     , duration                  reconnect_delay       = default_reconnect_delay               ()
                                                                     , boost::optional<duration> connection_timeout    = default_connection_timeout            ()
                                                                     , boost::optional<duration> ssl_handshake_timeout = ssl_client::default_handshake_timeout ()
                                                                     , boost::optional<duration> web_handshake_timeout = web_client::default_handshake_timeout ()
                                                                     , boost::optional<ip_dscp>  option_dscp           = default_option_dscp                   ()
                                                                 )                                                                       ;
                                          ssl_web_client         (
                                                                       boost::asio::io_service&
                                                                     , query
                                                                     , on_connecting
                                                                     , on_connection_timeout
                                                                     , on_connected
                                                                     , on_ssl_verify
                                                                     , on_ssl_handshake_timeout
                                                                     , on_ssl_handshake_completed
                                                                     , on_web_handshake_timeout
                                                                     , on_web_handshake_completed
                                                                     , on_received
                                                                     , on_sent                                         = nullptr
                                                                     , on_disconnected                                 = nullptr
                                                                     , duration                  reconnect_delay       = default_reconnect_delay               ()
                                                                     , boost::optional<duration> connection_timeout    = default_connection_timeout            ()
                                                                     , boost::optional<duration> ssl_handshake_timeout = ssl_client::default_handshake_timeout ()
                                                                     , boost::optional<duration> web_handshake_timeout = web_client::default_handshake_timeout ()
                                                                     , boost::optional<ip_dscp>  option_dscp           = default_option_dscp                   ()
                                                                 )                                        ;
                boost::optional<duration> ssl_handshake_timeout  ()                              const    ;
                boost::optional<duration> web_handshake_timeout  ()                              const    ;
                void                      set_verify_mode        (boost::asio::ssl::verify_mode)          ;
                void                      load_verify_file       (std::string const&)                     ;
                void                      default_verify_paths   ()                                       ;
                void use_certificate_file(std::string const&, boost::asio::ssl::context_base::file_format);
                void use_private_key_file(std::string const&, boost::asio::ssl::context_base::file_format);
                void                      reset                  ()                                       ;
    protected:
                void                      start_ssl_handshaker   (tcp_socket&&)                           ;
                void                      start_web_handshaker   (ssl_socket&&)                           ;
    private:
        virtual void                      connection_established (tcp_socket&&)                  override ;

        std::string                     const host_                       ;
        std::string                     const resource_                   ;
        boost::optional<duration>       const ssl_handshake_timeout_      ;
        boost::optional<duration>       const web_handshake_timeout_      ;
        on_ssl_verify                   const on_ssl_verify_              ;
        on_ssl_handshake_timeout        const on_ssl_handshake_timeout_   ;
        on_ssl_handshake_completed      const on_ssl_handshake_completed_ ;
        on_web_handshake_timeout        const on_web_handshake_timeout_   ;
        on_web_handshake_completed      const on_web_handshake_completed_ ;
        ssl_context                           ssl_context_                ;
        std::unique_ptr<ssl_handshaker>       ssl_handshaker_             ;
        std::unique_ptr<ssl_web_handshaker>   ssl_web_handshaker_         ;
        std::unique_ptr<msw::timer>           timer_                      ;
    };
}


namespace msw
{
    inline ssl_web_client::ssl_web_client
    (
          boost::asio::io_service&   io_service
        , query                      query
        , std::string const&         host
        , std::string const&         resource
        , on_connecting              on_connecting
        , on_connection_timeout      on_connection_timeout
        , on_connected               on_connected
        , on_ssl_verify              on_ssl_verify
        , on_ssl_handshake_timeout   on_ssl_handshake_timeout
        , on_ssl_handshake_completed on_ssl_handshake_completed
        , on_web_handshake_timeout   on_web_handshake_timeout
        , on_web_handshake_completed on_web_handshake_completed
        , on_received                on_received
        , on_sent                    on_sent
        , on_disconnected            on_disconnected
        , duration                   reconnect_delay
        , boost::optional<duration>  connection_timeout
        , boost::optional<duration>  ssl_handshake_timeout
        , boost::optional<duration>  web_handshake_timeout
        , boost::optional<ip_dscp>   option_dscp
    )
        : zzz::tcp_client_base<ssl_web_session>
        (
            io_service
            , query
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
        , host_                       ( host                                          )
        , resource_                   ( resource                                      )
        , ssl_handshake_timeout_      ( ssl_handshake_timeout                         )
        , web_handshake_timeout_      ( web_handshake_timeout                         )
        , on_ssl_verify_              ( on_ssl_verify                                 )
        , on_ssl_handshake_timeout_   ( on_ssl_handshake_timeout                      )
        , on_ssl_handshake_completed_ ( on_ssl_handshake_completed                    )
        , on_web_handshake_timeout_   ( on_web_handshake_timeout                      )
        , on_web_handshake_completed_ ( on_web_handshake_completed                    )
        , ssl_context_                ( io_service, boost::asio::ssl::context::sslv23 )
    {}
    inline ssl_web_client::ssl_web_client
    (
          boost::asio::io_service&   io_service
        , query                      query
        , std::string const&         resource
        , on_connecting              on_connecting
        , on_connection_timeout      on_connection_timeout
        , on_connected               on_connected
        , on_ssl_verify              on_ssl_verify
        , on_ssl_handshake_timeout   on_ssl_handshake_timeout
        , on_ssl_handshake_completed on_ssl_handshake_completed
        , on_web_handshake_timeout   on_web_handshake_timeout
        , on_web_handshake_completed on_web_handshake_completed
        , on_received                on_received
        , on_sent                    on_sent
        , on_disconnected            on_disconnected
        , duration                   reconnect_delay
        , boost::optional<duration>  connection_timeout
        , boost::optional<duration>  ssl_handshake_timeout
        , boost::optional<duration>  web_handshake_timeout
        , boost::optional<ip_dscp>   option_dscp
    )
        : ssl_web_client
        (
              io_service
            , query
            , query.host_name()
            , resource
            , on_connecting
            , on_connection_timeout
            , on_connected
            , on_ssl_verify
            , on_ssl_handshake_timeout
            , on_ssl_handshake_completed
            , on_web_handshake_timeout
            , on_web_handshake_completed
            , on_received
            , on_sent
            , on_disconnected
            , reconnect_delay
            , connection_timeout
            , ssl_handshake_timeout
            , web_handshake_timeout
            , option_dscp
        )
    {}
    inline ssl_web_client::ssl_web_client
    (
          boost::asio::io_service&   io_service
        , query                      query
        , on_connecting              on_connecting
        , on_connection_timeout      on_connection_timeout
        , on_connected               on_connected
        , on_ssl_verify              on_ssl_verify
        , on_ssl_handshake_timeout   on_ssl_handshake_timeout
        , on_ssl_handshake_completed on_ssl_handshake_completed
        , on_web_handshake_timeout   on_web_handshake_timeout
        , on_web_handshake_completed on_web_handshake_completed
        , on_received                on_received
        , on_sent                    on_sent
        , on_disconnected            on_disconnected
        , duration                   reconnect_delay
        , boost::optional<duration>  connection_timeout
        , boost::optional<duration>  ssl_handshake_timeout
        , boost::optional<duration>  web_handshake_timeout
        , boost::optional<ip_dscp>   option_dscp
    )
        : ssl_web_client
        (
              io_service
            , query
            , "/"
            , on_connecting
            , on_connection_timeout
            , on_connected
            , on_ssl_verify
            , on_ssl_handshake_timeout
            , on_ssl_handshake_completed
            , on_web_handshake_timeout
            , on_web_handshake_completed
            , on_received
            , on_sent
            , on_disconnected
            , reconnect_delay
            , connection_timeout
            , ssl_handshake_timeout
            , web_handshake_timeout
            , option_dscp
        )
    {}
    inline boost::optional<ssl_web_client::duration> ssl_web_client::ssl_handshake_timeout() const
    {
        return ssl_handshake_timeout_;
    }
    inline boost::optional<ssl_web_client::duration> ssl_web_client::web_handshake_timeout() const
    {
        return web_handshake_timeout_;
    }
    inline void ssl_web_client::set_verify_mode(boost::asio::ssl::verify_mode mode)
    {
        ssl_context_.set_verify_mode(mode);
    }
    inline void ssl_web_client::load_verify_file(std::string const& filename)
    {
        ssl_context_.load_verify_file(filename);
    }
    inline void ssl_web_client::default_verify_paths()
    {
        ssl_context_.set_default_verify_paths();
    }

    inline void ssl_web_client::use_certificate_file(std::string const& filename, boost::asio::ssl::context_base::file_format format)
    {
        ssl_context_.use_certificate_file(filename, format);
    }

    inline void ssl_web_client::use_private_key_file(std::string const& filename, boost::asio::ssl::context_base::file_format format)
    {
        ssl_context_.use_private_key_file(filename, format);
    }

    inline void ssl_web_client::reset()
    {
        timer_.reset();
        ssl_handshaker_.reset();
        ssl_web_handshaker_.reset();
        zzz::tcp_client_base<ssl_web_session>::reset();
    }
    inline void ssl_web_client::start_ssl_handshaker(tcp_socket&& tcp_socket)
    {
        ssl_handshaker_= msw::make_unique<ssl_handshaker>
        (
            std::move(tcp_socket)
            , ssl_context_
            , [this](msw::ssl_socket ssl_socket)
            {
                ssl_handshaker_.reset();
                start_web_handshaker(std::move(ssl_socket));
                (on_ssl_handshake_completed(on_ssl_handshake_completed_))(boost::system::error_code());
                // warning: after calling the callback should not have code, you can delete the object
            }
            , on_ssl_verify_
            , [this](boost::system::error_code ec)
            {
                ssl_handshaker_.reset();
                reconnect();
                (on_ssl_handshake_completed(on_ssl_handshake_completed_))(ec);
                // warning: after calling the callback should not have code, you can delete the object
            }
        );
        if (ssl_handshake_timeout_)
        {
            timer_= msw::make_unique<msw::timer>
            (
                io_service()
                , *ssl_handshake_timeout_
                , [this]()
                {
                    timer_.reset();
                    if (ssl_handshaker_)
                    {
                        ssl_handshaker_.reset();
                        reconnect();
                        (on_ssl_handshake_timeout(on_ssl_handshake_timeout_))();
                        // warning: after calling the callback should not have code, you can delete the object
                    }
                }
            );
        }
    }
    inline void ssl_web_client::start_web_handshaker(ssl_socket&& ssl_socket)
    {
        ssl_web_handshaker_ = msw::make_unique<ssl_web_handshaker>
        (
            ssl_web_socket(ssl_socket.eject_native_socket())
            , host_
            , resource_
            , [this](msw::ssl_web_socket ssl_web_socket)
            {
                ssl_web_handshaker_.reset();
                create_session(std::move(ssl_web_socket));
                (on_web_handshake_completed(on_web_handshake_completed_))(boost::system::error_code());
                // warning: after calling the callback should not have code, you can delete the object
            }
            , [this](boost::system::error_code ec)
            {
                ssl_web_handshaker_.reset();
                reconnect();
                (on_web_handshake_completed(on_web_handshake_completed_))(ec);
                // warning: after calling the callback should not have code, you can delete the object
            }
        );
        if (web_handshake_timeout_)
        {
            timer_ = msw::make_unique<msw::timer>
            (
                io_service()
                , *web_handshake_timeout_
                , [this]()
                {
                    timer_.reset();
                    if (ssl_web_handshaker_)
                    {
                        ssl_web_handshaker_.reset();
                        reconnect();
                        (on_web_handshake_timeout(on_web_handshake_timeout_))();
                        // warning: after calling the callback should not have code, you can delete the object
                    }
                }
            );
        }
    }
    inline void ssl_web_client::connection_established(tcp_socket&& tcp_socket)
    {
        start_ssl_handshaker(std::move(tcp_socket));
    }
}
