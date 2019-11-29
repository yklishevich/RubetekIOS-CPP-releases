#pragma once

#include <string>

#include <syslog.h>

#include <boost/algorithm/string/case_conv.hpp>


namespace dawn
{
    int syslog_facility_from_string(std::string const&);
}


namespace dawn
{
    inline int syslog_facility_from_string(std::string const& s)
    {
        std::string const fac = boost::algorithm::to_lower_copy(s);
             if (s == "auth"    ) return LOG_AUTH     ;
        else if (s == "authpriv") return LOG_AUTHPRIV ;
        else if (s == "cron"    ) return LOG_CRON     ;
        else if (s == "daemon"  ) return LOG_DAEMON   ;
        else if (s == "ftp"     ) return LOG_FTP      ;
        else if (s == "kern"    ) return LOG_KERN     ;
        else if (s == "local0"  ) return LOG_LOCAL0   ;
        else if (s == "local1"  ) return LOG_LOCAL1   ;
        else if (s == "local2"  ) return LOG_LOCAL2   ;
        else if (s == "local3"  ) return LOG_LOCAL3   ;
        else if (s == "local4"  ) return LOG_LOCAL4   ;
        else if (s == "local5"  ) return LOG_LOCAL5   ;
        else if (s == "local6"  ) return LOG_LOCAL6   ;
        else if (s == "local7"  ) return LOG_LOCAL7   ;
        else if (s == "mail"    ) return LOG_MAIL     ;
        else if (s == "news"    ) return LOG_NEWS     ;
        else if (s == "syslog"  ) return LOG_SYSLOG   ;
        else if (s == "user"    ) return LOG_USER     ;
        else if (s == "uucp"    ) return LOG_UUCP     ;
        throw std::runtime_error("can't conver facility from string '" + s + "'");
    }
}
