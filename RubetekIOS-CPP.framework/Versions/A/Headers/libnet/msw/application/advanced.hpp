#pragma once

#include <msw/entry_point/advanced.hpp>
#include <msw/application/main.hpp>


#define MSW_APPLICATION(A, ...)\
void msw::main(command_line cmd_line)\
{\
    msw::application::main<A>({__VA_ARGS__}, std::move(cmd_line));\
}
