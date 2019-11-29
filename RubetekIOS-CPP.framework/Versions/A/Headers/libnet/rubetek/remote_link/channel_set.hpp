#pragma once

#include <memory>
#include <functional>
#include <unordered_map>

#include <msw/range.hpp>
#include <msw/noncopyable.hpp>
#include <msw/lexical_cast/bytes_as_string.hpp>

#include <rubetek/log.hpp>
#include <rubetek/remote_link/channel.hpp>
#include <rubetek/essence/unique_id/channel.hpp>


namespace rubetek
{
    template <typename RemoteId, typename Context = void*>
    struct remote_channel_set
    {
        typedef RemoteId                                                                 remote_id      ;
        typedef Context                                                                  context_type   ;

        typedef std::function<void(remote_id)>                                           channel_close  ;
        typedef std::function<void(unique_channel_id_type)>                              channel_closed ;
        typedef std::function<void(unique_channel_id_type, msw::range<msw::byte const>)> send_payload   ;

             remote_channel_set  (channel_close, send_payload)                               ;
             ~remote_channel_set ()                                                          ;

        void clear               ()                                                          ;

        void close               (remote_id, channel_closed)                                 ;

        bool has_channel         (remote_id)                                           const ;

        void on_close            (unique_channel_id_type)                                    ;
        void on_payload          (unique_channel_id_type, msw::range<msw::byte const>)       ;

    protected:
        typedef std::unordered_map<remote_id, unique_channel_id_type>          channel_by_remote_id_list ;
        typedef std::unique_ptr<remote_channel<remote_id, context_type>>       remote_channel_ptr        ;
        typedef std::unordered_map<unique_channel_id_type, remote_channel_ptr> channels                  ;

        logger                    const logger_                    ;
        channel_close                   channel_close_             ;
        send_payload                    send_payload_              ;
        channel_by_remote_id_list       channel_by_remote_id_list_ ;
        channels                        channels_                  ;
    };
}


namespace rubetek
{
    template <typename RemoteId, typename Context>
    remote_channel_set<RemoteId, Context>::remote_channel_set(channel_close channel_close, send_payload send_payload)
        : logger_        ( "remote channels" )
        , channel_close_ ( channel_close     )
        , send_payload_  ( send_payload      )
    {}
    template <typename RemoteId, typename Context>
    remote_channel_set<RemoteId, Context>::~remote_channel_set()
    {}

    template <typename RemoteId, typename Context>
    void remote_channel_set<RemoteId, Context>::clear()
    {
        logger_.debug("clear");
        for (auto const& ch : channel_by_remote_id_list_)
            channel_close_(ch.first);
        channels_.clear();
        channel_by_remote_id_list_.clear();
    }

    template <typename RemoteId, typename Context>
    void remote_channel_set<RemoteId, Context>::close(remote_id id, channel_closed closed)
    {
        logger_.debug("close, remote_id: ", id);
        auto it = channel_by_remote_id_list_.find(id);
        if (it == channel_by_remote_id_list_.end())
        {
            logger_.warning("close: channel not found");
        }
        else
        {
            logger_.debug("close channel to: ", id);
            closed(it->second);
            channels_.erase(it->second);
            channel_by_remote_id_list_.erase(it);
        }
    }

    template <typename RemoteId, typename Context>
    bool remote_channel_set<RemoteId, Context>::has_channel(remote_id id) const
    {
        return channel_by_remote_id_list_.count(id) > 0;
    }

    template <typename RemoteId, typename Context>
    void remote_channel_set<RemoteId, Context>::on_close(unique_channel_id_type channel_id)
    {
        logger_.debug("on close, channel_id: ", channel_id);
        auto it = channels_.find(channel_id);
        if (it == channels_.end()) logger_.error("can't close channel. not found: ", channel_id);
        else
        {
            channel_close_(it->second->remote_id());
            channel_by_remote_id_list_.erase(it->second->remote_id());
            channels_.erase(it);
        }
    }
    template <typename RemoteId, typename Context>
    void remote_channel_set<RemoteId, Context>::on_payload(unique_channel_id_type channel_id, msw::range<msw::byte const> packet)
    {
        auto it = channels_.find(channel_id);
        if (it == channels_.end()) logger_.error("can't to deliver payload. not found channel: ", channel_id);
        else it->second->packet_from_remote(packet);
    }
}
