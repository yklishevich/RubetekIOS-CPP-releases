#pragma once

#include <functional>

#include <msw/plain_types.hpp>           // byte
#include <msw/noncopyable.hpp>
#include <msw/file/ips/signature.hpp>
#include <msw/make_range/from_plain.hpp>


namespace msw {
namespace ips {

    struct packer
        : noncopyable
    {
        typedef std::function<void(range<byte const>)> data_ready;

        explicit    packer      (data_ready)        ;
        void        pack_block  (range<byte const>) ;
    private:
        data_ready data_ready_;
    };

}}


namespace msw {
namespace ips {

    inline packer::packer(data_ready data_ready)
        : data_ready_(data_ready)
    {
        data_ready_(signature());
    }

    inline void packer::pack_block(range<byte const> block)
    {
        data_ready_(make_range<byte>(static_cast<uint32>(block.size().count())));
        data_ready_(block);
    }

}}
