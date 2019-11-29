#pragma once

#include <string>


namespace dawn
{
    struct email_letter
    {
        std::string address ;
        std::string from    ;
        std::string to      ;
        std::string subject ;
        std::string text    ;
    };
}
