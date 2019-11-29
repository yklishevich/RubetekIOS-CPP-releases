#pragma once

extern "C"
{
    #include "libavutil/ffversion.h"
}


namespace msw
{
    static std::string version_of_ffmpeg()
    {
        return FFMPEG_VERSION;
    }
}
