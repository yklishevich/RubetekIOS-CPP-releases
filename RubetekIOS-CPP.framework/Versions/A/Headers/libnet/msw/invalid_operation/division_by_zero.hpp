#pragma once 


namespace msw
{
    inline void division_by_zero()
    {
        int x = 1;
        int z = 0;
        x = x / z;
        z++;
    }
}
