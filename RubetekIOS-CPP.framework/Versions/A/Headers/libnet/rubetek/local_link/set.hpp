#pragma once

#include <algorithm>
#include <functional>
#include <unordered_map>

#include <msw/ptr.hpp>
#include <msw/assert.hpp>
#include <msw/noncopyable.hpp>

#include <rubetek/log.hpp>
#include <rubetek/local_link/session_interface.hpp>
#include <rubetek/local_link/selector.hpp>


namespace rubetek
{
    template <typename UniqueId>
    struct local_link_set
        : msw::noncopyable
    {
        typedef std::function<void(UniqueId, local_link_selector*)> on_add    ;
        typedef std::function<void(UniqueId)>                       on_remove ;

                                      local_link_set  (on_add, on_remove)                              ;
                                      ~local_link_set ()                                               ;

    local_link_interface::on_received add_link        (local_link_session_interface*, UniqueId const&) ;
    void                              remove_link     (local_link_session_interface*)                  ;

    private:
        typedef std::unordered_map<UniqueId, local_link_selector> link_selectors;

        logger         const logger_         ;
        on_add               on_add_         ;
        on_remove            on_remove_      ;
        link_selectors       link_selectors_ ;
    };
}


namespace rubetek
{
    template <typename UniqueId>
    local_link_set<UniqueId>::local_link_set(on_add on_add, on_remove on_remove)
        : logger_    ( "local link set", log::level::debug )
        , on_add_    ( on_add                              )
        , on_remove_ ( on_remove                           )
    {}
    template <typename UniqueId>
    local_link_set<UniqueId>::~local_link_set()
    {
        MSW_ASSERT(link_selectors_.empty());
    }

    template <typename UniqueId>
    local_link_interface::on_received local_link_set<UniqueId>::add_link(local_link_session_interface* session, UniqueId const& id)
    {
        logger_.debug("add link: ", session, " for ", id);
        auto it = link_selectors_.find(id);
        bool added = false;
        if (it == link_selectors_.end())
        {
            it = link_selectors_.insert(std::make_pair(id, local_link_selector())).first;
            logger_.debug("link: ", session, " added");
            added = true;
        }
        else logger_.debug("link for ", id, " already exist");
        local_link_session_interface::on_received on_received = it->second.add_link(session);
        if (added) on_add_(id, &it->second);
        return on_received;
    }
    template <typename UniqueId>
    void local_link_set<UniqueId>::remove_link(local_link_session_interface* session)
    {
        logger_.debug("remove link: ", session);
        for (auto& pr : link_selectors_)
        {
            if (pr.second.remove_link_if_has(session))
            {
                if (!pr.second.has_links())
                {
                    on_remove_(pr.first);
                    logger_.debug("erase link: ", session);
                    link_selectors_.erase(pr.first);
                }
                return;
            }
        }
    }
}
