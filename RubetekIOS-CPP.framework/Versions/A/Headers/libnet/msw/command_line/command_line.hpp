#pragma once

#include <string>
#include <vector>

#include <msw/noncopyable.hpp>
#include <msw/plain_types.hpp>
#include <msw/throw_runtime_error.hpp>


namespace msw
{
    struct command_line
        : noncopyable
    {
        typedef std::vector<std::string> arguments_type ;

        template <typename Command, typename Arguments>
        command_line(Command&&, Arguments&&);

        command_line(command_line&&);

        std::string    const& command         ()          const ;
        arguments_type const& arguments       ()          const ;
        bool                  has_arguments   ()          const ;
        msw::uint             arguments_count ()          const ;

        std::string    const& operator[]      (msw::uint) const ;

    private:
        std::string    const command_   ;
        arguments_type const arguments_ ;
    };
}


namespace msw
{
    template <typename Command, typename Arguments>
    command_line::command_line(Command&& command, Arguments&& arguments)
        : command_   ( std::forward<Command>(command)     )
        , arguments_ ( std::forward<Arguments>(arguments) )
    {}

    inline command_line::command_line(command_line&& other)
        : command_   ( std::move(other.command_)   )
        , arguments_ ( std::move(other.arguments_) )
    {}

    inline std::string const& command_line::command() const
    {
        return command_;
    }
    inline command_line::arguments_type const& command_line::arguments() const
    {
        return arguments_;
    }
    inline bool command_line::has_arguments() const
    {
        return !arguments_.empty();
    }
    inline msw::uint command_line::arguments_count() const
    {
        return arguments_.size();
    }

    inline std::string const& command_line::operator[](msw::uint index) const
    {
        if (index)
        {
            if (index > arguments_.size()) throw_runtime_error("command line: attempt access to '", index, "' argument, when only ", arguments_.size(), " argument(s)");
            return arguments_[index - 1];
        }
        return command_;
    }
}
