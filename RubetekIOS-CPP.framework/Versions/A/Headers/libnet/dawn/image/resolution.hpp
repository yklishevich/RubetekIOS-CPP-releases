#pragma once


namespace dawn  {
namespace image {

    struct resolution
    {
        static unsigned const side_max_size = 16000;

                 resolution  ()                                   ;
                 resolution  (unsigned w, unsigned h)             ;

        unsigned width       ()                             const ;
        unsigned height      ()                             const ;

        void     width       (unsigned)                           ;
        void     height      (unsigned)                           ;
        void     reset       ()                                   ;
        void     reset       (unsigned w, unsigned h)             ;

        unsigned pixel_count ()                             const ;
        unsigned frame_size  (unsigned bytes_per_pixel = 3) const ;
        bool     empty       ()                             const ;

    private:
        bool     check       ()                             const ;

        unsigned width_  ;
        unsigned height_ ;
    };

}}

namespace dawn  {
namespace image {

    inline resolution::resolution()
        : width_  (0)
        , height_ (0)
    {}
    inline resolution::resolution(unsigned w, unsigned h)
        : width_  (w)
        , height_ (h)
    {
        MSW_ASSERT(check());
    }

    inline unsigned resolution::width() const
    {
        return width_;
    }
    inline unsigned resolution::height() const
    {
        return height_;
    }

    inline void resolution::width(unsigned w)
    {
        width_ = w;
        MSW_ASSERT(check());
    }
    inline void resolution::height(unsigned h)
    {
        height_ = h;
        MSW_ASSERT(check());
    }
    inline void resolution::reset()
    {
        reset(0, 0);
    }
    inline void resolution::reset(unsigned w, unsigned h)
    {
        width (w);
        height(h);
    }

    inline unsigned resolution::pixel_count() const
    {
        return width_ * height_;
    }
    inline unsigned resolution::frame_size(unsigned bytes_per_pixel) const
    {
        return pixel_count() * bytes_per_pixel;
    }
    inline bool resolution::empty() const
    {
        return (width_ == 0) || (height_ == 0);
    }


    inline bool resolution::check() const
    {
        return (width_ <= side_max_size) && (height_ <= side_max_size);
    }

}}
