#pragma once

#include <msw/buffer.hpp>

#include <rubetek/module/tag_type.hpp>


namespace rubetek
{
    struct inter_module_command_packer
    {
        typedef msw::range<msw::byte const> packet_type;
                                             packet_type pack(module::tag_type)                     ;
        template <typename T>                packet_type pack(module::tag_type, T&&)                ;
        template <typename T, typename Size> packet_type pack(module::tag_type, T const* p, Size s) ;
    private:
        msw::buffer<msw::byte> buf_;
    };
}


namespace rubetek
{
    inline inter_module_command_packer::packet_type inter_module_command_packer::pack(module::tag_type tag)
    {
        return pack(tag, msw::range<msw::byte>());
    }
    template <typename T>
    inter_module_command_packer::packet_type inter_module_command_packer::pack(module::tag_type tag, T&& v)
    {
        buf_.clear();
        buf_.push_back(tag);
        buf_.push_back(std::forward<T>(v));
        return buf_.all();
    }
    template <typename T, typename Size>
    inter_module_command_packer::packet_type inter_module_command_packer::pack(module::tag_type tag, T const* p, Size s)
    {
        return pack(tag, msw::make_range_native(p, s));
    }
}
