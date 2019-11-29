#pragma once

#include <functional>

#include <msw/range.hpp>
#include <msw/noncopyable.hpp>
#include <msw/asio/tcp_session.hpp>


namespace msw
{
    struct tcp_session_interface
        : noncopyable
    {
        typedef tcp_session::on_received on_received ;
        typedef tcp_session::on_sent     on_sent     ;

        virtual void                       send            (range<byte const>)                = 0 ;
        virtual void                       close           ()                                 = 0 ;
        virtual tcp_session::endpoint_type local_endpoint  ()                           const = 0 ;
        virtual tcp_session::endpoint_type remote_endpoint ()                           const = 0 ;
        virtual tcp_session::endpoint_type remote_endpoint (boost::system::error_code&) const = 0 ;
        virtual void                       set_on_received (on_received)                      = 0 ;
        virtual void                       set_on_sent     (on_sent)                          = 0 ;
        virtual void                       set_on_error    (on_error)                         = 0 ;

    protected:
        ~tcp_session_interface() {}
    };
    typedef tcp_session_interface* tcp_session_interface_ptr;
}
