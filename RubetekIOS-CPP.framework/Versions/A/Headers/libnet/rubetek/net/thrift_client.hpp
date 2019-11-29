#pragma once

#include <functional>

#include <msw/noncopyable.hpp>
#include <msw/lexical_cast/network/query/to_string.hpp>

#include <rubetek/config.hpp>
#include <rubetek/log.hpp>
#ifdef SMART_HOUSE_THRIFT_CLIENT_DISABLE_SSL
#   include <rubetek/net/tcp_client.hpp>
#else
#   include <rubetek/net/ssl_hybrid_client.hpp>
#endif
#include <rubetek/proto/thrift/transceiver.hpp>


namespace rubetek
{
    template <typename Pkt>
    struct thrift_client
        : msw::noncopyable
    {
                typedef
#ifdef SMART_HOUSE_THRIFT_CLIENT_DISABLE_SSL
            tcp_client
#else
            ssl_hybrid_client
#endif
            client_type;

        typedef std::function<void(Pkt const&)> on_received        ;
        typedef client_type::on_connected       on_connected       ;
        typedef client_type::on_disconnected    on_disconnected    ;
        typedef client_type::on_connection_fail on_connection_fail ;

             thrift_client (
                                 boost::asio::io_service&
                               , msw::tcp_resolver::query
                               , on_received
                               , on_connected
                               , on_disconnected
                               , on_connection_fail
                               , bool                     use_ssl = true
                           )                  ;

        void send          (Pkt const&)       ;
        bool use_ssl       ()           const ;
        bool connected     ()           const ;
        void reset         ()                 ;

    private:

        logger                   const logger_             ;
        bool                     const use_ssl_            ;
        boost::asio::io_service&       io_service_         ;
        client_type                    client_             ;
        thrift_transceiver<Pkt>        thrift_transceiver_ ;
    };
}


namespace rubetek
{
    template <typename Pkt>
    thrift_client<Pkt>::thrift_client
    (
          boost::asio::io_service& io_service
        , msw::tcp_resolver::query query
        , on_received              on_received
        , on_connected             on_connected
        , on_disconnected          on_disconnected
        , on_connection_fail       on_connection_fail
        , bool                     use_ssl
    )
        : logger_     ( "thrfit client", msw::network_query_to_string(query) )
        , use_ssl_    ( use_ssl                                              )
        , io_service_ ( io_service                                           )
        , client_
        (
            io_service
            , query
            , [this](msw::range<msw::byte> packet)
            {
                //logger_.debug("process_tcp_payload");
                try
                {
                    thrift_transceiver_.process_tcp_payload(packet);
                }
                catch (std::exception const& e)
                {
                    logger_.error("process tcp payload: ", e.what());
                    throw;
                }
                //logger_.debug("process_tcp_payload exit");
            }
            , on_connected
            , [this, on_disconnected]()
            {
                thrift_transceiver_.reset();
                on_disconnected();
            }
            , on_connection_fail
#ifndef SMART_HOUSE_THRIFT_CLIENT_DISABLE_SSL
            , use_ssl
#endif
        )
        , thrift_transceiver_
        (
            on_received
            , [this](msw::range<msw::byte const> packet)
            {
                //logger_.debug("tcp send");
                client_.send(packet);
                //logger_.debug("tcp send exit");
            }
            , [this, on_disconnected]() // on_thrift_error
            {
                client_.reset();
                on_disconnected();
            }
        )
    {}
    template <typename Pkt>
    void thrift_client<Pkt>::send(Pkt const& packet)
    {
        //logger_.debug("send");
        thrift_transceiver_.send(packet);
        //logger_.debug("send exit");
    }
    template <typename Pkt>
    bool thrift_client<Pkt>::use_ssl() const
    {
        return use_ssl_;
    }
    template <typename Pkt>
    bool thrift_client<Pkt>::connected() const
    {
        return client_.connected();
    }
    template <typename Pkt>
    void thrift_client<Pkt>::reset()
    {
        return client_.reset();
    }
}
