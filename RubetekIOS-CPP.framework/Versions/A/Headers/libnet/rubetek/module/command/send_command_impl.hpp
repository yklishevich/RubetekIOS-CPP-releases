#pragma once

#include <rubetek/module/interface.hpp>
#include <rubetek/utility/raw_memory.hpp>
#include <rubetek/essence/unique_id/editor.hpp>


namespace rubetek {
namespace module  {

    struct send_command_impl
    {
        void send(utility::raw_memory const& editor_id, utility::raw_memory const&);

        static module_controller_link controller_link;
    };

}}


namespace rubetek {
namespace module  {

    module_controller_link send_command_impl::controller_link = 0;

    void send_command_impl::send(utility::raw_memory const& editor_id, utility::raw_memory const& command)
    {
        assert(controller_link);
        controller_link->send_command(editor_id, command);
    }

}}
