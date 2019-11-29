#pragma once

#include <string>

#include <msw/return_move.hpp>
#include <msw/system_error/last_code.hpp>
#include <msw/system_error/description.hpp>
#include <msw/lexical_cast/int_to/dec.hpp>
#include <msw/lexical_cast/int_to/hex.hpp>


namespace msw          {
namespace system_error {

    inline std::string last_message()
    {
        int const err_code = last_code();
        std::string msg;
        msg += "[system error ";
        msg += int_to_dec(err_code).data().native();
        msg += " (0x";
        msg += int_to_hex(err_code).data().native();
        msg += "): ";
        msg += description(err_code);
        msg += "]";
        MSW_RETURN_MOVE(msg);
    }

}}
