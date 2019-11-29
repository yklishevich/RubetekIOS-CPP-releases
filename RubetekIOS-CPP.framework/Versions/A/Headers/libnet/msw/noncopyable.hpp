#pragma once


namespace msw
{
    struct noncopyable
    {
                     noncopyable () {}
    private:
                     noncopyable (noncopyable const&) = delete ;
        noncopyable& operator =  (noncopyable const&) = delete ;
    };
}
