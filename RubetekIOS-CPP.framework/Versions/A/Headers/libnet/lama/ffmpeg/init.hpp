#pragma once

#include <msw/single.hpp>
#include <msw/noncopyable.hpp>

#include <lama/ffmpeg/include/ffmpeg.hpp>


namespace lama   {
namespace ffmpeg {

    struct init_impl
        : msw::noncopyable
    {
        init_impl()
        {
            ::avcodec_register_all();
        }
    };

    inline void init()
    {
        msw::single<init_impl>();
    }

    struct initializer
    {
        initializer()
        {
            init();
        }
    };

}}
