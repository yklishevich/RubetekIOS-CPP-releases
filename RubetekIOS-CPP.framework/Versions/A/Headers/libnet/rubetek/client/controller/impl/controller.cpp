#include <memory>
#include <functional>

#include <msw/config.hpp>
#include <msw/std/memory.hpp>
#include <msw/callback/event.hpp>
#include <msw/dimension/trigger.hpp>
#include <msw/percentage_relation.hpp>
#include <msw/lexical_cast/bool2str.hpp>

#include <rubetek/log.hpp>
#include <rubetek/essence/property.hpp>
#include <rubetek/local_link/interface.hpp>
#include <rubetek/remote_link/interface.hpp>
#include <rubetek/client/controller_info.hpp>
#include <rubetek/proto/ihmlp/async_transceiver.hpp>
#include <rubetek/client/controller/ihap_transceiver.hpp>
#include <rubetek/client/controller/controller_back_interface.hpp>
#include <rubetek/client/controller/active_controller_back_interface.hpp>


namespace rubetek {
namespace client  {

    inline controller::controller
    (
          unique_controller_id::const_reference id
        , controller_back_interface*            back_interface
        , bool                                  known
    )
        : logger_           ( "controller", id                                                                             )
        , id_               ( id                                                                                           )
        , back_interface_   ( back_interface                                                                               )
        , state_            ( state::offline                                                                               )
        , known_            ( known                                                                                        )
        , ihap_transceiver_ ( std::bind(&ihmlp::async_transceiver::send, &ihmlp_transceiver_, std::placeholders::_1), this )
        , ihmlp_transceiver_
        (
            [this](msw::range<msw::byte const> packet)
            {
                //logger_.debug("ihap_transceiver_.process_ihap_packet");
                ihap_transceiver_.process_ihap_packet(packet);
                //logger_.debug("ihap_transceiver_.process_ihap_packet exit");
            }
            , [this]()
            {
                ihap_transceiver_.ihap_reset();
            }
        )
        , active_back_interface_(nullptr)
    {}

    inline void controller::add_local_link(local_link_interface* local_link, bool known)
    {
        logger_.debug("add local link, known: ", known);
        ihmlp_transceiver_.set_primary_link
        (
            [local_link](msw::range<msw::byte const> packet)
            {
                MSW_ASSERT(local_link->has_links());
                if (local_link->has_links()) local_link->send(packet);
            }
        );
        local_link->set_on_received([this](msw::range<msw::byte const> packet)
        {
            if (packet.empty()) logger_.error("received empty packet from local link");
            else ihmlp_transceiver_.process_ihmlp_packet(packet);
        });

        ip_address_.get_local = [local_link]()
        {
            return local_link->ip_address();
        };
        known_.set(known);
        if (state_ == state::offline) start_init();
        else back_interface_->controller_on_switch_links(id_);
    }
    inline void controller::remove_local_link()
    {
        logger_.debug("remove local link");
        ip_address_.get_local = nullptr;
        ihmlp_transceiver_.reset_primary_link();
        if (!ihmlp_transceiver_.has_links()) set_offline();
        else back_interface_->controller_on_switch_links(id_);
    }
    inline void controller::add_remote_link(remote_link_interface<>* remote_link, std::string const& ip_address, unique_channel_id_type const& remote_channel_id)
    {
        logger_.debug("add remote link");
        ihmlp_transceiver_.set_secondary_link
        (
            [this, remote_link](msw::range<msw::byte const> packet)
            {
                remote_link->send(packet);
            }
        );
        remote_link->set_on_received([this](msw::range<msw::byte const> packet)
        {
            if (packet.empty()) logger_.error("received empty packet from remote link");
            else ihmlp_transceiver_.process_ihmlp_packet(packet);
        });

        if (state_ == state::offline) start_init();
        ip_address_.remote = ip_address;
        remote_channel_id_ = remote_channel_id;
        back_interface_->controller_on_change_remote_connection(id_);
    }
    inline void controller::remove_remote_link()
    {
        logger_.debug("remove remote link");
        ip_address_.remote.clear();
        remote_channel_id_.clear();
        ihmlp_transceiver_.reset_secondary_link();
        if (!ihmlp_transceiver_.has_links()) set_offline();
        back_interface_->controller_on_change_remote_connection(id_);
    }

