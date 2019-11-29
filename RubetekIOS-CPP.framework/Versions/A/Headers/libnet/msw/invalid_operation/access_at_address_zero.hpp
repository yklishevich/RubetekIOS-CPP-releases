#pragma once 


namespace msw
{
    inline void access_at_address_zero()
    {
        int* p = 0;
        *p = 1;
    }
}
