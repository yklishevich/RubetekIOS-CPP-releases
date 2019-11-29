#pragma once

#include <memory>
#include <string>

#include <boost/optional.hpp>

#include <msw/asio/timer.hpp>
#include <msw/asio/web_session.hpp>
#include <msw/zzz/asio/on_error.hpp>
#include <msw/asio/web_handshaker.hpp>
#include <msw/zzz/asio/tcp_client_base.hpp>


namespace msw
{
    struct web_client
        : zzz::tcp_client_base<web_session>
    {
        typedef std::function<void()>               on_handshake_timeout   ;
        typedef on_error                            on_handshake_completed ;
        typedef msw::chrono::steady_clock::duration duration               ;

        static  boost::optional<duration> default_handshake_timeout ()                      ;

                                          web_client                (
                                                                          boost::asio::io_service&
                                                                        , query
                                                                        , std::string const& host
                                                                        , std::string const& resource
                                                                        , on_connecting
                                                                        , on_connection_timeout
                                                                        , on_connected
                                                                        , on_handshake_timeout
                                                                        , on_handshake_completed
                                                                        , on_received
                                                                        , on_sent                                      = nullptr
                                                                        , on_disconnected                              = nullptr
                                                                        , duration                  reconnect_delay    = default_reconnect_delay    ()
                                                                        , boost::optional<duration> connection_timeout = default_connection_timeout ()
                                                                        , boost::optional<duration> handshake_timeout  = default_handshake_timeout  ()
                                                                        , boost::optional<ip_dscp>  option_dscp        = default_option_dscp        ()
                                                                    )                       ;
                                          web_client                (
                                                                          boost::asio::io_service&
                                                                        , query
                                                                        , std::string const& resource
                                                                        , on_connecting
                                                                        , on_connection_timeout
                                                                        , on_connected
                                                                        , on_handshake_timeout
                                                                        , on_handshake_completed
                                                                        , on_received
                                                                        , on_sent                                      = nullptr
                                                                        , on_disconnected                              = nullptr
                                                                        , duration                  reconnect_delay    = default_reconnect_delay    ()
                                                                        , boost::optional<duration> connection_timeout = default_connection_timeout ()
                                                                        , boost::optional<duration> handshake_timeout  = default_handshake_timeout  ()
                                                                        , boost::optional<ip_dscp>  option_dscp        = default_option_dscp        ()
                                                                    )                       ;
                                          web_client                (
                                                                          boost::asio::io_service&
                                                                        , query
                                                                        , on_connecting
                                                                        , on_connection_timeout
                                                                        , on_connected
                                                                        , on_handshake_timeout
                                                                        , on_handshake_completed
                                                                        , on_received
                                                                        , on_sent                                      = nullptr
                                                                        , on_disconnected                              = nullptr
                                                                        , duration                  reconnect_delay    = default_reconnect_delay    ()
                                                                        , boost::optional<duration> connection_timeout = default_connection_timeout ()
                                                                        , boost::optional<duration> handshake_timeout  = default_handshake_timeout  ()
                                                                        , boost::optional<ip_dscp>  option_dscp        = default_option_dscp        ()
                                                                    )                       ;
                boost::optional<duration> handshake_timeout         ()             const    ;
                void                      reset                     ()                      ;
    protected:
                void                      start_handshaker          (tcp_socket&&)          ;
    private:
        virtual void                      connection_established    (tcp_socket&&) override ;

        std::string                     const host_                   ;
        std::string                     const resource_               ;
        boost::optional<duration>       const handshake_timeout_      ;
        on_handshake_timeout            const on_handshake_timeout_   ;
        on_handshake_completed          const on_handshake_completed_ ;
        std::unique_ptr<web_handshaker>       web_handshaker_         ;
        std::unique_ptr<msw::timer>           timer_                  ;
    };
}


namespace msw
{
    inline boost::optional<web_client::duration> web_client::default_handshake_timeout()
    {
        return duration(msw::chrono::seconds(20));
    }

    inline web_client::web_client
    (
          boost::asio::io_service&  io_service
        , query                     query
        , std::string const&        host
        , std::string const&        resource
        , on_connecting             on_connecting
        , on_connection_timeout     on_connection_timeout
        , on_connected              on_connected
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
        : zzz::tcp_client_base<web_session>
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
        , host_                   ( host                   )
        , resource_               ( resource               )
        , handshake_timeout_      ( handshake_timeout      )
        , on_handshake_timeout_   ( on_handshake_timeout   )
        , on_handshake_completed_ ( on_handshake_completed )
    {}
    inline web_client::web_client
    (
          boost::asio::io_service&  io_service
        , query                     query
        , std::string const&        resource
        , on_connecting             on_connecting
        , on_connection_timeout     on_connection_timeout
        , on_connected              on_connected
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
        : web_client
        (
              io_service
            , query
            , query.host_name()
            , resource
            , on_connecting
            , on_connection_timeout
            , on_connected
            , on_handshake_timeout
            , on_handshake_completed
            , on_received
            , on_sent
            , on_disconnected
            , reconnect_delay
            , connection_timeout
            , handshake_timeout
            , option_dscp
        )
    {}
    inline web_client::web_client
    (
          boost::asio::io_service&  io_service
        , query                     query
        , on_connecting             on_connecting
        , on_connection_timeout     on_connection_timeout
        , on_connected              on_connected
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
        : web_client
        (
              io_service
            , query
            , "/"
            , on_connecting
            , on_connection_timeout
            , on_connected
            , on_handshake_timeout
            , on_handshake_completed
            , on_received
            , on_sent
            , on_disconnected
            , reconnect_delay
            , connection_timeout
            , handshake_timeout
            , option_dscp
        )
    {}
    inline boost::optional<web_client::duration> web_client::handshake_timeout() const
    {
        return handshake_timeout_;
    }
    inline void web_client::reset()
    {
        timer_.reset();
        web_handshaker_.reset();
        zzz::tcp_client_base<web_session>::reset();
    }
    inline void web_client::start_handshaker(tcp_socket&& tcp_socket)
    {
        web_handshaker_.reset
        (
            new web_handshaker
            (
                web_socket(std::move(tcp_socket))
                , host_
                , resource_
                , [this](msw::web_socket web_socket)
                {
                    web_handshaker_.reset();
                    create_session(std::move(web_socket));
                    (on_handshake_completed(on_handshake_completed_))(boost::system::error_code());
                    // warning: after calling the callback should not have code, you can delete the object
                }
                , [this](boost::system::error_code ec)
                {
                    web_handshaker_.reset();
                    reconnect();
                    (on_handshake_completed(on_handshake_completed_))(ec);
                    // warning: after calling the callback should not have code, you can delete the object
                }
            )
        );
        if (handshake_timeout_)
        {
            timer_.reset
            (
                new msw::timer
                (
                    io_service()
                    , *handshake_timeout_
                    , [this]()
                    {
                        timer_.reset();
                        if (web_handshaker_)
                        {
                            web_handshaker_.reset();
                            reconnect();
                            (on_handshake_timeout(on_handshake_timeout_))();
                            // warning: after calling the callback should not have code, you can delete the object
                        }
                    }
                )
            );
        }
    }
    inline void web_client::connection_established(tcp_socket&& tcp_socket)
    {
        start_handshaker(std::move(tcp_socket));
    }
}
