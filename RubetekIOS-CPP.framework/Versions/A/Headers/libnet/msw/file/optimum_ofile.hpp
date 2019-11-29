#pragma once

#include <msw/buffer.hpp>
#include <msw/file/ofile.hpp>
#include <msw/noncopyable.hpp>


namespace msw
{
    struct optimum_ofile
    {
        explicit optimum_ofile (std::string const& filename, msw::size<byte> block_size = MB)       ;
        ~        optimum_ofile ()                                                                   ;

        void     write         (range<byte const>)                                                  ;

        uint64   size          ()                                                             const ;

        void     flush         ()                                                                   ;

    private:

        ofile        ofile_ ;
        buffer<byte> buf_   ;
    };
}


namespace msw
{
    inline optimum_ofile::optimum_ofile(std::string const& filename, msw::size<byte> block_size)
        : ofile_ ( filename      )
        , buf_   ( 0, block_size )
    {}
    inline optimum_ofile::~optimum_ofile()
    {
        flush();
    }

    inline void optimum_ofile::write(range<byte const> r)
    {
        if (buf_.empty())
        {
            if (r.size() >= buf_.capacity()) ofile_.write(r);
            else buf_.push_back_silent(r);
        }
        else
        {
            if (buf_.free_space() > r.size()) buf_.push_back_silent(r);
            else
            {
                buf_.push_back_silent(r.pop_front(buf_.free_space()));
                flush();
                write(r);
            }
        }
    }

    inline uint64 optimum_ofile::size() const
    {
        return ofile_.size();
    }

    inline void optimum_ofile::flush()
    {
        ofile_.write(buf_.all());
        buf_.clear();
    }
}
