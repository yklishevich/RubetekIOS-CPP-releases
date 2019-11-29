#pragma once

#include <set>

#include <msw/application/wrapper.hpp>
#include <msw/system_signal/catcher.hpp>


namespace msw         {
namespace application {

    template <typename App, typename ...Args>
    void main(std::set<system_signal> const& signals, Args&&... args)
    {
        signal_catcher signal_catcher ( signals                       );
        wrapper<App>   wrapper        ( (std::forward<Args>(args))... );
        // expressin (std::forward<Args>(args)) wrapped in brackets specially. bug msvc, when args: 2 arguments (entry_point::classic)

        while (wrapper.executing())
        {
            system_signal s = signal_catcher.wait_signal();
            if (s != system_signal::none) wrapper.process_signal(s);
        }
    }

}}
