#pragma once

#include <string>


#define RUBETEK_STRING_ESSENCE(Name)\
    struct Name\
    {\
        typedef std::string type            ;\
        typedef type&       reference       ;\
        typedef type const& const_reference ;\
    };
