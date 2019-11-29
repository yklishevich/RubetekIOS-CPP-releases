#pragma once

#include <msw/throw_runtime_error.hpp>

#include <lama/ffmpeg/init.hpp>
#include <lama/ffmpeg/include/ffmpeg.hpp>


namespace lama   {
namespace ffmpeg {

    inline AVCodec* find_decoder(AVCodecID codec_id)
    {
        init();
        AVCodec* const codec = ::avcodec_find_decoder(codec_id);
        if (!codec) msw::throw_runtime_error("ffmpeg: can't find decoder ", codec_id);
        return codec;
    }

}}
