#pragma once

#include <rubetek/module/reload_clients_list/impl.hpp>


namespace rubetek {
namespace module  {

    struct enable_reload_clients_list
        : private reload_clients_list_impl
    {
        using reload_clients_list_impl::reload_clients_list;
    };

}}
