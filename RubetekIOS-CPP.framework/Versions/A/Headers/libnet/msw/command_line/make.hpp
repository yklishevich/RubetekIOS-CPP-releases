#pragma once

#include <msw/config.hpp>
#include <msw/command_line/command_line.hpp>


namespace msw
{
    command_line make_command_line(int, char*[]);
#ifdef MSW_MSVC
    command_line make_command_line();
#endif
}


namespace msw
{
    inline command_line make_command_line(int argc, char* argv[])
    {
        std::string                  command   ;
        command_line::arguments_type arguments ;
        if (argc)
        {
            command = argv[0];
            for (int n = 1; n != argc; ++n)
                arguments.push_back(argv[n]);
        }
        return command_line(std::move(command), std::move(arguments));
    }
#ifdef MSW_MSVC
    inline command_line make_command_line()
    {
        return make_command_line(__argc, __argv);
    }
#endif
}
