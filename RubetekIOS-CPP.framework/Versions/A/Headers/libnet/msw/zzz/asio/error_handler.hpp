#pragma once

#include <boost/system/system_error.hpp>

#include <msw/zzz/asio/on_error.hpp>


namespace msw
{
    struct error_handler
    {
        explicit error_handler (on_error)                  ;
        void     operator ()   (boost::system::error_code) ;
    private:
        on_error on_error_;
    };
}


namespace msw
{
    inline error_handler::error_handler(on_error on_error)
        : on_error_(on_error)
    {}
    inline void error_handler::operator () (boost::system::error_code ec)
    {
        if (on_error_) (on_error(on_error_))(ec);
        else throw boost::system::system_error(ec);
    }
}
