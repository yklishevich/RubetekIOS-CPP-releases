#pragma once

#include <msw/range.hpp>
#include <msw/throw_runtime_error.hpp>

#include <rubetek/module/tag_type.hpp>


namespace rubetek
{
    struct inter_module_command_unpacker
    {
        explicit inter_module_command_unpacker(msw::range<msw::byte const>);
        module::tag_type            tag     () const;
        msw::range<msw::byte const> payload () const;
    private:
        static module::tag_type eject_tag(msw::range<msw::byte const>& packet);

        module::tag_type            const tag_     ;
        msw::range<msw::byte const> const payload_ ;
    };
}


namespace rubetek
{
    inline inter_module_command_unpacker::inter_module_command_unpacker(msw::range<msw::byte const> packet)
        : tag_     ( eject_tag(packet)  )
        , payload_ ( packet             )
    {}
    inline module::tag_type inter_module_command_unpacker::tag() const
    {
        return tag_;
    }
    inline msw::range<msw::byte const> inter_module_command_unpacker::payload() const
    {
        return payload_;
    }

    inline module::tag_type inter_module_command_unpacker::eject_tag(msw::range<msw::byte const>& packet)
    {
        if (packet.size() < msw::size<msw::byte>(sizeof(module::tag_type))) msw::throw_runtime_error("unacceptably small packet size: ", packet.size());
        return packet.pop_front<module::tag_type>();
    }
}
