#pragma once

#include <string>
#include <functional>

#include <rubetek/module/log/level.hpp>
#include <rubetek/module/log/interface.hpp>


namespace rubetek      {
namespace module_shell {

    struct module_log
        : module::log::log_controller_interface
    {
        typedef std::function<void(module::log::level, std::string const& msg)> push_message;

        explicit module_log(push_message push_message)
            : push_message_(push_message)
        {}
    private:

        void write(char const* msg, rubetek::module::log::level level) const
        {
            push_message_(level, msg);
        }

        push_message push_message_;
    };

}}
