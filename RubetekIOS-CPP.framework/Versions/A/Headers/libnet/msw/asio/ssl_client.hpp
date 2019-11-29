#pragma once

#include <memory>

#include <boost/optional.hpp>

#include <msw/std/memory.hpp>
#include <msw/asio/timer.hpp>
#include <msw/asio/ssl_context.hpp>
#include <msw/asio/ssl_session.hpp>
#include <msw/zzz/asio/on_error.hpp>
#include <msw/asio/ssl_handshaker.hpp>
#include <msw/zzz/asio/tcp_client_base.hpp>


namespace msw
{
    struct ssl_client
        : zzz::tcp_client_base<ssl_session>
    {
        typedef ssl_handshaker::on_verify           on_verify              ;
        typedef std::function<void()>               on_handshake_timeout   ;
        typedef on_error                            on_handshake_completed ;
        typedef msw::chrono::steady_clock::duration duration               ;

        static  boost::optional<duration> default_handshake_timeout ()                                  ;

                                          ssl_client                (
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
                                                                        , boost::optional<ip_dscp>  option_dscp        = default_option_dscp        ()
                                                                    )                                   ;
                boost::optional<duration> handshake_timeout         ()                   const          ;
                void                      load_verify_file          (std::string const&)                ;
                void                      reset                     ()                                  ;
    protected:
                void                      start_handshaker          (tcp_socket&&)                      ;
    private:
        virtual void                      connection_established    (tcp_socket&&)             override ;

        boost::optional<duration>       const handshake_timeout_      ;
        on_verify                       const on_verify_              ;
        on_handshake_timeout            const on_handshake_timeout_   ;
        on_handshake_completed          const on_handshake_completed_ ;
        ssl_context                           ssl_context_            ;
        std::unique_ptr<ssl_handshaker>       ssl_handshaker_         ;
        std::unique_ptr<msw::timer>           timer_                  ;
    };
}


namespace msw
{
    inline boost::optional<ssl_client::duration> ssl_client::default_handshake_timeout()
    {
        return duration(msw::chrono::seconds(20));
    }

    inline ssl_client::ssl_client
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
        , boost::optional<ip_dscp>  option_dscp
    )
        : zzz::tcp_client_base<ssl_session>
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
        , handshake_timeout_      ( handshake_timeout                             )
        , on_verify_              ( on_verify                                     )
        , on_handshake_timeout_   ( on_handshake_timeout                          )
        , on_handshake_completed_ ( on_handshake_completed                        )
        , ssl_context_            ( io_service, boost::asio::ssl::context::sslv23 )
    {
        //ssl_context_.set_options
        //(
        //  boost::asio::ssl::context::default_workarounds
        //  | boost::asio::ssl::context::no_sslv2
        //  | boost::asio::ssl::context::single_dh_use
        //);
        //ssl_context_.set_password_callback([](){return "";});
        //ssl_context_.use_certificate_chain_file("server.pem");
        //ssl_context_.use_private_key_file("server.pem", boost::asio::ssl::context::pem);
        //ssl_context_.use_tmp_dh_file("dh512.pem");
    }
    inline boost::optional<ssl_client::duration> ssl_client::handshake_timeout() const
    {
        return handshake_timeout_;
    }
    inline void ssl_client::load_verify_file(std::string const& filename)
    {
        ssl_context_.load_verify_file(filename);
    }
    inline void ssl_client::reset()
    {
        timer_.reset();
        ssl_handshaker_.reset();
        zzz::tcp_client_base<ssl_session>::reset();
    }
    inline void ssl_client::start_handshaker(tcp_socket&& tcp_socket)
    {
        ssl_handshaker_ = msw::make_unique<ssl_handshaker>
        (
            std::move(tcp_socket)
            , ssl_context_
            , [this](msw::ssl_socket ssl_socket)
            {
                ssl_handshaker_.reset();
                create_session(std::move(ssl_socket));
                (on_handshake_completed(on_handshake_completed_))(boost::system::error_code());
                // warning: after calling the callback should not have code, you can delete the object
            }
            , on_verify_
            , [this](boost::system::error_code ec)
            {
                ssl_handshaker_.reset();
                reconnect();
                (on_handshake_completed(on_handshake_completed_))(ec);
                // warning: after calling the callback should not have code, you can delete the object
            }
        );
        if (handshake_timeout_)
        {
            timer_ = msw::make_unique<msw::timer>
            (
                io_service()
                , *handshake_timeout_
                , [this]()
                {
                    timer_.reset();
                    if (ssl_handshaker_)
                    {
                        ssl_handshaker_.reset();
                        reconnect();
                        (on_handshake_timeout(on_handshake_timeout_))();
                        // warning: after calling the callback should not have code, you can delete the object
                    }
                }
            );
        }
    }
    inline void ssl_client::connection_established(tcp_socket&& tcp_socket)
    {
        start_handshaker(std::move(tcp_socket));
    }
}