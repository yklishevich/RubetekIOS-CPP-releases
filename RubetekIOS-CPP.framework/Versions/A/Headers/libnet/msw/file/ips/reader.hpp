#pragma once

#include <msw/size.hpp>
#include <msw/assert.hpp>
#include <msw/plain_types.hpp>        // byte
#include <msw/noncopyable.hpp>
#include <msw/algorithm/equal.hpp>
#include <msw/file/optimum_ifile.hpp>
#include <msw/file/ips/signature.hpp>


namespace msw {
namespace ips {

    struct reader
        : noncopyable
    {
        explicit    reader           (std::string const& file_name)       ;

        bool        has_blocks       ()                             const ;
        size<byte>  block_size       ()                             const ;
        range<byte> read_block       ()                                   ;

        uint64      readed_raw_bytes ()                             const ;

    private:
        void read_block_size();

        optimum_ifile ifile_      ;
        bool          has_blocks_ ;
        size<byte>    block_size_ ;
    };

}}


namespace msw {
namespace ips {

    template <typename Handler>
    void read(std::string const& file_name, Handler);

}}


namespace msw {
namespace ips {

    inline reader::reader(std::string const& file_name)
        : ifile_(file_name)
    {
        if (!equal(ifile_.read(signature().size()), signature())) throw std::runtime_error("bad header ip stream");
        read_block_size();
    }

    inline bool reader::has_blocks() const
    {
        return has_blocks_;
    }
    inline size<byte> reader::block_size() const
    {
        MSW_ASSERT(has_blocks());
        return block_size_;
    }
    inline range<byte> reader::read_block()
    {
        MSW_ASSERT(has_blocks());
        range<byte> block = ifile_.read(block_size());
        if (block.size() != block_size()) throw std::runtime_error("error read of block");
        read_block_size();
        return block;
    }

    inline uint64 reader::readed_raw_bytes() const
    {
        return ifile_.position();
    }

    inline void reader::read_block_size()
    {
        range<byte> field_size = ifile_.read(size<byte>(4));
        has_blocks_ = (field_size.size() == size<byte>(4));
        if (has_blocks_) block_size_ = field_size.as<uint32>();
        else block_size_.clear();
    }

}}


namespace msw {
namespace ips {

    template <typename Handler>
    void read(std::string const& file_name, Handler h)
    {
        reader reader_(file_name);
        while (reader_.has_blocks()) h(reader_.read_block());
    }

}}
