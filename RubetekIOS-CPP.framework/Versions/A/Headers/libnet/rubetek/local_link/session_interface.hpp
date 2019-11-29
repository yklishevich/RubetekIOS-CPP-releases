#pragma once

#include <string>

#include <msw/range.hpp>
#include <msw/noncopyable.hpp>
#include <msw/callback/byte_range.hpp>


namespace rubetek
{
    struct local_link_session_interface
        : msw::noncopyable
    {
        typedef msw::callback::byte_const_range on_received;

        virtual void        send       (msw::range<msw::byte const>)       = 0 ;
        virtual std::string ip_address ()                            const = 0 ;

    protected:
        ~local_link_session_interface() {};
    };
}
