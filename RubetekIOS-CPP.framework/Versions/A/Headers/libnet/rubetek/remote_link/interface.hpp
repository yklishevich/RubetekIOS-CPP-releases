#pragma once

#include <msw/range.hpp>
#include <msw/noncopyable.hpp>
#include <msw/callback/byte_range.hpp>


namespace rubetek
{
    template <typename Context = void*>
    struct remote_link_interface
        : msw::noncopyable
    {
        typedef Context                         context_type ;
        typedef msw::callback::byte_const_range on_received  ;

        virtual void          set_on_received (on_received)                 = 0 ;
        virtual void          send            (msw::range<msw::byte const>) = 0 ;
        virtual context_type& get_context     ()                            = 0 ;

    protected:
        ~remote_link_interface() {};
    };
}
