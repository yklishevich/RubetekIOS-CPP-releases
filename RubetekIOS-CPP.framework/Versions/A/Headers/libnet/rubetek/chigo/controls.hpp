#pragma once

#include <msw/range.hpp>


namespace chigo
{
    enum class electric_control : msw::byte
    {
          autom
        , start
        , stop
    };
    enum class speeds_control : msw::byte
    {
          autom
        , high
        , mid
        , low
    };
    enum class damper_control : msw::byte
    {
          natural
        , swing
        , fixed
    };
    enum class mode_control : msw::byte
    {
          cooling = 0x01
        , dry
        , ventilation
        , heating
        , autom
    };
}
