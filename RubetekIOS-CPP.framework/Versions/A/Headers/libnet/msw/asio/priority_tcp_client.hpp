#pragma once

#include <queue>

#include <msw/buffer.hpp>
#include <msw/asio/tcp_client.hpp>


namespace msw
{
    struct priority_tcp_client
        : tcp_client
    {
               priority_tcp_client (
                                         boost::asio::io_service&
                                       , query
                                       , on_connecting
                                       , on_connection_timeout
                                       , on_connected
                                       , on_received
                                       , on_sent                                      = nullptr
                                       , on_disconnected                              = nullptr
                                       , duration                  reconnect_delay    = default_reconnect_delay    ()
                                       , boost::optional<duration> connection_timeout = default_connection_timeout ()
                                   )                         ;

    void       send                (range<byte const>)       ;
    void       priority_send       (range<byte const>)       ;

    uint       queue_packets_count ()                  const ;
    size<byte> queue_packets_size  ()                  const ;

    void       reset               ()                        ;

    private:
        std::queue<buffer<byte>> not_priority_packets_      ;
        size<byte>               not_priority_packets_size_ ;
    };
}


namespace msw
{
    inline priority_tcp_client::priority_tcp_client
    (
          boost::asio::io_service&  io_service
        , query                     query
        , on_connecting             on_connecting
        , on_connection_timeout     on_connection_timeout
        , on_connected              on_connected
        , on_received               on_received
        , on_sent                   on_sent
        , on_disconnected           on_disconnected
        , duration                  reconnect_delay
        , boost::optional<duration> connection_timeout
    )
        : tcp_client
        (
              io_service
            , query
            , on_connecting
            , on_connection_timeout
            , on_connected
            , on_received
            , [this, on_sent](msw::range<msw::byte> packet)
            {
                if (!tcp_client::queue_packets_count())
                {
                    if (!not_priority_packets_.empty())
                    {
                        priority_send(not_priority_packets_.front().all());
                        not_priority_packets_size_ -= not_priority_packets_.front().size();
                        not_priority_packets_.pop();
                    }
                }
                if (on_sent) on_sent(packet);
            }
            , [this, on_disconnected](boost::system::error_code const& ec)
            {
                decltype(not_priority_packets_)(std::move(not_priority_packets_));
                not_priority_packets_size_.clear();
                if (on_disconnected) on_disconnected(ec);
            }
            , reconnect_delay
            , connection_timeout
        )
    {}

    inline void priority_tcp_client::send(range<byte const> packet)
    {
        if (tcp_client::queue_packets_count())
        {
            not_priority_packets_.push(buffer<byte>(packet));
            not_priority_packets_size_ += packet.size();
        }
        else priority_send(packet);
    }
    inline void priority_tcp_client::priority_send(range<byte const> packet)
    {
        tcp_client::send(packet);
    }

    inline uint priority_tcp_client::queue_packets_count() const
    {
        return tcp_client::queue_packets_count() + not_priority_packets_.size();
    }
    inline size<byte> priority_tcp_client::queue_packets_size() const
    {
        return tcp_client::queue_packets_size() + not_priority_packets_size_;
    }

    inline void priority_tcp_client::reset()
    {
        tcp_client::reset();
    }
}
