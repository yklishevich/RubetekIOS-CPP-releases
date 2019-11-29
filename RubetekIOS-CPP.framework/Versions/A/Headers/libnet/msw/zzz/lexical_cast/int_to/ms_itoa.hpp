#pragma once

#include <cstdlib>        // _itoa_s, _i64toa_s, _ui64toa_s

#include <msw/assert.hpp>


namespace msw {
namespace zzz {

    inline int ms_itoa_impl(int v, char* buf, int len)
    {
        return _itoa_s(v, buf, len, 10);
    }

    inline int ms_itoa_impl(unsigned v, char* buf, int len)
    {
        return _ui64toa_s(v, buf, len, 10);
    }

    inline int ms_itoa_impl(long long v, char* buf, int len)
    {
        return _i64toa_s(v, buf, len, 10);
    }

    inline int ms_itoa_impl(unsigned long long v, char* buf, int len)
    {
        return _ui64toa_s(v, buf, len, 10);
    }

    template <typename T>
    void ms_itoa(T v, char* buf, int len)
    {
        MSW_ASSERT(len);
        if (ms_itoa_impl(v, buf, len)) buf[0] = 0;
    }

}}
