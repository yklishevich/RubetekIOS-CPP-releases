#pragma once

#include <msw/entry_point/classic.hpp>
#include <msw/application/main.hpp>


#define MSW_APPLICATION(A, ...)\
void msw::main(int argc, char* argv[])\
{\
    msw::application::main<A>({__VA_ARGS__}, argc, argv);\
}
