#pragma once

#include <msw/plain_types.hpp>


namespace rubetek {
namespace ihup    {

    enum class cmd : msw::byte
    {
          init
        , close
        , version
        , project_settings
        , create_instance
        , free_instance
        , open_device
        , close_device
        , properties
        , editor_command
        , log
        , accessible
        , system_command
        , reload_clients_list
        , packet
    };

}}
