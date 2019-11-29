#pragma once

#include <memory>
#include <functional>

#include <boost/variant.hpp>
#include <boost/optional.hpp>

#include <msw/std/memory.hpp>
#include <msw/asio/timer.hpp>
#include <msw/noncopyable.hpp>
#include <msw/asio/resolver.hpp>
#include <msw/lambda_visitor.hpp>
#include <msw/asio/tcp_session.hpp>
#include <msw/asio/tcp_connector.hpp>
#include <msw/asio/socket_options.hpp>
#include <msw/throw_runtime_error.hpp>


namespace msw {
namespace zzz {

    template <typename Session>
    struct tcp_client_base
        : noncopyable
    {
        typedef          Session                                         session_type          ;
        typedef typename session_type::socket_type                       socket_type           ;
        typedef          tcp_resolver::query                             query                 ;
        typedef          tcp_session::endpoint_type                      endpoint_type         ;
        typedef          std::function<void(tcp_session::endpoint_type)> on_connecting         ;
        typedef          std::function<void()>                           on_connection_timeout ;
        typedef          on_error                                        on_connected          ;
        typedef          tcp_session::on_received                        on_received           ;
        typedef          tcp_session::on_sent                            on_sent               ;
        typedef          on_error                                        on_disconnected       ;
        typedef          msw::chrono::steady_clock::duration             duration              ;

        static  duration                  default_reconnect_delay    ()                                                                           ;
        static  boost::optional<duration> default_connection_timeout ()                                                                           ;
        static  boost::optional<ip_dscp>  default_option_dscp        ()                                                                           ;

        template <typename QueryEndpoint>
                                          tcp_client_base            (
                                                                           boost::asio::io_service&
                                                                         , QueryEndpoint
                                                                         , on_connecting
                                                                         , on_connection_timeout
                                                                         , on_connected
                                                                         , on_received
                                                                         , on_sent
                                                                         , on_disconnected
                                                                         , duration                  reconnect_delay
                                                                         , boost::optional<duration> connection_timeout
                                                                         , boost::optional<ip_dscp>
                                                                     )                                                                            ;

                bool                      connected                  ()                                                                     const ;

                void                      send                       (range<byte const>)                                                          ;

                uint                      queue_packets_count        ()                                                                     const ;
                size<byte>                queue_packets_size         ()                                                                     const ;

                endpoint_type             local_endpoint             ()                                                                     const ;
                endpoint_type             remote_endpoint            ()                                                                     const ;

                duration                  reconnect_delay            ()                                                                     const ;
                boost::optional<duration> connection_timeout         ()                                                                     const ;

                void                      reset                      ()                                                                           ;

    protected:
                boost::asio::io_service&  io_service                 ()                                                                     const ;
        template <typename OtherSession>
                void                      create_session             (std::unique_ptr<OtherSession>&, typename OtherSession::socket_type&&)       ;
                void                      create_session             (socket_type&&)                                                              ;

    private:
                void                      resolve                    ()                                                                           ;
                void                      connect                    (endpoint_type, tcp_resolver::iterator = tcp_resolver::iterator())           ;
                void                      connect                    (tcp_resolver::iterator)                                                     ;
                void                      stop_connection            ()                                                                           ;
    protected:
                void                      reconnect                  ()                                                                           ;
    private:
        virtual void                      connection_established     (tcp_socket&&)                                                           = 0 ;

        typedef boost::variant<query, endpoint_type> query_endpoint;

        boost::asio::io_service&                  io_service_            ;
        query_endpoint                      const query_endpoint_        ;
        duration                            const reconnect_delay_       ;
        boost::optional<duration>           const connection_timeout_    ;
        boost::optional<ip_dscp>            const option_dscp_           ;
        on_connecting                       const on_connecting_         ;
        on_connection_timeout               const on_connection_timeout_ ;
        on_connected                        const on_connected_          ;
        on_received                         const on_received_           ;
        on_sent                             const on_sent_               ;
        on_disconnected                     const on_disconnected_       ;
        std::unique_ptr<msw::tcp_resolver>        tcp_resolver_          ;
        std::unique_ptr<msw::tcp_connector>       tcp_connector_         ;
        std::unique_ptr<msw::timer>               timer_                 ;
        std::unique_ptr<session_type>             session_               ;
    };

}}


namespace msw {
namespace zzz {

    template <typename Session>
    typename tcp_client_base<Session>::duration tcp_client_base<Session>::default_reconnect_delay()
    {
        return msw::chrono::seconds(1);
    }
    template <typename Session>
    boost::optional<typename tcp_client_base<Session>::duration> tcp_client_base<Session>::default_connection_timeout()
    {
        return duration(msw::chrono::seconds(20));
    }
    template <typename Session>
    boost::optional<ip_dscp> tcp_client_base<Session>::default_option_dscp()
    {
        return boost::none;
    }

    template <typename Session>
    template <typename QueryEndpoint>
    tcp_client_base<Session>::tcp_client_base
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
        : io_service_            ( io_service            )
        , query_endpoint_        ( query_endpoint        )
        , reconnect_delay_       ( reconnect_delay       )
        , connection_timeout_    ( connection_timeout    )
        , option_dscp_           ( option_dscp           )
        , on_connecting_         ( on_connecting         )
        , on_connection_timeout_ ( on_connection_timeout )
        , on_connected_          ( on_connected          )
        , on_received_           ( on_received           )
        , on_sent_               ( on_sent               )
        , on_disconnected_       ( on_disconnected       )
    {
        resolve();
    }

