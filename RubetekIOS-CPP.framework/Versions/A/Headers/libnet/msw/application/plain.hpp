#pragma once

#include <msw/entry_point/plain.hpp>
#include <msw/application/main.hpp>


#define MSW_APPLICATION(A, ...)\
void msw::main()\
{\
    msw::application::main<A>({__VA_ARGS__});\
}
