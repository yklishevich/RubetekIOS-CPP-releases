#pragma once

#include <list>
#include <string>
#include <algorithm>
#include <functional>

#include <msw/range.hpp>
#include <msw/assert.hpp>
#include <msw/noncopyable.hpp>

#include <rubetek/log.hpp>
#include <rubetek/local_link/interface.hpp>
#include <rubetek/local_link/session_interface.hpp>


namespace rubetek
{
    struct local_link_selector
        : local_link_interface
    {
                    local_link_selector   ()                                    ;
                    local_link_selector   (local_link_selector&&)               ;

        on_received add_link              (local_link_session_interface*)       ;
        void        remove_link           (local_link_session_interface*)       ;
        bool        remove_link_if_has    (local_link_session_interface*)       ;

        bool        has_link              (local_link_session_interface*) const ;
        bool        has_links             ()                              const ;

        std::string ip_address            ()                              const ;

        void        set_on_received       (on_received)                         ;
        void        send                  (msw::range<msw::byte const>)         ;

    private:
        typedef std::list<local_link_session_interface*> links;

        void        received_from_session (msw::range<msw::byte const>)         ;

        logger      const logger_      ;
        links             links_       ;
        on_received       on_received_ ;
    };
}


namespace rubetek
{
    inline local_link_selector::local_link_selector()
        : logger_("local link selector", this)
    {}
    inline local_link_selector::local_link_selector(local_link_selector&& other)
        : logger_      ( "local link selector", this   )
        , links_       ( std::move(other.links_)       )
        , on_received_ ( std::move(other.on_received_) )
    {}

    inline local_link_selector::on_received local_link_selector::add_link(local_link_session_interface* session)
    {
        logger_.debug("add link: ", session);
        links_.push_back(session);
        return std::bind(&local_link_selector::received_from_session, this, std::placeholders::_1);
    }
    inline void local_link_selector::remove_link(local_link_session_interface* session)
    {
        logger_.debug("remove link: ", session);
        links_.remove(session);
    }
    inline bool local_link_selector::remove_link_if_has(local_link_session_interface* session)
    {
        logger_.debug("remove link if has: ", session);
        auto it = std::find(links_.begin(), links_.end(), session);
        if (it != links_.end())
        {
            logger_.debug("remove it");
            links_.erase(it);
            return true;
        }
        return false;
    }

    inline bool local_link_selector::has_link(local_link_session_interface* session) const
    {
        return std::find(links_.begin(), links_.end(), session) != links_.end();
    }
    inline bool local_link_selector::has_links() const
    {
        return !links_.empty();
    }

    inline std::string local_link_selector::ip_address() const
    {
        if (!has_links()) throw std::runtime_error("get ip address fail. no active links");
        return links_.front()->ip_address();
    }

    inline void local_link_selector::set_on_received(on_received on_received)
    {
        logger_.debug("set on received");
        on_received_ = on_received;
    }
    inline void local_link_selector::send(msw::range<msw::byte const> packet)
    {
        if (!has_links()) throw std::runtime_error("send fail. no active links");
        links_.front()->send(packet);
    }

    inline void local_link_selector::received_from_session(msw::range<msw::byte const> packet)
    {
        MSW_ASSERT(on_received_);
        if (on_received_) (on_received(on_received_))(packet);
    }
}
