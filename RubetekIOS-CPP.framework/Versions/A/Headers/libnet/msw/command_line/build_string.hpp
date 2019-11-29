#pragma once

#include <string>
#include <vector>


namespace msw
{
    inline std::string build_command_line_string()
    {
        return "";
    }
    template <typename ...Args>
    std::string build_command_line_string(std::string const& arg, Args ...args)
    {
        std::string const rhs = build_command_line_string(std::forward<Args>(args)...);
        return arg.empty() ? rhs : (arg + (rhs.empty() ? rhs : (" " + rhs)));
    }
    inline std::string build_command_line_string(std::vector<std::string> const& args)
    {
        std::string out;
        for (std::string const& arg : args)
        {
            if (!arg.empty())
            {
                if (!out.empty()) out.push_back(' ');
                out += arg;
            }
        }
        return std::move(out);
    }
}
