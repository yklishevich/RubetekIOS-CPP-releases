#pragma once

#include <string>
#include <functional>

#include <msw/std/memory.hpp>

#include <rubetek/remote_link/channel.hpp>
#include <rubetek/remote_link/channel_set.hpp>


namespace rubetek {
namespace client  {

    struct remote_channels
        : remote_channel_set<unique_client_id::type>
    {
        typedef std::function<void(unique_channel_id_type, std::string const& ip, unique_client_id::const_reference, remote_link_interface<>*)> channel_open;

        remote_channels
        (
              channel_open  channel_open
            , channel_close channel_close
            , send_payload  send_payload
        )
            : remote_channel_set<unique_client_id::type> ( channel_close, send_payload )
            , channel_open_                              ( channel_open                )
        {}

        void on_open(unique_channel_id_type channel_id, unique_client_id::const_reference rem_id, std::string const& ip)
        {
            if (channel_by_remote_id_list_.find(rem_id) != channel_by_remote_id_list_.end())
            {
                logger_.error("channel to ", rem_id, " already exists");
            }
            else
            {
                logger_.info("insert new channel to ", rem_id, ", channel id: ", channel_id);
                channel_by_remote_id_list_.insert(std::make_pair(rem_id, channel_id));
                auto res = channels_.insert
                (
                    std::make_pair
                    (
                        channel_id
                        , msw::make_unique<remote_channel<remote_id>>
                        (
                            rem_id
                            , [this, channel_id](msw::range<msw::byte const> payload)
                            {
                                send_payload_(channel_id, payload);
                            }
                        )
                    )
                );
                if (!res.second)        throw std::runtime_error("adding channel fail");
                if (!res.first->second) throw std::runtime_error("remote channel fail ptr");
                channel_open_(channel_id, ip, rem_id, res.first->second.get());
            }
        }

    private:
        channel_open channel_open_;
    };

}}