    inline unique_controller_id::const_reference controller::id() const
    {
        return id_;
    }
    inline std::string controller::name() const
    {
        return make_info().name;
    }
    inline controller::state controller::get_state() const
    {
        return state_;
    }
    inline bool controller::known() const
    {
        return static_cast<bool>(known_);
    }
    inline bool controller::online() const
    {
        return state_ != state::offline;
    }
    inline bool controller::active() const
    {
        return state_ > state::standby;
    }
    inline bool controller::can_connect() const
    {
        return state_ == state::standby;
    }
    inline std::string controller::ip_address() const
    {
        return has_local_link() ? ip_address_.get_local() : ip_address_.remote;
    }
    inline std::string controller::local_ip_address() const
    {
        return has_local_link() ? ip_address_.get_local() : std::string();
    }
    inline std::string controller::remote_ip_address() const
    {
        return ip_address_.remote;
    }
    inline bool controller::has_info() const
    {
        return static_cast<bool>(info_);
    }
    inline controller_info controller::make_info() const
    {
        if (!info_) msw::throw_runtime_error("no controller info");
        return controller_info
        ({
              id_
            , info_->name
            , info_->version
            , info_->system_info
            , info_->demo_mode
            , local_ip_address ()
            , remote_ip_address()
            , remote_channel_id_
            , info_->server_address
            , availability()
            , active()
        });
    }
    inline bool controller::has_local_link() const
    {
        return ihmlp_transceiver_.has_primary_link();
    }
    inline bool controller::demo_mode() const
    {
        if (!info_) msw::throw_runtime_error("no controller info");
        return info_->demo_mode;
    }
    inline controller_availability controller::availability() const
    {
        return get_state() == state::offline
            ? controller_availability::none
            : 
            (
                has_local_link()
                ? controller_availability::local
                : controller_availability::remote
            );
    }

    inline void controller::connect
    (
          active_controller_back_interface* back_interface
        , device_info const&                device
        , unsigned                          project_crc
        , std::string const&                project_alias
    )
    {
        logger_.debug("connect");
        if (state_ != state::standby) throw std::runtime_error("connection controller fail");
        active_back_interface_ = back_interface;
        ihap_transceiver_.activate
        (
              device
            , project_crc
            , project_alias
        );
    }
    inline void controller::disconnect()
    {
        logger_.debug("disconnect");
        if (state_ <= state::standby) logger_.error("disconnect fail. controller not connected");
        active_back_interface_ = nullptr;
        ihap_transceiver_.inactivate();
        set_state(state::standby);
    }

    inline void controller::send_password(std::string const& pwd)
    {
        ihap_transceiver_.send_password(pwd);
    }
    inline void controller::send_project(std::string const& project)
    {
        ihap_transceiver_.send_project(project);
    }
    inline void controller::send_property(property_address addr, property_list const& list)
    {
        ihap_transceiver_.send_property(addr, list);
    }
    inline void controller::send_module_command(msw::range<msw::byte const> cmd)
    {
        logger_.debug("send module command");
        ihap_transceiver_.send_module_command(cmd);
        //logger_.debug("send_module_command exit");
    }
    inline void controller::send_project_alias(std::string const& project_alias)
    {
        ihap_transceiver_.send_project_alias(project_alias);
    }
    inline void controller::send_notification_id(std::string const& id)
    {
        ihap_transceiver_.send_notification_id(id);
    }
    inline void controller::refresh()
    {
        ihap_transceiver_.refresh();
    }

    inline void controller::set_auto_known()
    {
        logger_.debug("set auto known");
        if (known_) logger_.error("already known");
        else
        {
            known_.set(true);
            if (state_ == state::offline) auto_known_.set();
            else if (state_ == state::standby) ihap_transceiver_.become_known();
        }
    }
    inline void controller::make_unknown()
    {
        logger_.debug("make unknown");
        known_.reset();
        if (state_ != state::offline) ihap_transceiver_.remove();
        //logger_.debug("make unknown exit");
    }

    inline void controller::remove_client(unique_client_id::const_reference id)
    {
        logger_.debug("remove client: ", id);
        ihap_transceiver_.remove_client(id);
    }

    inline std::string controller::state_to_str(controller::state s)
    {
        if ( s == state::offline         ) return "offline"         ;
        if ( s == state::init            ) return "init"            ;
        if ( s == state::standby         ) return "standby"         ;
        if ( s == state::authentication  ) return "authentication"  ;
        if ( s == state::loading_project ) return "loading-project" ;
        if ( s == state::active          ) return "active"          ;
        MSW_ASSERT(0);
        return "";
    }

