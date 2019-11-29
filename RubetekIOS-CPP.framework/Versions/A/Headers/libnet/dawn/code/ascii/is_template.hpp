#pragma once


#define DAWN_ASCII_IS_TEMPLATE(Name, First, Last)\
    inline bool is_##Name(char c)\
    {\
        return (int(c) >= First) && (int(c) <= Last);\
    }\
    inline bool is_##Name##s(std::string const& s)\
    {\
        for (auto c : s) if (!is_##Name(c)) return false;\
        return true;\
    }
