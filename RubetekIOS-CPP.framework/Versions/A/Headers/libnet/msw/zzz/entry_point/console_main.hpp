#pragma once

#include <locale>

#include <msw/zzz/entry_point/call_fwd.hpp>


struct locale_numpunct
    : std::numpunct<char>
{
    std::string do_grouping() const { return "\0"; }
};


int main(int argc, char* argv[])
{
    std::locale::global(std::locale(""));
    std::locale::global(std::locale(std::locale(""), new locale_numpunct));
    return msw::entry_point::zzz::call(argc, argv);
}
