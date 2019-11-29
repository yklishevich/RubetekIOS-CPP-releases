#pragma once

#include <rubetek/module/interface.hpp>


namespace rubetek {
namespace module  {

    struct reload_clients_list_impl
    {
        void reload_clients_list();

        static module_controller_link controller_link;
    };

}}


namespace rubetek {
namespace module  {

    module_controller_link reload_clients_list_impl::controller_link = 0;

    inline void reload_clients_list_impl::reload_clients_list()
    {
        assert(controller_link);
        controller_link->reload_clients_list();
    }

}}
