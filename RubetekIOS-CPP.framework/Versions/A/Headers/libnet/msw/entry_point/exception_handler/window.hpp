#pragma once

#include <string>
#include <exception>

#pragma comment(lib, "user32.lib")

struct HWND__;

extern "C" __declspec(dllimport) int __stdcall MessageBoxA(HWND__*, char const*, char const*, unsigned);


namespace msw         {
namespace entry_point {

    int window_exception_handler         (std::exception const&, std::string const& thread_name) ;
    int window_unknown_exception_handler (std::string const& thread_name)                        ;

}}


namespace msw         {
namespace entry_point {

    inline int window_exception_handler(std::exception const& e, std::string const& thread_name)
    {
        std::string const msg = std::string("Exception in the ") + thread_name + " thread";
        ::MessageBoxA(0, e.what(), msg.c_str(), 0x10);
        return -1;
    }

    inline int window_unknown_exception_handler(std::string const& thread_name)
    {
        std::string const msg = std::string("Exception in the ") + thread_name + " thread";
        ::MessageBoxA(0, msg.c_str(), "Error", 0x10);
        return -1;
    }

}}
