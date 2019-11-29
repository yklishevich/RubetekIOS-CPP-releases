#pragma once

#include <string>

#include <msw/config.hpp>
#include <msw/file/file.hpp>
#include <msw/buffer.hpp>
#include <msw/hard_ptr.hpp>
#include <msw/noncopyable.hpp>
#include <msw/system_error/throw.hpp>
#include <msw/throw_runtime_error.hpp>


namespace msw
{
    struct ifile
        : noncopyable
        , enable_hard_move
    {
        explicit    ifile           (std::string const& file_name)       ;
        ~           ifile           ()                                   ;

        range<byte> read            (range<byte>)                  const ;
        void        read_imperative (range<byte>)                  const ;
        void        read_imperative (range<byte>, bool& success)   const ;

        range<byte> peek            (range<byte>)                  const ;
        void        peek_imperative (range<byte>)                  const ;
        void        peek_imperative (range<byte>, bool& success)   const ;

        uint64      size            ()                             const ;
        uint64      position        ()                             const ;

    private:
        file::handle h_;
    };
}


namespace msw
{
    buffer<byte> read_file(std::string const& file_name, msw::size<msw::byte> size);
    buffer<byte> read_file(std::string const& file_name);
}


namespace msw
{
    inline ifile::ifile(std::string const& file_name)
        : h_(file::open_read(file_name.c_str()))
    {
        if (!h_) system_error::throw_exception("can't open file '" + file_name + "'");
    }
    inline ifile::~ifile()
    {
        file::close(h_);
    }

    inline range<byte> ifile::read(range<byte> r) const
    {
        unsigned long readed_bytes = 0;
        if (!file::read(h_, r.data().native(), static_cast<unsigned>(r.size().count()), readed_bytes))
            system_error::throw_exception("can't read from file");
        return r.crop_front(readed_bytes);
    }
    inline void ifile::read_imperative(range<byte> r) const
    {
        range<byte> block = read(r);
        if (block.size() != r.size())
            throw_runtime_error("can't read imperative block: ", r.size(), " B; readed only: ", block.size(), " B");
    }
    inline void ifile::read_imperative(range<byte> r, bool& success) const
    {
        range<byte> block = read(r);
        success = block.size() == r.size();
    }

    inline range<byte> ifile::peek(range<byte> r) const
    {
        unsigned long readed_bytes = 0;
        if (!file::peek(h_, r.data().native(), static_cast<unsigned>(r.size().count()), readed_bytes))
            system_error::throw_exception("can't peek from file");
        return r.crop_front(readed_bytes);
    }
    inline void ifile::peek_imperative(range<byte> r) const
    {
        range<byte> block = peek(r);
        if (block.size() != r.size())
            throw_runtime_error("can't peek imperative block: ", r.size(), " B; readed only: ", block.size(), " B");
    }
    inline void ifile::peek_imperative(range<byte> r, bool& success) const
    {
        range<byte> block = peek(r);
        success = block.size() == r.size();
    }

    inline uint64 ifile::size() const
    {
        return file::size(h_);
    }
    inline uint64 ifile::position() const
    {
        long long pos = file::seek_from_current(h_, 0);
        if (pos < 0) system_error::throw_exception("can't get file position");
        return pos;
    }
}


namespace msw
{
    inline buffer<byte> read_file(std::string const& file_name, msw::size<msw::byte> size)
    {
        ifile ifile_(file_name);
        msw::size<msw::byte> const file_size(static_cast<msw::uint>(ifile_.size()));
        buffer<byte> buf(file_size.min(size));
#ifdef MSW_DEBUG
        range<byte> const readed =
#endif
            ifile_.read(buf.all());
        MSW_ASSERT(readed.size() == buf.size());
        return std::move(buf);
    }
    inline buffer<byte> read_file(std::string const& file_name)
    {
        ifile ifile_(file_name);
        buffer<byte> buf(static_cast<msw::uint>(ifile_.size()));
#ifdef MSW_DEBUG
        range<byte> const readed =
#endif
            ifile_.read(buf.all());
        MSW_ASSERT(readed.size() == buf.size());
        return std::move(buf);
    }
}
