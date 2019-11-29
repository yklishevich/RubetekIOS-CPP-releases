#pragma once

#include <msw/noncopyable.hpp>
#include <msw/make_range_native.hpp>
#include <msw/dimension/trigger.hpp>
#include <msw/throw_runtime_error.hpp>

#include <lama/ffmpeg/find_decoder.hpp>
#include <lama/ffmpeg/include/ffmpeg.hpp>


namespace lama   {
namespace ffmpeg {

    struct frame
        : msw::noncopyable
        , initializer
    {
        frame()
            : frame_(::av_frame_alloc())
        {
            if (!frame_) msw::throw_runtime_error("ffmpeg: alloc frame fail");
        }
        ~frame()
        {
            free_data();
            ::av_frame_free(&frame_);
        }
        void image_alloc(int width, int height,  AVPixelFormat format)
        {
            free_data();
            if (::av_image_alloc(frame_->data, frame_->linesize, width, height, format, 1) < 0)
                msw::throw_runtime_error("ffmpeg: image alloc fail");
            allocated_.set();
            frame_->width  = width  ;
            frame_->height = height ;
            frame_->format = format ;
        }
        AVFrame* native() const
        {
            return frame_;
        }
        msw::range<msw::byte const> raw() const
        {
            return msw::make_range_native(frame_->data[0], ::av_image_get_buffer_size(AVPixelFormat(frame_->format), frame_->width, frame_->height, 1));
        }
        int width() const
        {
            return frame_->width;
        }
        int height() const
        {
            return frame_->height;
        }
    private:
        void free_data()
        {
            if (allocated_) ::av_free(frame_->data[0]);
        }

        AVFrame*     frame_     ;
        msw::trigger allocated_ ;
    };

}}
