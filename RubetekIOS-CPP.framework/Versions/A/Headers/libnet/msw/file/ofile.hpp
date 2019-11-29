#pragma once

#include <string>

#include <msw/file/file.hpp>
#include <msw/range.hpp>
#include <msw/hard_ptr.hpp>
#include <msw/noncopyable.hpp>
#include <msw/system_error/throw.hpp>


namespace msw
{
    struct ofile
        : noncopyable
        , enable_hard_move
    {
        explicit ofile (std::string const& file_name, bool append = false)       ;
        ~        ofile ()                                                        ;

        void     write (range<byte const>)                                 const ;

        uint64   size  ()                                                  const ;

    private:
        file::handle h_;
    };
}


namespace msw
{
    void write_to_file(std::string const& file_name, range<byte const>);
}


namespace msw
{
    inline ofile::ofile(std::string const& file_name, bool append)
        : h_(file::open_write(file_name.c_str(), append))
    {
        if (!h_) system_error::throw_exception("can't open file '" + file_name + "'");
        if (append) file::seek_from_end(h_, 0);
    }
    inline ofile::~ofile()
    {
        file::close(h_);
    }

    inline void ofile::write(range<byte const> r) const
    {
        if (!file::write(h_, r.data().native(), static_cast<unsigned>(r.size().count())))
            system_error::throw_exception("can't write to file");
    }

    inline uint64 ofile::size() const
    {
        return file::size(h_);
    }
}


namespace msw
{
    inline void write_to_file(std::string const& file_name, range<byte const> r)
    {
        ofile ofile_(file_name);
        ofile_.write(r);
    }
}
