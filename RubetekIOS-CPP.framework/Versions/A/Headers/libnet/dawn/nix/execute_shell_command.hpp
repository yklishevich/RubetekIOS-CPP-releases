#pragma once

#include <string>
#include <cstdio>
#include <stdexcept>


namespace dawn {
namespace nix  {

    inline std::string execute_shell_command(std::string command)
    {
        char buffer[256];
        std::string result;
        char const* cmd = command.c_str();
        FILE* pipe = popen(cmd, "r");
        if (!pipe) throw std::runtime_error("execute_shell_command: popen() failed!");
        try
        {
            while (!feof(pipe))
                if (fgets(buffer, 128, pipe))
                    result += buffer;
        }
        catch (...)
        {
            pclose(pipe);
            throw;
        }
        pclose(pipe);
        return result;
    }

}}
