#pragma once

#include <functional>

#include <msw/noncopyable.hpp>


namespace msw
{
    struct scoped_shut
        : noncopyable
    {
        typedef std::function<void()> on_shut;

        explicit scoped_shut  (on_shut) ;
                 ~scoped_shut ()        ;

    private:
        on_shut on_shut_;
    };
}


namespace msw
{
    inline scoped_shut::scoped_shut(on_shut on_shut)
        : on_shut_(on_shut)
    {}
    inline scoped_shut::~scoped_shut()
    {
        on_shut_();
    }
}


#define MSW_SCOPED_SHUT(...)\
    msw::scoped_shut scoped_shut\
    (\
        [&]()\
        {\
            __VA_ARGS__;\
        }\
    );
