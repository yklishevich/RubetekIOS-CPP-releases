#pragma once

#include <list>
#include <memory>
#include <cassert>
#include <unordered_map>

#include <msw/range.hpp>
#include <msw/buffer.hpp>
#include <msw/nonassignable.hpp>
#include <msw/make_range_native.hpp>
#include <msw/throw_runtime_error.hpp>

#include <rubetek/module/tag_type.hpp>
#include <rubetek/module/tag_ready.hpp>
#include <rubetek/module/interface.hpp>
#include <rubetek/inter_module_command/packer.hpp>


namespace rubetek {
namespace module  {

    struct context2
        : msw::nonassignable
    {
        typedef std::shared_ptr<context2> ptr;

        void receive_all          (tag_ready = nullptr)                      ;
        void receive_all_ex       (tag_ready_ex = nullptr)                   ;
        void subscribe            (tag_type            , tag_ready_short)    ;
        void subscribe            (std::list<tag_type> , tag_ready)          ;
        void subscribe_s          (tag_type            , tag_ready_s_short)  ;
        void subscribe_s          (std::list<tag_type> , tag_ready_s)        ;
        void subscribe_ex         (tag_type            , tag_ready_ex_short) ;
        void subscribe_ex         (std::list<tag_type> , tag_ready_ex)       ;
        void unsubscribe          (tag_type)                                 ;
        void unsubscribe          (std::list<tag_type>)                      ;
        void unsubscribe_from_all ()                                         ;

                                             void send(tag_type)                     ;
        template <typename T>                void send(tag_type, T&&)                ;
        template <typename T, typename Size> void send(tag_type, T const* p, Size s) ;

        static  ptr  make(module_controller_link) ;
                void free()                       ;

    private:

        explicit context2(module_controller_link);

        module_controller_link get_link() const;

        template <typename Module>
        friend struct module_shell;

        void process_packet(byte_const_range);
        void process_packet(tag_type, byte_const_range);

        typedef std::unordered_map<tag_type, tag_ready> tag_map_type;

        module_controller_link      link_        ;
        tag_ready                   receive_all_ ;
        tag_map_type                tag_map_     ;
        inter_module_command_packer imc_packer_  ;
    };

}}


namespace rubetek {
namespace module  {

    inline void context2::receive_all(tag_ready ready)
    {
        receive_all_ = ready;
    }
    inline void context2::receive_all_ex(tag_ready_ex ready)
    {
        if (ready) receive_all
        (
            [ready](tag_type tag, byte_const_range packet)
            {
                ready(tag, packet.data().native(), packet.size().count());
            }
        );
        else receive_all_ = nullptr;
    }
    inline void context2::subscribe(tag_type tag, tag_ready_short fn)
    {
        tag_map_[tag] = std::bind(fn, std::placeholders::_2);
    }
    inline void context2::subscribe(std::list<tag_type> tags, tag_ready fn)
    {
        for (tag_type tag : tags)
            tag_map_[tag] = fn;
    }
    inline void context2::subscribe_s(tag_type tag, tag_ready_s_short fn)
    {
        subscribe
        (
            tag
            , [fn](byte_const_range packet)
            {
                fn(msw::bytes_as_string(packet));
            }
        );
    }
    inline void context2::subscribe_s(std::list<tag_type> tags, tag_ready_s fn)
    {
        subscribe
        (
            tags
            , [fn](tag_type tag, byte_const_range packet)
            {
                fn(tag, msw::bytes_as_string(packet));
            }
        );
    }
    inline void context2::subscribe_ex(tag_type tag, tag_ready_ex_short fn)
    {
        subscribe
        (
            tag
            , [fn](byte_const_range packet)
            {
                fn(packet.data().native(), packet.size().count());
            }
        );
    }
    inline void context2::subscribe_ex(std::list<tag_type> tags, tag_ready_ex fn)
    {
        subscribe
        (
            tags
            , [fn](tag_type tag, byte_const_range packet)
            {
                fn(tag, packet.data().native(), packet.size().count());
            }
        );
    }
    inline void context2::unsubscribe(tag_type tag)
    {
        auto i = tag_map_.find(tag);
        if (i == tag_map_.end()) msw::throw_runtime_error("can't unsubscribe tag ", tag, ", it could not find");
    }
    inline void context2::unsubscribe(std::list<tag_type> tags)
    {
        for (tag_type tag : tags)
            unsubscribe(tag);
    }
    inline void context2::unsubscribe_from_all()
    {
        tag_map_.clear();
    }

    inline void context2::send(tag_type tag)
    {
        send(tag, msw::range<msw::byte>());
    }
    template <typename T>
    void context2::send(tag_type tag, T&& v)
    {
        auto const block = imc_packer_.pack(tag, std::forward<T>(v));
        utility::raw_memory mem(block.data().native(), block.size().count());
        get_link()->send_inter_module_packet(mem);
    }
    template <typename T, typename Size>
    void context2::send(tag_type tag, T const* p, Size s)
    {
        send(tag, msw::make_range_native(p, s));
    }


    inline context2::context2(module_controller_link link)
        : link_(link)
    {
        assert(link_);
    }

    inline module_controller_link context2::get_link() const
    {
        assert(link_);
        return link_;
    }

    inline void context2::process_packet(byte_const_range pkt)
    {
        if (pkt.size() < msw::size<tag_type>(1)) msw::throw_runtime_error("unknown tag, small packet size: ", pkt.size(), " B");
        tag_type const tag = pkt.pop_front<tag_type>();
        process_packet(tag, pkt);
    }
    inline void context2::process_packet(tag_type tag, byte_const_range packet)
    {
        if (receive_all_) receive_all_(tag, packet);
        auto i = tag_map_.find(tag);
        if (i != tag_map_.end()) i->second(tag, packet);
    }

    inline context2::ptr context2::make(module_controller_link link)
    {
        return ptr(new context2(link));
    }
    inline void context2::free()
    {
        link_ = nullptr;
    }

}}
