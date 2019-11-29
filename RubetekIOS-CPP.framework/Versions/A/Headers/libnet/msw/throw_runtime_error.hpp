#pragma once

#include <stdexcept>

#include <msw/format.hpp>


namespace msw
{
    template <typename ...Args>
    void throw_runtime_error(Args&& ...args)
    {
        throw std::runtime_error(msw::format(std::forward<Args>(args)...));
    }
}