    inline void controller::on_controller_info
    (
          std::string const& name
        , std::string const& version
        , std::string const& system_info
        , bool               demo_mode
        , std::string const& server_address
    )
    {
        logger_.debug("info: name: ", name, ", version: ", version, ", sys: ", system_info, ", demo: ", msw::yes_no(demo_mode), ", server: ", server_address);
        info_ = msw::make_unique<info>(info(name, version, system_info, demo_mode, server_address));
        if (state_ == state::init) set_state(state::standby);
        else logger_.warning("controller info received at not init state");
        auto_known_.reset();
    }
    inline void controller::on_controller_rename(std::string const& name)
    {
        logger_.debug("rename: ", name);
        if (info_)
        {
            info_->name = name;
            back_interface_->controller_on_rename(id_);
        }
        else logger_.error("rename not done");
    }
    inline void controller::on_remove()
    {
        logger_.debug("remove");
        back_interface_->controller_on_removed_from_controller(id_);
    }
    inline void controller::on_query_password(option_was_removed was_removed)
    {
        logger_.debug("query password");
        set_state(state::authentication);
        active_back_interface()->active_controller_on_query_password(was_removed);
    }
    inline void controller::on_no_project()
    {
        logger_.debug("no project");
        set_known();
        set_state(state::active);
        active_back_interface()->active_controller_on_no_project();
    }
    inline void controller::on_update_project(unsigned size)
    {
        logger_.debug("update project");
        set_known();
        if (state_ == state::loading_project)
            throw std::runtime_error("project already loading");
        else
        {
            set_state(state::loading_project);
            project_loader_.reset(new project_loader(size));
            active_back_interface()->active_controller_on_update_project();
        }
    }
    inline void controller::on_large_block(msw::range<msw::byte const> block)
    {
        if (project_loader_ && project_loader_->wait_prj)
        {
            if (project_loader_->wait_prj->type == wait_project::project)
                active_back_interface()->active_controller_on_project(block, project_loader_->wait_prj->crc);
            project_loader_->wait_prj.reset();
            project_loader_->loaded_size += static_cast<unsigned>(block.size().count());
        }
        else throw std::runtime_error("project not expected");
    }
    inline void controller::on_large_block_enriched(msw::size<msw::byte> sz)
    {
        if (!project_loader_) throw std::runtime_error("actively project not loading");
        int const progress = msw::percentage_relation(project_loader_->loaded_size + sz.count(), project_loader_->complete_size);
        active_back_interface()->active_controller_on_loading_progress(progress);
    }
    inline void controller::on_loading_cancel()
    {
        set_state(state::active);
        ihap_transceiver_.reset_large_block_transaction();
    }
    inline void controller::on_project(unsigned crc)
    {
        if (project_loader_)
            project_loader_->wait_prj.reset(new wait_project({crc, wait_project::project}));
        else
            throw std::runtime_error("loading project not expected");
    }
    inline void controller::on_ready()
    {
        logger_.debug("ready");
        set_known();
        set_state(state::active);
        active_back_interface()->active_controller_on_ready();
    }
    inline void controller::on_property(property_address address, property_list const& list)
    {
        active_back_interface()->active_controller_on_property(address, list);
    }
    inline void controller::on_module_command(std::string const& cmd)
    {
        if (active_back_interface_) active_back_interface()->active_controller_on_module_command(cmd);
        else logger_.debug("on module command: no active back interface");
    }

    inline active_controller_back_interface* controller::active_back_interface() const
    {
        if (!active_back_interface_) msw::throw_runtime_error("controller not connected. no active back interface");
        return active_back_interface_;
    }

    inline void controller::set_state(state s)
    {
        logger_.debug("set state: ", state_to_str(state_), "->", state_to_str(s));
        if (state_ != s)
        {
            state_ = s;
            back_interface_->controller_on_change_status(id_);
        }
    }
    inline void controller::set_offline()
    {
        logger_.debug("set offline");
        info_.reset();
        project_loader_.reset();
        set_state(state::offline);
        back_interface_->controller_on_set_offline(id_);
    }
    inline void controller::set_known()
    {
        logger_.debug("set known");
        if (known_) logger_.debug("already known");
        else
        {
            known_.set();
            back_interface_->controller_on_make_known(id_);
        }
    }
    inline void controller::start_init()
    {
        logger_.debug("start init");
        if (auto_known_) ihap_transceiver_.become_known();
        set_state(state::init);
        ihap_transceiver_.query_controller_info();
    }

}}
