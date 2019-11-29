#pragma once

#include <msw/std/memory.hpp>
#include <msw/noncopyable.hpp>

#include <lama/ffmpeg/frame.hpp>
#include <lama/ffmpeg/decoder.hpp>
#include <lama/ffmpeg/include/ffmpeg.hpp>
#include <lama/ffmpeg/frame_converter.hpp>


namespace lama   {
namespace ffmpeg {

    template <typename Decoder = decoder>
    struct rgb_decoder
        : msw::noncopyable
        , initializer
    {
        typedef          Decoder              decoder     ;
        typedef typename decoder::frame_ready frame_ready ;

        rgb_decoder(AVCodecID codec_id, frame_ready frame_ready)
            : decoder_
            (
                codec_id
                , [this, frame_ready](frame const& f)
                {
                    AVCodecContext* ctx = decoder_.context().native();
                    if (!frame_converter_) frame_converter_ = msw::make_unique<frame_converter>(ctx->width, ctx->height, ctx->pix_fmt, AV_PIX_FMT_RGB24);
                    frame_ready(frame_converter_->convert(f));
                }
            )
        {}
        void decode(msw::range<msw::byte const> block)
        {
            decoder_.decode(block);
        }
        msw::uint64 errors_count() const
        {
            return decoder_.errors_count();
        }
    private:
        decoder                          decoder_         ;
        std::unique_ptr<frame_converter> frame_converter_ ;
    };

}}
