#pragma once

#include <msw/callback/byte_range.hpp>

#include <rubetek/log.hpp>
#include <rubetek/remote_link/interface.hpp>


namespace rubetek
{
    template <typename RemoteId, typename Context = void*>
    struct remote_channel
        : remote_link_interface<Context>
    {
        typedef          RemoteId                                    remote_id_type ;
        typedef          Context                                     context_type   ;
        typedef          msw::callback::byte_const_range             fn_send        ;
        typedef typename remote_link_interface<Context>::on_received on_received    ;

        remote_channel(remote_id_type remote_id, fn_send fsend, context_type context = context_type())
            : logger_    ( "remote channel", remote_id )
            , remote_id_ ( remote_id                   )
            , send_      ( fsend                       )
            , context_   ( context                     )
        {}

        void packet_from_remote(msw::range<msw::byte const> packet)
        {
            MSW_ASSERT(on_received_);
            on_received_(packet);
        }

        remote_id_type const& remote_id() const
        {
            return remote_id_;
        }

    private:

        void set_on_received(on_received on_received)
        {
            on_received_ = on_received;
        }
        void send(msw::range<msw::byte const> packet)
        {
            send_(packet);
        }
        context_type& get_context()
        {
            return context_;
        }

        logger         const logger_      ;
        remote_id_type const remote_id_   ;
        fn_send              send_        ;
        on_received          on_received_ ;
        context_type         context_     ;
    };
}
