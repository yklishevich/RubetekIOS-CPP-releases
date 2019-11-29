#pragma once 


namespace msw
{
    struct preconstructor
    {
        template <typename Fn>
        preconstructor(Fn&& f)
        {
            f();
        }
    };
}
