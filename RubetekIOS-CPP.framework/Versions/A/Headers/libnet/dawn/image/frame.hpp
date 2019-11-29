#pragma once

#include <msw/buffer.hpp>
#include <msw/algorithm.hpp>

#include <dawn/image/resolution.hpp>


namespace dawn  {
namespace image {

    struct frame
    {
        typedef msw::range<msw::byte>       data_type       ;
        typedef msw::range<msw::byte const> const_data_type ;
        typedef msw::size<msw::byte>        size_type       ;

        frame (                                         unsigned bytes_per_pxl = 3) ;
        frame (resolution,                              unsigned bytes_per_pxl = 3) ;
        frame (unsigned w, unsigned h,                  unsigned bytes_per_pxl = 3) ;
        frame (const_data_type, resolution,             unsigned bytes_per_pxl = 3) ;
        frame (const_data_type, unsigned w, unsigned h, unsigned bytes_per_pxl = 3) ;

        data_type       data           ()       ;
        const_data_type data           () const ;
        size_type       size           () const ;
        bool            empty          () const ;
        void            clear          ()       ;

        unsigned                    w              () const ;
        unsigned                    h              () const ;
        resolution                  get_resolution () const ;
        unsigned                    bytes_per_pxel () const ;

        void                        w              (unsigned              ) ;
        void                        h              (unsigned              ) ;
        void                        set_resolution (resolution            ) ;
        void                        set_resolution (unsigned w, unsigned h) ;

        void                        set_frame      (const_data_type, resolution            ) ;
        void                        set_frame      (const_data_type, unsigned w, unsigned h) ;

    private:
        unsigned               const bytes_per_pixel_ ;
        msw::buffer<msw::byte>       frame_           ;
        resolution                   resolution_      ;
    };

}}


namespace dawn  {
namespace image {

    inline frame::frame(unsigned bytes_per_pxl)
        : bytes_per_pixel_(bytes_per_pxl)
    {}
    inline frame::frame(resolution res, unsigned bytes_per_pxl)
        : bytes_per_pixel_(bytes_per_pxl)
    {
        set_resolution(res);
    }
    inline frame::frame(unsigned width, unsigned height, unsigned bytes_per_pxl)
        : frame(resolution(width, height), bytes_per_pxl)
    {}
    inline frame::frame(const_data_type data, resolution res, unsigned bytes_per_pxl)
        : bytes_per_pixel_(bytes_per_pxl)
    {
        set_frame(data, res);
    }
    inline frame::frame(const_data_type data, unsigned width, unsigned height, unsigned bytes_per_pxl)
        : frame(data, resolution(width, height), bytes_per_pxl)
    {}

    inline msw::range<msw::byte> frame::data()
    {
        return frame_.all();
    }
    inline msw::range<msw::byte const> frame::data() const
    {
        return frame_.all();
    }
    inline msw::size<msw::byte> frame::size() const
    {
        return frame_.size();
    }
    inline bool frame::empty() const
    {
        return frame_.empty();
    }
    inline void frame::clear()
    {
        frame_.clear();
        resolution_.reset();
    }

    inline unsigned frame::w() const
    {
        return resolution_.width();
    }
    inline unsigned frame::h() const
    {
        return resolution_.height();
    }
    inline resolution frame::get_resolution() const
    {
        return resolution_;
    }
    inline unsigned frame::bytes_per_pxel() const
    {
        return bytes_per_pixel_;
    }

    inline void frame::w(unsigned width)
    {
        set_resolution(width, h());
    }
    inline void frame::h(unsigned height)
    {
        set_resolution(w(), height);
    }
    inline void frame::set_resolution(resolution res)
    {
        resolution_ = res;
        frame_.resize(res.frame_size());
    }
    inline void frame::set_resolution(unsigned width, unsigned height)
    {
        set_resolution(resolution(width, height));
    }

    inline void frame::set_frame(const_data_type data, resolution res)
    {
        set_resolution(res);
        MSW_ASSERT(data.size().count() == resolution_.frame_size());
        msw::copy(data, frame_);
    }
    inline void frame::set_frame(const_data_type data, unsigned w, unsigned h)
    {
        set_frame(data, resolution(w, h));
    }

}}
