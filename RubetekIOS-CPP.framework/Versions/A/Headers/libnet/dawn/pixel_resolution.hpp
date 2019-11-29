#pragma once


namespace dawn
{
    struct pixel_resolution
    {
        unsigned width  ;
        unsigned height ;

        pixel_resolution()
            : width  ( 0 )
            , height ( 0 )
        {}
        pixel_resolution(unsigned width_, unsigned height_)
            : width  ( width_  )
            , height ( height_ )
        {}
        unsigned frame_bytes_count(unsigned bytes_per_pixel = 3) const
        {
            return width * height * bytes_per_pixel;
        }
        bool empty() const
        {
            return (width == 0) && (height == 0);
        }
    };
}
