#pragma once


namespace msw
{
    struct nonassignable
    {
                       nonassignable () {}
    private:
        nonassignable& operator =    (nonassignable const&) = delete;
    };
}
