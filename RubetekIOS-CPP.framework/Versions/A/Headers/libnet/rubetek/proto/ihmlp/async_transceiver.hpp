#pragma once

#include <queue>

#include <msw/buffer.hpp>

#include <rubetek/proto/ihmlp/transceiver.hpp>


namespace rubetek {
namespace ihmlp   {

    struct async_transceiver
        : private transceiver
    {
        async_transceiver(payload_ready pay_ready, all_links_disconnected all_links_disconnected)
            : transceiver
            (
                [this, pay_ready](msw::range<msw::byte const> packet)
                {
                    if (!packets_.empty())
                    {
                        send_packet(packets_.front().all());
                        packets_.pop();
                    }
                    if (packet.has_items()) pay_ready(packet);
                }
                , [this, all_links_disconnected]()
                {
                    while(!packets_.empty()) packets_.pop();
                    all_links_disconnected();
                }
            )
        {}

        using transceiver::set_primary_link;
        using transceiver::reset_primary_link;
        using transceiver::set_secondary_link;
        using transceiver::reset_secondary_link;

        using transceiver::has_links;
        using transceiver::has_primary_link;

        using transceiver::process_ihmlp_packet;

        void send(msw::range<msw::byte const> packet)
        {
            if (can_send()) send_packet(packet);
            else packets_.push(msw::buffer<msw::byte>(packet));
        }

    private:
        std::queue<msw::buffer<msw::byte>> packets_;
    };

}}
