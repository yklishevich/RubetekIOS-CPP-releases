#pragma once

#include <msw/plain_types.hpp>           // byte
#include <msw/noncopyable.hpp>
#include <msw/file/ips/signature.hpp>
#include <msw/file/optimum_ofile2.hpp>
#include <msw/make_range/from_plain.hpp>


namespace msw {
namespace ips {

    struct writer
        : noncopyable
    {
        explicit writer      (std::string const& file_name)       ;
        void     write_block (range<byte const>)                  ;
        uint64   size        ()                             const ;
        void     flush       ()                                   ;
    private:
        optimum_ofile2 ofile_;
    };

}}


namespace msw {
namespace ips {

    inline writer::writer(std::string const& file_name)
        : ofile_(file_name)
    {
        ofile_.write(signature());
    }
    inline void writer::write_block(range<byte const> block)
    {
        ofile_.write(make_range<byte>(static_cast<uint32>(block.size().count())));
        ofile_.write(block);
    }
    inline uint64 writer::size() const
    {
        return ofile_.size();
    }
    inline void writer::flush()
    {
        ofile_.flush();
    }

}}
