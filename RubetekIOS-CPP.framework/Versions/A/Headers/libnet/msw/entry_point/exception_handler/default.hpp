#pragma once

#include <msw/entry_point/exception_handler/default_ex.hpp>


namespace msw         {
namespace entry_point {

    int default_exception_handler         (std::exception const&) ;
    int default_unknown_exception_handler ()                      ;

}}


namespace msw         {
namespace entry_point {

    inline int default_exception_handler(std::exception const& e)
    {
        return default_exception_handler_ex(e, "main");
    }

    inline int default_unknown_exception_handler()
    {
        return default_unknown_exception_handler_ex("main");
    }

}}
