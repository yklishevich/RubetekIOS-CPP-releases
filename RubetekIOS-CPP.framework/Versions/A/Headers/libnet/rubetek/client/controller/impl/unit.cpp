#include <msw/std/memory.hpp>
#include <msw/return_move.hpp>
#include <msw/throw_runtime_error.hpp>
#include <msw/lexical_cast/bool2str.hpp>

#include <rubetek/log.hpp>
#include <rubetek/client/device_info.hpp>
#include <rubetek/local_link/selector.hpp>
#include <rubetek/client/controller_info.hpp>
#include <rubetek/client/controller/controller.hpp>
#include <rubetek/client/controller_availability.hpp>
#include <rubetek/client/controller/unit_back_interface.hpp>
#include <rubetek/client/controller/controller_back_interface.hpp>
#include <rubetek/client/controller/active_controller_back_interface.hpp>


#ifdef RUBETEK_CLIENT_CONTROLLERS_UNIT_LOG
#   define RUBETEK_CLIENT_CONTROLLERS_UNIT_ON_LOG(...) logger_.debug(__VA_ARGS__);
#else
#   define RUBETEK_CLIENT_CONTROLLERS_UNIT_ON_LOG(...)
#endif


namespace rubetek {
namespace client  {

    inline controllers_unit::controllers_unit(boost::asio::io_service& io_service, controllers_unit_back_interface* back_interface)
        : logger_            ( "controllers unit", log::level::info )
        , back_interface_    ( back_interface                       )
        , active_controller_ ( controllers_.end()                   )
        , timer_
        (
            io_service
            , msw::chrono::milliseconds(200)
            , [this]()
            {
                idle();
            }
        )
    {}

    inline void controllers_unit::add_local_link(unique_controller_id::const_reference id, local_link_selector* local_link, bool known)
    {
        auto it = controllers_.find(id);
        if (it == controllers_.end()) it = add_controller(id);
        logger_.info("add local link: ", id, ", known: ", msw::yes_no(known));
        it->second->add_local_link(local_link, known);
    }
    inline void controllers_unit::remove_local_link(unique_controller_id::const_reference id)
    {
        logger_.info("remove local link: ", id);
        auto it = get_iterator(id);
        it->second->remove_local_link();
        potentially_removal_controllers_.push_back(it);
    }

    inline void controllers_unit::add_remote_link
    (
          unique_controller_id::const_reference id
        , remote_link_interface<>*              remote_link
        , std::string const&                    ip_address
        , unique_channel_id_type const&         channel_id
    )
    {
        logger_.info("add remote link: ", id);
        auto it = controllers_.find(id);
        if (it == controllers_.end()) it = add_controller(id);
        it->second->add_remote_link(remote_link, ip_address, channel_id);
    }
    inline void controllers_unit::remove_remote_link(unique_controller_id::const_reference id)
    {
        logger_.info("remove remote link: ", id);
        auto it = get_iterator(id);
        it->second->remove_remote_link();
        potentially_removal_controllers_.push_back(it);
    }

    inline bool controllers_unit::has_controller(unique_controller_id::const_reference id) const
    {
        return controllers_.find(id) != controllers_.end();
    }

    inline known_controller_list controllers_unit::known_list() const
    {
        RUBETEK_CLIENT_CONTROLLERS_UNIT_ON_LOG("known list");
        known_controller_list list;
        for (auto const& cc : controllers_)
        {
            controller const& c = *cc.second;
            if ((c.get_state() != controller::state::init) && c.known())
            {
                unique_controller_id::type const id = cc.first;
                known_controller_info info;
                info.info  = make_info(id);
                info.alias = back_interface_->controllers_unit_on_get_alias(id);
                list.push_back(info);
            }
        }
        RUBETEK_CLIENT_CONTROLLERS_UNIT_ON_LOG("known list exit");
        MSW_RETURN_MOVE(list);
    }
    inline unknown_controller_list controllers_unit::unknown_list() const
    {
        unknown_controller_list list;
        for (auto const& cc : controllers_)
        {
            controller const& c = *cc.second;
            if (((c.get_state() == controller::state::standby) || (c.get_state() == controller::state::authentication)) && !c.known())
            {
                list.push_back(c.make_info());
            }
        }
        MSW_RETURN_MOVE(list);
    }

    inline void controllers_unit::add_known_controller(unique_controller_id::const_reference id)
    {
        add_controller(id, true);
    }
    inline void controllers_unit::add_unknown_controller(unique_controller_id::const_reference id)
    {
        add_controller(id, false);
    }
    inline void controllers_unit::remove_known_controller(unique_controller_id::const_reference id)
    {
        logger_.info("remove controller: ", id);
        auto it = get_iterator(id);
        it->second->make_unknown();
        if (has_active_controller() && (active_controller_id() == id)) disconnect_active_controller();
        remove_remote_link(id);
    }

    inline bool controllers_unit::can_connect_to(unique_controller_id::const_reference id) const
    {
        bool const result = get_iterator(id)->second->can_connect();
        RUBETEK_CLIENT_CONTROLLERS_UNIT_ON_LOG("can connect to: ", id, ": ", msw::yes_no(result));
        return result;
    }
    inline void controllers_unit::connect_to
    (
          unique_controller_id::const_reference id
        , active_controller_back_interface*     back_interface
        , device_info const&                    device
        , unsigned                              project_crc
        , std::string const&                    project_alias
    )
    {
        logger_.info("connect to: ", id);
        if (has_active_controller()) active_controller().disconnect();
        auto it = get_iterator(id);
        it->second->connect
        (
              back_interface
            , device
            , project_crc
            , project_alias
        );
        active_controller_ = it;
    }

