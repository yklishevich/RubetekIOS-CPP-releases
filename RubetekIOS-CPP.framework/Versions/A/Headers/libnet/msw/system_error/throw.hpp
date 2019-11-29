#pragma once

#include <string>
#include <stdexcept>

#include <msw/format.hpp>
#include <msw/throw_runtime_error.hpp>
#include <msw/system_error/last_message.hpp>


namespace msw          {
namespace system_error {

    inline void throw_exception(std::string msg)
    {
        throw_runtime_error(msg, " ", last_message());
    }

    template <typename ...Args>
    void throw_exception(Args&& ...args)
    {
        throw_exception(format(std::forward<Args>(args)...));
    }

}}
