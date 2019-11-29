#pragma once

#include <msw/noncopyable.hpp>
#include <msw/throw_runtime_error.hpp>

#include <lama/ffmpeg/frame.hpp>
#include <lama/ffmpeg/include/ffmpeg.hpp>


namespace lama   {
namespace ffmpeg {

    struct frame_converter
        : msw::noncopyable
        , initializer
    {
        frame_converter(int width, int height, AVPixelFormat src_format, AVPixelFormat dst_format)
            : height_  (height)
            , context_ (::sws_getContext(width, height, src_format, width, height, dst_format, SWS_BICUBIC, 0, 0, 0))
        {
            if (!context_) msw::throw_runtime_error("ffmpeg: sws get context fail");
            frame_.image_alloc(width, height, dst_format);
        }
        ~frame_converter()
        {
            ::sws_freeContext(context_);
        }
        frame& convert(frame const& src_frame)
        {
            ::sws_scale(context_, src_frame.native()->data, src_frame.native()->linesize, 0, height_, frame_.native()->data, frame_.native()->linesize);
            return frame_;
        }
    private:
        int         const height_  ;
        SwsContext*       context_ ;
        frame             frame_   ;
    };

}}
