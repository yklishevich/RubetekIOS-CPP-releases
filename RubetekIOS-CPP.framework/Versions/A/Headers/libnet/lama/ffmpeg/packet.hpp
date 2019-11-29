#pragma once

#include <msw/range.hpp>
#include <msw/noncopyable.hpp>
#include <msw/throw_runtime_error.hpp>

#include <lama/ffmpeg/include/ffmpeg.hpp>


namespace lama   {
namespace ffmpeg {

    struct packet
        : msw::noncopyable
        , initializer
    {
        packet()
        {
            ::av_init_packet(&packet_);
        }
        void set_data(msw::range<msw::byte const> block)
        {
            packet_.data = const_cast<msw::byte*>(block.data().native());
            packet_.size = static_cast<int>(block.size().count());
        }
        AVPacket& native()
        {
            return packet_;
        }
    private:
        AVPacket packet_;
    };

}}
