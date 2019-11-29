#pragma once

#include <string>
#include <iostream>
#include <exception>


namespace msw         {
namespace entry_point {

    int console_exception_handler         (std::exception const&, std::string const& thread_name) ;
    int console_unknown_exception_handler (std::string const& thread_name)                        ;

}}


namespace msw         {
namespace entry_point {

    inline int console_exception_handler(std::exception const& e, std::string const& thread_name)
    {
        std::cout << "exception in the " << thread_name << " thread: " << e.what() << std::endl;
        return -1;
    }

    inline int console_unknown_exception_handler(std::string const& thread_name)
    {
        std::cout << "unknown exception in the " << thread_name << " thread\n";
        return -1;
    }

}}
