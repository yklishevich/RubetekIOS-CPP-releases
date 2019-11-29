#pragma once

#include <functional>

#include <msw/range.hpp>
#include <msw/noncopyable.hpp>
#include <msw/dimension/counter.hpp>

#include <lama/ffmpeg/frame.hpp>
#include <lama/ffmpeg/packet.hpp>
#include <lama/ffmpeg/codec_context.hpp>
#include <lama/ffmpeg/include/ffmpeg.hpp>


namespace lama   {
namespace ffmpeg {

    struct decoder
        : msw::noncopyable
        , initializer
    {
        typedef std::function<void(frame const&)> frame_ready;

        decoder(AVCodecID codec_id, frame_ready frame_ready)
            : frame_ready_   ( frame_ready )
            , codec_context_ ( codec_id    )
        {
            if (::avcodec_open2(codec_context_.native(), find_decoder(codec_id), 0) < 0) msw::throw_runtime_error("ffmpeg: open2 fail");
        }
        ~decoder()
        {
            ::avcodec_close(codec_context_.native());
        }
        void decode(msw::range<msw::byte const> block)
        {
            packet_.set_data(block);
            if (::avcodec_send_packet(codec_context_.native(), &packet_.native())) errors_count_++;
            // а если кадр состоит из нескольких кодовых слов или кодовое слово содержит несколько кадров !?
            if (::avcodec_receive_frame(codec_context_.native(), frame_.native()) >= 0) frame_ready_(frame_);
            else errors_count_++;
        }
        msw::uint64 errors_count() const
        {
            return errors_count_.value();
        }
        codec_context const& context() const
        {
            return codec_context_;
        }
    private:

        frame_ready               const frame_ready_   ;
        codec_context                   codec_context_ ;
        packet                          packet_        ;
        frame                           frame_         ;
        msw::counter<msw::uint64>       errors_count_  ;
    };

}}
