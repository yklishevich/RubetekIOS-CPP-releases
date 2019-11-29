#pragma once

#include <msw/zzz/asio/tcp_server/session.hpp>
#include <msw/asio/tcp_server/create_session.hpp>


namespace msw
{
    template <typename Session>
    struct default_on_accept
    {
        typedef Session                           session_type   ;
        typedef msw::create_session<session_type> create_session ;

        void operator () (create_session) const;
    };
}


namespace msw
{
    template <typename Session>
    void default_on_accept<Session>::operator () (create_session create) const
    {
        create();
    }
}