    inline controller_info controllers_unit::make_info(unique_controller_id::const_reference id) const
    {
        logger_.debug("make info for: ", id);
        controller const& c = *get_iterator(id)->second;
        controller_info info;
        if (c.has_info()) info = c.make_info();
        else
        {
            info.id            = id                                                ;
            info.name          = back_interface_->controllers_unit_on_get_name(id) ;
            info.accessibility = c.availability()                                  ;
            info.active        = c.active()                                        ;
        }
        return info;
    }
    inline controller_availability controllers_unit::availability_of(unique_controller_id::const_reference id) const
    {
        RUBETEK_CLIENT_CONTROLLERS_UNIT_ON_LOG("call 'availability of': ", id);
        return get_iterator(id)->second->availability();
    }
    inline std::string controllers_unit::ip_address_of(unique_controller_id::const_reference id) const
    {
        RUBETEK_CLIENT_CONTROLLERS_UNIT_ON_LOG("call 'ip address of': ", id);
        return get_iterator(id)->second->ip_address();
    }

    inline bool controllers_unit::has_active_controller() const
    {
        return active_controller_ != controllers_.end();
    }
    inline controller& controllers_unit::active_controller()
    {
        if (!has_active_controller()) throw std::runtime_error("no active controller. can't get it");
        return *active_controller_->second;
    }
    inline unique_controller_id::const_reference controllers_unit::active_controller_id() const
    {
        if (!has_active_controller()) throw std::runtime_error("no active controller. can't get it id");
        return active_controller_->first;
    }
    inline void controllers_unit::disconnect_active_controller()
    {
        logger_.info("disconnect active controller");
        if (!has_active_controller()) throw std::runtime_error("no active controller. can't disconnect it");
        auto active = active_controller_;
        active_controller_ = controllers_.end();
        if (active->second->online()) active->second->disconnect();
    }

    inline void controllers_unit::set_auto_known(unique_controller_id::const_reference id)
    {
        logger_.info("set auto known");
        get_iterator(id)->second->set_auto_known();
    }

    inline void controllers_unit::idle()
    {
        if (!potentially_removal_controllers_.empty())
        {
            for (controllers::iterator const& it : potentially_removal_controllers_)
                try_remove(it);
            potentially_removal_controllers_.clear();
        }
    }

    inline controllers_unit::controllers::iterator controllers_unit::get_iterator(unique_controller_id::const_reference id)
    {
        auto it = controllers_.find(id);
        if (it == controllers_.end()) msw::throw_runtime_error("controller '", id, "' not found");
        return it;
    }
    inline controllers_unit::controllers::const_iterator controllers_unit::get_iterator(unique_controller_id::const_reference id) const
    {
        auto it = controllers_.find(id);
        if (it == controllers_.end()) msw::throw_runtime_error("controller '", id, "' not found");
        return it;
    }
    inline controllers_unit::controllers::iterator controllers_unit::add_controller(unique_controller_id::const_reference id, bool known)
    {
        logger_.info("add controller: ", id);
        auto ins_result = controllers_.insert
        (
            std::make_pair
            (
                id
                , msw::make_unique<controller>
                (
                      id
                    , this
                    , known
                )
            )
        );
        return ins_result.first;
    }
    inline void controllers_unit::on_remove_from_controller(unique_controller_id::const_reference id)
    {
        logger_.info("remove from controller: ", id);
        potentially_removal_controllers_.push_back(get_iterator(id));
        back_interface_->controllers_unit_on_removed_from_controller(id);
    }
    inline void controllers_unit::try_remove(controllers::iterator it)
    {
        logger_.debug("try remove controller: ", it->first);
        if ((it->second->get_state() == controller::state::offline) && !it->second->known())
        {
            logger_.info("remove controller: ", it->first);
            controllers_.erase(it);
            back_interface_->controllers_unit_on_any_controller_change_status();
        }
        else
        {
            if (it->second->known())                                   logger_.debug( "can't remove, it known"       );
            if (it->second->get_state() != controller::state::offline) logger_.debug( "can't remove, it not offline" );
        }
    }

    inline void controllers_unit::controller_on_change_status(unique_controller_id::const_reference)
    {
        back_interface_->controllers_unit_on_any_controller_change_status();
    }
    inline void controllers_unit::controller_on_switch_links(unique_controller_id::const_reference id)
    {
        back_interface_->controllers_unit_on_any_controller_change_status();
        controller_on_change_remote_connection(id);
    }
    inline void controllers_unit::controller_on_change_remote_connection(unique_controller_id::const_reference id)
    {
        if (has_active_controller() && (active_controller().id() == id) && active_controller().online())
        {
            back_interface_->controllers_unit_on_active_controller_change_connection();
        }
        back_interface_->controllers_unit_on_any_controller_change_status();
    }
    inline void controllers_unit::controller_on_make_known(unique_controller_id::const_reference id)
    {
        back_interface_->controllers_unit_on_became_known(id);
    }
    inline void controllers_unit::controller_on_removed_from_controller(unique_controller_id::const_reference id)
    {
        back_interface_->controllers_unit_on_removed_from_controller(id);
    }
    inline void controllers_unit::controller_on_set_offline(unique_controller_id::const_reference id)
    {
        if (has_active_controller() && (active_controller().id() == id))
        {
            back_interface_->controllers_unit_on_disconnected_active_controller();
            active_controller_ = controllers_.end();
        }
    }
    inline void controllers_unit::controller_on_rename(unique_controller_id::const_reference id)
    {
        back_interface_->controllers_unit_on_rename(id);
    }

}}
