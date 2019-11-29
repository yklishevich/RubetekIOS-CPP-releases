#include <msw/config.hpp>

#ifdef MSW_MSVC
#   pragma warning (push)
#   pragma warning (disable: 4244)
#endif

extern "C"
{
    #include "libavutil/ffversion.h"
    #include "libavcodec/avcodec.h"
    #include "libavformat/avformat.h"
    #include "libavutil/avutil.h"
    #include "libavutil/imgutils.h"
    #include "libswscale/swscale.h"
}

#ifdef MSW_MSVC
#   pragma warning (pop)
#endif
