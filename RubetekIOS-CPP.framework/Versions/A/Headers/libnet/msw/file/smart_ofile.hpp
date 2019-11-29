#pragma once

#include <msw/std/memory.hpp>
#include <msw/file/ips/writer.hpp>
#include <msw/file/pcap/writer.hpp>
#include <msw/file/detect_format.hpp>


namespace msw
{
    namespace zzz
    {
        struct smart_ofile_base;
    }
    struct smart_ofile
    {
        explicit     smart_ofile (std::string const& filename)                     ;
                     smart_ofile (std::string const& filename, file::format)       ;
        void         write       (range<byte const>)                               ;
        file::format format      ()                                          const ;
        uint64       size        ()                                          const ;
        void         flush       ()                                                ;
    private:
        file::format                           const fmt_   ;
        std::unique_ptr<zzz::smart_ofile_base>       ofile_ ;
    };
}

namespace msw {
namespace zzz {

    struct smart_ofile_base
    {
        virtual void   write            (range<byte const>)       = 0 ;
        virtual uint64 size             ()                  const = 0 ;
        virtual void   flush            () {}
        virtual ~      smart_ofile_base () {}
    };
    struct smart_ofile_bin
        : smart_ofile_base
        , optimum_ofile2
    {
        explicit smart_ofile_bin (std::string const& filename)
            : optimum_ofile2 (filename)
        {}
        void write(range<byte const> block) final
        {
            optimum_ofile2::write(block);
        }
        uint64 size() const final
        {
            return optimum_ofile2::size();
        }
        void flush() final
        {
            optimum_ofile2::flush();
        }
    };
    struct smart_ofile_ips
        : smart_ofile_base
        , ips::writer
    {
        explicit smart_ofile_ips (std::string const& filename)
            : ips::writer(filename)
        {}
        void write(range<byte const> block) final
        {
            write_block(block);
        }
        uint64 size() const final
        {
            return ips::writer::size();
        }
        void flush() final
        {
            ips::writer::flush();
        }
    };
    struct smart_ofile_pcap
        : smart_ofile_base
        , pcap_file::writer
    {
        explicit smart_ofile_pcap (std::string const& filename)
            : pcap_file::writer(filename)
        {}
        void write(range<byte const> block) final
        {
            write_packet(block);
        }
        uint64 size() const final
        {
            return pcap_file::writer::size();
        }
    };

}}


namespace msw
{
    inline smart_ofile::smart_ofile(std::string const& filename)
        : smart_ofile(filename, file::detect_format_by_ext(filename))
    {}
    inline smart_ofile::smart_ofile(std::string const& filename, file::format fmt)
        : fmt_(fmt)
    {
        if      (fmt_ == file::format::ips ) ofile_ = msw::make_unique_inherited<zzz::smart_ofile_base, zzz::smart_ofile_ips >(filename);
        else if (fmt_ == file::format::pcap) ofile_ = msw::make_unique_inherited<zzz::smart_ofile_base, zzz::smart_ofile_pcap>(filename);
        else                                ofile_ = msw::make_unique_inherited<zzz::smart_ofile_base, zzz::smart_ofile_bin >(filename);
    }
    inline void smart_ofile::write(range<byte const> r)
    {
        ofile_->write(r);
    }
    inline file::format smart_ofile::format() const
    {
        return fmt_;
    }
    inline uint64 smart_ofile::size() const
    {
        return ofile_->size();
    }
    inline void smart_ofile::flush()
    {
        ofile_->flush();
    }
}
