#pragma once

#include <msw/noncopyable.hpp>
#include <msw/throw_runtime_error.hpp>

#include <lama/ffmpeg/find_decoder.hpp>
#include <lama/ffmpeg/include/ffmpeg.hpp>


namespace lama   {
namespace ffmpeg {

    struct codec_context
        : msw::noncopyable
        , initializer
    {
        explicit codec_context(AVCodec* codec)
            : context_(::avcodec_alloc_context3(codec))
        {
            if (!context_) msw::throw_runtime_error("ffmpeg: alloc context fail");
        }
        explicit codec_context(AVCodecID codec_id)
            : codec_context(find_decoder(codec_id))
        {}
        ~codec_context()
        {
            ::avcodec_free_context(&context_);
        }
        AVCodecContext* native() const
        {
            return context_;
        }
    private:
        AVCodecContext* context_;
    };

}}
