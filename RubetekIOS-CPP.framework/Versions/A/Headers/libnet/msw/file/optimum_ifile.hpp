#pragma once

#include <msw/buffer.hpp>
#include <msw/file/ifile.hpp>
#include <msw/noncopyable.hpp>
#include <msw/throw_runtime_error.hpp>


#define MSW_IFILE_DEFAULT_BLOCK_SIZE msw::MB

namespace msw
{
    struct optimum_ifile
        : noncopyable
    {
        explicit    optimum_ifile   (
                                          std::string const& filename
                                        , size<byte> = MSW_IFILE_DEFAULT_BLOCK_SIZE
                                    )                                 ;

        range<byte> read            (size<byte>)                      ;
        range<byte> read_imperative (size<byte>)                      ;
        range<byte> read_imperative (size<byte>, bool& success)       ;

        range<byte> peek            (size<byte>)                      ;
        range<byte> peek_imperative (size<byte>)                      ;
        range<byte> peek_imperative (size<byte>, bool& success)       ;

        uint64      size            ()                          const ;
        uint64      position        ()                          const ;

    private:

        msw::size<byte> block_size_ ;
        ifile           ifile_      ;
        buffer<byte>    buf_        ;
        range<byte>     transit_    ;
    };
}


namespace msw
{
    template <typename Handler>
    void read_file(std::string const& file_name, Handler, size<byte> = MSW_IFILE_DEFAULT_BLOCK_SIZE);
}


namespace msw
{
    inline optimum_ifile::optimum_ifile(std::string const& filename, msw::size<byte> block_size)
        : block_size_ ( block_size         )
        , ifile_      ( filename           )
        , buf_        ( 0, block_size_ * 2 )
        , transit_    ( buf_.all()         )
    {}

    inline range<byte> optimum_ifile::read(msw::size<byte> size)
    {
        if (transit_.size() >= size) return transit_.pop_front(size);
        msw::size<byte> const block_size = (block_size_.max)(size - transit_.size());
        buf_.assign_silent(transit_);
        buf_.increase_size(block_size);
        transit_ = buf_.begin() + ifile_.read(buf_.all().back(block_size)).end();
        return transit_.pop_front_max(size);
    }
    inline range<byte> optimum_ifile::read_imperative(msw::size<byte> size)
    {
        bool success;
        range<byte> block = read_imperative(size, success);
        if (!success)
            throw_runtime_error("can't read imperative block: ", size, " B; readed only: ", block.size(), " B");
        return block;
    }
    inline range<byte> optimum_ifile::read_imperative(msw::size<byte> size, bool& success)
    {
        range<byte> block = read(size);
        success = block.size() == size;
        return block;
    }

    inline range<byte> optimum_ifile::peek(msw::size<byte> size)
    {
        if (transit_.size() >= size) return transit_.front(size);
        msw::size<byte> const block_size = (block_size_.max)(size - transit_.size());
        buf_.assign_silent(transit_);
        buf_.increase_size(block_size);
        transit_ = buf_.begin() + ifile_.read(buf_.all().back(block_size)).end();
        return transit_.front_max(size);
    }
    inline range<byte> optimum_ifile::peek_imperative(msw::size<byte> size)
    {
        bool success;
        range<byte> block = peek_imperative(size, success);
        if (!success)
            throw_runtime_error("can't peek imperative block: ", size, " B; readed only: ", block.size(), " B");
        return block;
    }
    inline range<byte> optimum_ifile::peek_imperative(msw::size<byte> size, bool& success)
    {
        range<byte> block = peek(size);
        success = block.size() == size;
        return block;
    }

    inline uint64 optimum_ifile::size() const
    {
        return ifile_.size();
    }
    inline uint64 optimum_ifile::position() const
    {
        return ifile_.position();
    }
}


namespace msw
{
    template <typename Handler>
    void read_file(std::string const& file_name, Handler h, size<byte> block_size)
    {
        optimum_ifile ifile_(file_name);
        for (;;)
        {
            range<byte> r = ifile_.read(block_size);
            if (r.has_items()) h(r);
            else break;
        }
    }
}
