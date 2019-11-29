#pragma once

#include <msw/command_line/command_line.hpp>


namespace msw
{
    command_line make_command_line(void*, void*, char const*, int);
}


namespace msw
{
    inline command_line make_command_line(void*, void*, char const* cmd_line, int)
    {
        command_line::arguments_type arguments;
        std::string cmdline(cmd_line);
        std::size_t space_pos;
        while ((space_pos = cmdline.find(" ")) != std::string::npos)
        {
            arguments.push_back(cmdline.substr(0, space_pos));
            cmdline = cmdline.substr(space_pos + 1, cmdline.size() - space_pos - 1);
        }
        if (!cmdline.empty()) arguments.push_back(cmdline);
        return command_line(std::string(), std::move(arguments));
    }
}
