#pragma once

#include <functional>

#include <msw/buffer.hpp>
#include <msw/noncopyable.hpp>
#include <msw/dimension/counter.hpp>

#include <lama/ffmpeg/frame.hpp>
#include <lama/ffmpeg/packet.hpp>
#include <lama/ffmpeg/codec_context.hpp>
#include <lama/ffmpeg/include/ffmpeg.hpp>


namespace lama   {
namespace ffmpeg {

    struct legacy_decoder
        : msw::noncopyable
        , initializer
    {
        typedef std::function<void(frame const&)> frame_ready;

        legacy_decoder(AVCodecID codec_id, frame_ready frame_ready)
            : frame_ready_   ( frame_ready )
            , codec_context_ ( codec_id    )
        {
            if (::avcodec_open2(codec_context_.native(), find_decoder(codec_id), 0) < 0) msw::throw_runtime_error("ffmpeg: open2 fail");
        }
        ~legacy_decoder()
        {
            ::avcodec_close(codec_context_.native());
        }
        void decode(msw::range<msw::byte const> block)
        {
            if (block_.has_items())
            {
                block_.push_back(block);
                block = block_.all();
            }
            process(block);
            block_ = block;
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
        void process(msw::range<msw::byte const>& block)
        {
            int got_picture = 0;
            while (block.has_items())
            {
                packet_.set_data(block);
                int processed_size = ::avcodec_decode_video2(codec_context_.native(), frame_.native(), &got_picture, &packet_.native());
                if (processed_size > 0)
                {
                    block.pop_front(processed_size);
                    if (got_picture) frame_ready_(frame_);
                }
                else
                {
                    if (processed_size)
                    {
                        block.clear();
                        errors_count_++;
                    }
                    break;
                }
            }
        }

        frame_ready               const frame_ready_   ;
        codec_context                   codec_context_ ;
        packet                          packet_        ;
        frame                           frame_         ;
        msw::buffer<msw::byte>          block_         ;
        msw::counter<msw::uint64>       errors_count_  ;
    };

}}
