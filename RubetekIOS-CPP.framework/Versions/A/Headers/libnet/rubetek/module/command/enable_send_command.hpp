#pragma once

#include <rubetek/essence/unique_id/editor.hpp>
#include <rubetek/module/command/content.hpp>
#include <rubetek/module/command/send_command_impl.hpp>
#include <rubetek/utility/lock_buffer_queue.hpp>


namespace rubetek {
namespace module  {

    struct enable_send_command
        : private send_command_impl
    {
        void send_command  (unique_editor_id_type, command_data, command_size) ;
        void put_command   (unique_editor_id_type, command_data, command_size) ;
        void send_commands ()                                                  ;
    private:
        utility::lock_buffer_queue<unique_editor_id_type> queue_;
    };

}}


namespace rubetek {
namespace module  {

    inline void enable_send_command::send_command(unique_editor_id_type id, command_data data, command_size size)
    {
        send(utility::raw_memory(reinterpret_cast<unsigned char const*>(id.data()), id.size()), utility::raw_memory(data, size));
    }
    inline void enable_send_command::put_command(unique_editor_id_type id, command_data data, command_size size)
    {
        auto command = utility::raw_memory(data, size);
        queue_.push(id, buffer(command.begin(), command.end()));
    }
    inline void enable_send_command::send_commands()
    {
        queue_.eject([this](unique_editor_id_type id, buffer const& buf)
        {
            send_command(id, buf.data(), static_cast<command_size>(buf.size()));
        });
    }

}}
