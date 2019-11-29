#ifndef CC_SYSTEM_COMMAND_HPP
#define CC_SYSTEM_COMMAND_HPP

#include <sys/types.h>
#include <stdlib.h>

#include <string>

namespace rubetek
{
    namespace module
    {
        std::string run_system_command(const std::string &command)
        {
            std::string content;

            FILE *file_stream = popen(command.c_str(), "r");

            if (file_stream)
            {
                char buff[1024];

                while (fgets(buff, sizeof(buff), file_stream))
                {
                    content += std::string(buff);
                }

                pclose(file_stream);
            }
            return content;
        }
    }
}

#endif //CC_SYSTEM_COMMAND_HPP
