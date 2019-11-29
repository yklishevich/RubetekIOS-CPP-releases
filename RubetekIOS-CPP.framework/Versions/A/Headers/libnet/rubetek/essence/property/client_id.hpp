#pragma once

#include <rubetek/essence/property/id.hpp>


namespace rubetek
{
    enum class client_property_id
        : property_id_type
    {
          error
        , selector
        , label
        , charge
        , power
        , interval
        , timeout
        , roller_move
        , camera_port
        , camera_url
        , tv_command
        , tv_auth
        , energy
        , settings
        , thermostat_mode
        , thermostat_temp
        , thermostat_settings
    };

    enum class tv_command_id
    {
          mute
        , volume_up
        , volume_down
        , power_on
        , power_off
        , test
    };
}
