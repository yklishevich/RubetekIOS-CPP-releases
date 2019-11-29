#pragma once

#include <string>

#include <boost/lexical_cast.hpp>

#include <msw/size.hpp>
#include <msw/plain_types.hpp>


namespace dawn
{
    enum class log_io_mode
    {
          disable
        , only_size
        , cut16
        , cut32
        , cut64
        , cut128
        , full
    };

    inline log_io_mode log_io_mode_from_string(std::string const& mode)
    {
        if      ( mode == "disable"   ) return log_io_mode::disable   ;
        else if ( mode == "only_size" ) return log_io_mode::only_size ;
        else if ( mode == "cut16"     ) return log_io_mode::cut16     ;
        else if ( mode == "cut32"     ) return log_io_mode::cut32     ;
        else if ( mode == "cut64"     ) return log_io_mode::cut64     ;
        else if ( mode == "cut128"    ) return log_io_mode::cut128    ;
        else if ( mode == "full"      ) return log_io_mode::full      ;
        throw std::runtime_error("can't convert log io mode from string: " + mode);
    }
}
