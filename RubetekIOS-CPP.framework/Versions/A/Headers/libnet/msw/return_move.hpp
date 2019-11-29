#pragma once

#include <msw/config.hpp>


#ifdef MSW_CLANG
#   define MSW_RETURN_MOVE(X) return X;
#else
#   define MSW_RETURN_MOVE(X) return std::move(X);
#endif
