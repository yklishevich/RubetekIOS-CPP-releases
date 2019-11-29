#pragma once


namespace msw {
namespace zzz {

#ifdef _WINDEF_
    typedef HINSTANCE hinstance;
#else
    typedef void*     hinstance;
#endif

}}

int __stdcall WinMain(msw::zzz::hinstance, msw::zzz::hinstance, char*, int)
{
    return main(__argc, __argv);
}

// on error: 'WinMain': function cannot be overloaded, need to include 'windows.h' before 'msw/entry_point/...'
