#pragma once

#include <string>
#include <functional>

#include <msw/range.hpp>
#include <msw/noncopyable.hpp>
#include <msw/callback/event.hpp>
#include <msw/proto/spp/packer.hpp>
#include <msw/proto/spp/unpacker.hpp>
#include <msw/callback/byte_range.hpp>
#include <msw/lexical_cast/bool2str.hpp>
#include <msw/lexical_cast/bytes_as_string.hpp>

#include <rubetek/log.hpp>
#include <rubetek/essence/unique_id/client.hpp>
#include <rubetek/essence/unique_id/controller.hpp>
#include <rubetek/local_link/set.hpp>
#include <rubetek/local_link/session_interface.hpp>


#ifdef RUBETEK_CLIENT_LOCAL_SESSION_LOG
#   define RUBETEK_CLIENT_LOCAL_SESSION_ON_LOG(...) logger_.debug(__VA_ARGS__);
#else
#   define RUBETEK_CLIENT_LOCAL_SESSION_ON_LOG(...)
#endif


namespace rubetek {
namespace client  {

    struct local_controller_session
        : local_link_session_interface
    {
        typedef std::function<void(msw::range<msw::byte const>)> fn_send             ;
        typedef msw::callback::event                             fn_close            ;
        typedef local_link_set<unique_controller_id::type>       local_link_set_type ;

                    local_controller_session    (
                                                    fn_send
                                                    , fn_close
                                                    , std::string const& name
                                                    , unique_client_id::const_reference
                                                    , std::string const& ip_address
                                                    , local_link_set_type&
                                                    , bool novel
                                                    , bool novel32
                                                )                             ;
                    ~local_controller_session   ()                            ;
        void        on_packet_received          (msw::range<msw::byte const>) ;

    private:

        void        process_incoming_packet     (msw::range<msw::byte const>)       ;
        void        send                        (msw::range<msw::byte const>)       ;
        std::string ip_address                  ()                            const ;

        logger                const logger_           ;
        std::string           const ip_address_       ;
        bool                  const novel_            ;
        bool                  const novel32_          ;
        bool                        received_version_ ;
        fn_send                     send_             ;
        fn_close                    close_            ;
        local_link_set_type&        local_link_set_   ;
        on_received                 on_received_      ;
        msw::spp::packer_16         spp_packer_       ;
        msw::spp::unpacker_16       spp_unpacker_     ;
        msw::spp::packer_32         spp_packer32_     ;
        msw::spp::unpacker_32       spp_unpacker32_   ;
    };

}}


namespace rubetek {
namespace client  {

    inline local_controller_session::local_controller_session
    (
        fn_send                             fsend
        , fn_close                          close
        , std::string const&                name
        , unique_client_id::const_reference client_id
        , std::string const&                ip_address
        , local_link_set_type&              local_link_set
        , bool                              novel
        , bool                              novel32
    )
        : logger_           (
                                 "local controller session"
                                 , name
#ifdef RUBETEK_CLIENT_LOCAL_SESSION_LOG
                                 , log::level::debug
#endif
                            )
        , ip_address_       ( ip_address     )
        , novel_            ( novel          )
        , novel32_          ( novel32        )
        , received_version_ ( false          )
        , send_             ( fsend          )
        , close_            ( close          )
        , local_link_set_   ( local_link_set )
        , spp_packer_
        (
            [this](msw::range<msw::byte const> packet)
            {
                send_(packet);
            }
        )
        , spp_unpacker_
        (
            [this](msw::range<msw::byte const> packet)
            {
                process_incoming_packet(packet);
            }
        )
        , spp_packer32_
        (
            [this](msw::range<msw::byte const> packet)
            {
                send_(packet);
            }
        )
        , spp_unpacker32_
        (
            [this](msw::range<msw::byte const> packet)
            {
                process_incoming_packet(packet);
            }
        )
    {
        MSW_ASSERT(!(novel_ && novel32_));
        RUBETEK_CLIENT_LOCAL_SESSION_ON_LOG( "novel proto: "    , msw::yes_no(novel_   ) );
        RUBETEK_CLIENT_LOCAL_SESSION_ON_LOG( "novel-32 proto: " , msw::yes_no(novel32_ ) );
        msw::wbyte const version = 0;
        if (novel_ || novel32_) send(msw::make_range<msw::byte>(version));
        send(msw::make_range<msw::byte>(client_id));
    }
    inline local_controller_session::~local_controller_session()
    {
        if (on_received_) local_link_set_.remove_link(this);
    }

    inline void local_controller_session::on_packet_received(msw::range<msw::byte const> packet)
    {
        RUBETEK_CLIENT_LOCAL_SESSION_ON_LOG("on packet received");
        if (novel_)        spp_unpacker_.   unpack(packet);
        else if (novel32_) spp_unpacker32_. unpack(packet);
        else               process_incoming_packet(packet);
    }

    inline void local_controller_session::process_incoming_packet(msw::range<msw::byte const> packet)
    {
        RUBETEK_CLIENT_LOCAL_SESSION_ON_LOG("process incoming packet");
        if (on_received_) on_received_(packet);
        else
        {
            if ((novel_ || novel32_) && !received_version_)
            {
                received_version_ = true;
                RUBETEK_CLIENT_LOCAL_SESSION_ON_LOG("version received: ", packet);
            }
            else on_received_ = local_link_set_.add_link(this, msw::bytes_as_string(packet));
        }
    }
    inline void local_controller_session::send(msw::range<msw::byte const> packet)
    {
        if      (novel_)   spp_packer_  .add_packet(packet);
        else if (novel32_) spp_packer32_.add_packet(packet);
        else               send_(packet);
    }
    inline std::string local_controller_session::ip_address() const
    {
        return ip_address_;
    }

}}
