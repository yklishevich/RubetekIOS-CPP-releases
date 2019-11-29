#pragma once

#include <msw/noncopyable.hpp>
#include <msw/callback/event.hpp>
#include <msw/asio/tcp_client.hpp>
#include <msw/asio/priority_tcp_client.hpp>
#include <msw/lexical_cast/network/to_string.hpp>

#include <dawn/log/logger_io.hpp>
#include <dawn/log/log_io_tags.hpp>


namespace dawn {
namespace net  {

    template <typename Client>
    struct tcp_client_base
        : msw::noncopyable
    {
        typedef typename Client::query         query              ;
        typedef typename Client::on_received   on_received        ;
        typedef          msw::callback::event  on_connected       ;
        typedef          msw::callback::event  on_disconnected    ;
        typedef          msw::callback::event  on_connection_fail ;
        typedef typename Client::endpoint_type endpoint_type      ;

        template <typename QueryEndpoint>
                             tcp_client_base     (
                                                       boost::asio::io_service&
                                                     , QueryEndpoint
                                                     , on_received
                                                     , on_connected       = nullptr
                                                     , on_disconnected    = nullptr
                                                     , on_connection_fail = nullptr
                                                 )                                   ;

        void                 send                (msw::range<msw::byte const>)       ;
        bool                 connected           ()                            const ;
        msw::uint            queue_packets_count ()                            const ;
        msw::size<msw::byte> queue_packets_size  ()                            const ;
        endpoint_type        local_endpoint      ()                            const ;
        endpoint_type        remote_endpoint     ()                            const ;
        void                 reset               ()                                  ;

    private:
        dawn::logger                      const logger_     ;
        dawn::logger_io<log_io_tags::tcp> const logger_io_  ;
    protected:
        Client                                  tcp_client_ ;
    };

    typedef tcp_client_base<msw::tcp_client> tcp_client;

    struct priority_tcp_client
        : tcp_client_base<msw::priority_tcp_client>
    {
        template <typename QueryEndpoint>
             priority_tcp_client (
                                       boost::asio::io_service&
                                     , QueryEndpoint
                                     , on_received
                                     , on_connected       = nullptr
                                     , on_disconnected    = nullptr
                                     , on_connection_fail = nullptr
                                 )                             ;

        void priority_send       (msw::range<msw::byte const>) ;
    };

}}


namespace dawn {
namespace net  {

    template <typename Client>
    template <typename QueryEndpoint>
    tcp_client_base<Client>::tcp_client_base
    (
          boost::asio::io_service& io_service
        , QueryEndpoint            query_endpoint
        , on_received              on_received
        , on_connected             on_connected
        , on_disconnected          on_disconnected
        , on_connection_fail       on_connection_fail
    )
        : logger_    ( "tcp client", msw::network_point_to_string(query_endpoint) )
        , logger_io_ ( logger_                                                    )
        , tcp_client_
        (
            io_service
            , query_endpoint
            , [this](msw::tcp_session::endpoint_type endpoint)
            {
                logger_.debug("connecting to: ", msw::network_endpoint_to_string(endpoint));
            }
            , [this, on_connection_fail]()
            {
                logger_.debug("waiting time for connection has expired");
                if (on_connection_fail) on_connection_fail();
            }
            , [this, on_connected, on_connection_fail](boost::system::error_code ec)
            {
                if (ec)
                {
                    logger_.debug("connect fail: ", ec.message());
                    if (on_connection_fail) on_connection_fail();
                }
                else
                {
                    logger_.debug("successfully connected");
                    if (on_connected) on_connected();
                }
            }
            , [this, on_received](msw::range<msw::byte> packet)
            {
                logger_io_.received(packet);
                on_received(packet);
            }
            , [this](msw::range<msw::byte> packet)
            {
                logger_io_.sent(packet);
            }
            , [this, on_disconnected](boost::system::error_code ec)
            {
                logger_.debug("disconnected: ", ec.message());
                if (on_disconnected) on_disconnected();
            }
        )
    {}
    template <typename Client>
    void tcp_client_base<Client>::send(msw::range<msw::byte const> packet)
    {
        logger_io_.send(packet);
        tcp_client_.send(packet);
    }
    template <typename Client>
    bool tcp_client_base<Client>::connected() const
    {
        return tcp_client_.connected();
    }
    template <typename Client>
    msw::uint tcp_client_base<Client>::queue_packets_count() const
    {
        return tcp_client_.queue_packets_count();
    }
    template <typename Client>
    msw::size<msw::byte> tcp_client_base<Client>::queue_packets_size() const
    {
        return tcp_client_.queue_packets_size();
    }
    template <typename Client>
    typename tcp_client_base<Client>::endpoint_type tcp_client_base<Client>::local_endpoint() const
    {
        return tcp_client_.local_endpoint();
    }
    template <typename Client>
    typename tcp_client_base<Client>::endpoint_type tcp_client_base<Client>::remote_endpoint() const
    {
        return tcp_client_.remote_endpoint();
    }
    template <typename Client>
    void tcp_client_base<Client>::reset()
    {
        tcp_client_.reset();
    }

    template <typename QueryEndpoint>
    inline priority_tcp_client::priority_tcp_client
    (
          boost::asio::io_service& io_service
        , QueryEndpoint            query_endpoint
        , on_received              on_received
        , on_connected             on_connected
        , on_disconnected          on_disconnected
        , on_connection_fail       on_connection_fail
    )
        : tcp_client_base<msw::priority_tcp_client>
        (
              io_service
            , query_endpoint
            , on_received
            , on_connected
            , on_disconnected
            , on_connection_fail
        )
    {}
    inline void priority_tcp_client::priority_send(msw::range<msw::byte const> packet)
    {
        tcp_client_.priority_send(packet);
    }

}}