    template <typename Session>
    bool tcp_client_base<Session>::connected() const
    {
        return static_cast<bool>(session_);
    }

    template <typename Session>
    void tcp_client_base<Session>::send(range<byte const> packet)
    {
        if (!session_) msw::throw_runtime_error("tcp client: impossible to send a packet, because there is no connection");
        session_->send(packet);
    }

    template <typename Session>
    uint tcp_client_base<Session>::queue_packets_count() const
    {
        return session_ ? session_->queue_packets_count() : 0;
    }
    template <typename Session>
    size<byte> tcp_client_base<Session>::queue_packets_size() const
    {
        return session_ ? session_->queue_packets_size() : 0;
    }

    template <typename Session>
    typename tcp_client_base<Session>::endpoint_type tcp_client_base<Session>::local_endpoint() const
    {
        if (!session_) msw::throw_runtime_error("tcp client: local endpoint not available, because there is no connection");
        return session_->local_endpoint();
    }
    template <typename Session>
    typename tcp_client_base<Session>::endpoint_type tcp_client_base<Session>::remote_endpoint() const
    {
        if (!session_) msw::throw_runtime_error("tcp client: remote endpoint not available, because there is no connection");
        return session_->remote_endpoint();
    }

    template <typename Session>
    typename tcp_client_base<Session>::duration tcp_client_base<Session>::reconnect_delay() const
    {
        return reconnect_delay_;
    }
    template <typename Session>
    boost::optional<typename tcp_client_base<Session>::duration> tcp_client_base<Session>::connection_timeout() const
    {
        return connection_timeout_;
    }

    template <typename Session>
    void tcp_client_base<Session>::reset()
    {
        tcp_resolver_  .reset();
        tcp_connector_ .reset();
        timer_         .reset();
        session_       .reset();
        resolve();
    }

    template <typename Session>
    boost::asio::io_service& tcp_client_base<Session>::io_service() const
    {
        return io_service_;
    }
    template <typename Session>
    template <typename OtherSession>
    void tcp_client_base<Session>::create_session(std::unique_ptr<OtherSession>& session, typename OtherSession::socket_type&& socket)
    {
        session = msw::make_unique<OtherSession>
        (
            std::move(socket)
            , on_received_
            , on_sent_
            , [this, &session](boost::system::error_code ec)
            {
                session.reset();
                reconnect();
                if (on_disconnected_) (on_disconnected(on_disconnected_))(ec);
                // warning: after calling the callback should not have code, you can delete the object
            }
        );
    }
    template <typename Session>
    void tcp_client_base<Session>::create_session(socket_type&& socket)
    {
        create_session<session_type>(session_, std::move(socket));
    }

    template <typename Session>
    void tcp_client_base<Session>::resolve()
    {
        apply_lambda_void_visitor
        (
            query_endpoint_
            , [this](query q)
            {
                tcp_resolver_ = msw::make_unique<msw::tcp_resolver>
                (
                    io_service_
                    , q
                    , [this](tcp_resolver::iterator i)
                    {
                        connect(i);
                    }
                    , [this](boost::system::error_code ec)
                    {
                        reconnect();
                        (on_connected(on_connected_))(ec);
                        // warning: after calling the callback should not have code, you can delete the object
                    }
                );
            }
            , [this](endpoint_type endpoint)
            {
                connect(endpoint);
            }
        );
    }
    template <typename Session>
    void tcp_client_base<Session>::connect(endpoint_type const endpoint, tcp_resolver::iterator i)
    {
        if (on_connecting_) (on_connecting(on_connecting_))(endpoint);
        tcp_connector_ = msw::make_unique<msw::tcp_connector>
        (
            io_service_
            , endpoint
            , [this](tcp_socket socket)
            {
                stop_connection();
                if (option_dscp_) set_socket_option(socket).dscp(*option_dscp_);
                connection_established(std::move(socket));
                (on_connected(on_connected_))(boost::system::error_code());
                // warning: after calling the callback should not have code, you can delete the object
            }
            , [this, i](boost::system::error_code ec)
            {
                if (i == tcp_resolver::iterator())
                {
                    stop_connection();
                    reconnect();
                    (on_connected(on_connected_))(ec);
                    // warning: after calling the callback should not have code, you can delete the object
                }
                else connect(i);
            }
        );
        if (connection_timeout_)
        {
            timer_ = msw::make_unique<msw::timer>
            (
                io_service_
                , *connection_timeout_
                , [this]()
                {
                    timer_.reset();
                    if (tcp_connector_)
                    {
                        stop_connection();
                        reconnect();
                        (on_connection_timeout(on_connection_timeout_))();
                        // warning: after calling the callback should not have code, you can delete the object
                    }
                }
            );
        }
    }
    template <typename Session>
    void tcp_client_base<Session>::connect(tcp_resolver::iterator i)
    {
        MSW_ASSERT(i != tcp_resolver::iterator());
        endpoint_type const endpoint = i++->endpoint();
        connect(endpoint, i);
    }
    template <typename Session>
    void tcp_client_base<Session>::stop_connection()
    {
        tcp_resolver_.reset();
        tcp_connector_.reset();
    }
    template <typename Session>
    void tcp_client_base<Session>::reconnect()
    {
        timer_ = msw::make_unique<msw::timer>
        (
            io_service_
            , reconnect_delay_
            , [this]()
            {
                timer_.reset();
                resolve();
            }
        );
    }

}}
