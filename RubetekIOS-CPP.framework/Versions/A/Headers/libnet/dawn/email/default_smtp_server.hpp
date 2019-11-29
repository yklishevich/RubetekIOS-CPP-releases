#pragma once

#include <string>

#include <dawn/email/extract_host.hpp>


namespace dawn
{
    inline std::string default_smtp_server(std::string const& address)
    {
        return "smtp." + email_extract_host(address);
    }
}
