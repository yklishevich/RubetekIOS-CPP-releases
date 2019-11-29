#pragma once

#include <string>

#include <msw/range.hpp>
#include <msw/asio/timer.hpp>
#include <msw/noncopyable.hpp>
#include <msw/callback/event.hpp>
#include <msw/callback/byte_range.hpp>

#include <dawn/net/tcp_client.hpp>

#include <rubetek/log.hpp>
#include <rubetek/proto/ihlp/transceiver.hpp>


namespace rubetek
{
    template <typename SizeType = msw::wbyte>
    struct ihlp_connection
    {
        typedef msw::callback::event           on_connected ;
        typedef msw::callback::byte_range      on_received  ;
        typedef msw::callback::event           on_fail      ;
        typedef boost::asio::ip::tcp::endpoint endpoint     ;

             ihlp_connection  (
                                   boost::asio::io_service&
                                 , endpoint
                                 , ihlp::timeouts
                                 , on_connected
                                 , on_received
                                 , on_fail
                              )                                   ;
        void send             (msw::range<msw::byte const>)       ;
        bool connected        ()                            const ;
    private:

        logger                           const logger_           ;
        ihlp::transceiver_base<SizeType>       ihlp_transceiver_ ;
        dawn::net::tcp_client                  tcp_client_       ;
        msw::timer                             timer_            ;
    };
}


namespace rubetek
{
    template <typename SizeType>
    ihlp_connection<SizeType>::ihlp_connection
    (
          boost::asio::io_service& io_service
        , endpoint                 rmt_endpoint
        , ihlp::timeouts           ihlp_timeouts
        , on_connected             on_connected
        , on_received              on_received
        , on_fail                  on_connection_fail
    )
        : logger_
        (
               "ihlp connection"
             , msw::network_endpoint_to_string(rmt_endpoint)
             , log::level::debug
        )
        , ihlp_transceiver_
        (
            on_received
            , [this](msw::range<msw::byte const> packet)
            {
                if (connected()) tcp_client_.send(packet);
            }
            , ihlp_timeouts
        )
        , tcp_client_
        (
            io_service
            , rmt_endpoint
            , [this](msw::range<msw::byte> packet)
            {
                ihlp_transceiver_.process_tcp_payload(packet);
            }
            , on_connected
            , on_connection_fail
            , on_connection_fail
        )
        , timer_
        (
            io_service
            , msw::chrono::milliseconds(200)
            , [this, on_connection_fail]()
            {
                if (!ihlp_transceiver_.check_timeout())
                {
                    logger_.debug("close by timeout");
                    on_connection_fail();
                }
            }
        )
    {}
    template <typename SizeType>
    void ihlp_connection<SizeType>::send(msw::range<msw::byte const> packet)
    {
        MSW_ASSERT(connected());
        ihlp_transceiver_.send(packet);
    }
    template <typename SizeType>
    bool ihlp_connection<SizeType>::connected() const
    {
        return tcp_client_.connected();
    }
}
