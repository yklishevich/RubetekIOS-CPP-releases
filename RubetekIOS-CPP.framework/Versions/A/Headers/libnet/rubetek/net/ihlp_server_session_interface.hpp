#pragma once

#include <string>
#include <functional>

#include <msw/range.hpp>
#include <msw/noncopyable.hpp>
#include <msw/asio/tcp_session.hpp>


namespace rubetek
{
    struct ihlp_server_session_interface
        : msw::noncopyable
    {
        typedef msw::tcp_session::on_received   on_received   ;
        typedef msw::tcp_session::endpoint_type endpoint_type ;
        typedef boost::asio::ip::address        ip_address    ;

        virtual std::string   name            ()                            const = 0 ;
        virtual endpoint_type remote_endpoint (boost::system::error_code&)  const = 0 ;
        virtual void          send            (msw::range<msw::byte const>)       = 0 ;
        virtual void          close           ()                                  = 0 ;
        virtual void          set_on_received (on_received)                       = 0 ;
        virtual ip_address    remote_ip       ()                            const = 0 ;

    protected:
        ~ihlp_server_session_interface() {}
    };
    typedef ihlp_server_session_interface* ihlp_server_session_interface_ptr;
}
