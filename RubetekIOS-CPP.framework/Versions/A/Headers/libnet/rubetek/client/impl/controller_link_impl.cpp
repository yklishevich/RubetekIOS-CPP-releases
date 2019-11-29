#include <string>
#include <algorithm>

#include <rubetek/config.hpp>

#include <msw/assert.hpp>
#include <msw/format.hpp>
#include <msw/std/memory.hpp>
#include <msw/normilize_path.hpp>
#include <msw/throw_runtime_error.hpp>
#include <msw/lexical_cast/bool2str.hpp>
#include <msw/lexical_cast/bytes_as_string.hpp>
#include <msw/lexical_cast/network/query/from_string_ex.hpp>

#include <rubetek/config.hpp>
#include <rubetek/application/build_info.hpp>
#include <rubetek/essence/property.hpp>
#include <rubetek/essence/unique_id/client.hpp>
#include <rubetek/essence/default_port/server.hpp>
#include <rubetek/essence/unique_id/controller.hpp>
#include <rubetek/client/controller_link_context.hpp>
#include <rubetek/client/controller_link_back_interface.hpp>


#ifdef RUBETEK_CLIENT_CONTROLLER_LINK_LOG
#   define RUBETEK_CLIENT_CONTROLLER_LINK_ON_LOG(...) logger_.debug(__VA_ARGS__);
#else
#   define RUBETEK_CLIENT_CONTROLLER_LINK_ON_LOG(...)
#endif


namespace rubetek {
namespace client  {

    inline std::string deduce_projects_path(std::string const& work_path)
    {
        return msw::normilize_path(work_path) + "projects/";
    }

    inline controller_link_impl::controller_link_impl
    (
          controller_link_back_interface*   back_interface
        , unique_client_id::const_reference client_id
        , controller_link_context const&    context
        , bool                              //focus
        , bool                              disable_start_logic
        , bool                              disable_find_on_empty
    )
        : logger_("controller link" , log::level::info)
        , show_build_info_
        (
            [this, client_id, context]()
            {
                logger_.info(build_info());
                logger_.info("client id: "  , client_id);
                logger_.info("device name: ", context.device.name, "; os: ", context.device.operation_system);
            }
        )
        , context_
        ({
              client_id
            , context.device
        })
        , callback_service_ ( back_interface                          )
        , configurator_     ( msw::normilize_path (context.work_path) )
        , project_manager_  ( deduce_projects_path(context.work_path) )
        , show_list_        ( show_list::none                         )
        , controllers_unit_ ( io_service_, this                       )
        , local_unit_
        (
              io_service_
            , client_id
            , this
        )
        , remote_unit_
        (
              io_service_
            , context.server_address.empty()
                ? boost::none
                : boost::optional<remote_unit::query>(msw::tcp_query_from_string_ex(context.server_address, context.use_ssl ? default_server_ssl_port : default_server_port))
            , context.use_ssl
            , client_id
            , this
        )
        , preconstructor_
        (
            [this, disable_start_logic, disable_find_on_empty]()
            {
                if (!check_configuration())
                {
                    logger_.error("discrepancy configuration files");
                    project_manager_.clear();
                }
                std::list<unique_controller_id::type> projects_id = project_manager_.projects_id();
                logger_.info("add known controllers from project manager");
                for (auto id : projects_id)
                    controllers_unit_.add_known_controller(id);
                if (disable_start_logic) logger_.info("start logic is disabled");
                else start(!disable_find_on_empty);
            }
        )
        , timer_
        (
            io_service_
            , msw::chrono::milliseconds(5000)
            , [this]()
            {
#ifdef RUBETEK_CLIENT_CONTROLLER_LINK_TICKS
                logger_.debug("tick");
#endif
            }
        )
        , io_thread_(io_service_)
    {
        logger_.info("auto connect by location: " , msw::yes_no(configurator_.auto_connect_by_location()) );
        logger_.info("auto connect to recent: "   , msw::yes_no(configurator_.auto_connect_to_recent  ()) );
#ifdef RUBETEK_CLIENT_CONTROLLER_LINK_TICKS
        msw::thread
        (
            [&]()
            {
                for (;;)
                {
                    msw::this_thread::sleep_for(msw::chrono::seconds(3));
                    log::debug("tick");
                }
            }
        ).detach();
#endif
    }
    inline controller_link_impl::~controller_link_impl()
    {
        io_service_.stop();
    }

    inline void controller_link_impl::auto_connect_by_location(bool yes)
    {
        logger_.debug("auto connect by location: ", yes);
        io_service_.post([this, yes]()
        {
            configurator_.auto_connect_by_location(yes);
        });
    }
    inline bool controller_link_impl::auto_connect_by_location() const
    {
        return configurator_.auto_connect_by_location();
    }
    inline void controller_link_impl::auto_connect_to_recent(bool yes)
    {
        logger_.debug("auto connect to recent: ", yes);
        io_service_.post([this, yes]()
        {
            configurator_.auto_connect_to_recent(yes);
        });
    }
    inline bool controller_link_impl::auto_connect_to_recent() const
    {
        return configurator_.auto_connect_to_recent();
    }

    inline void controller_link_impl::focus(bool yes)
    {
        logger_.debug("focus: ", msw::yes_no(yes));
    }

    inline void controller_link_impl::show_unknown_controllers()
    {
        logger_.debug("show unknown controllers");
        io_service_.post([this]()
        {
            start_timer_.reset();
            find_first_controller_.reset();
            self_show_unknown_controllers();
        });
    }
    inline void controller_link_impl::show_known_controllers()
    {
        logger_.debug("show known controllers");
        io_service_.post([this]()
        {
            start_timer_.reset();
            find_first_controller_.reset();
            self_show_known_controllers();
        });
    }
    inline void controller_link_impl::show_all_controllers()
    {
        logger_.debug("show all controllers");
        io_service_.post([this]()
        {
            start_timer_.reset();
            find_first_controller_.reset();
            self_show_all_controllers();
        });
    }
    inline void controller_link_impl::cancel_show_list()
    {
        logger_.debug("cancel show list");
        io_service_.post([this]()
        {
            self_cancel_show_list();
        });
    }
    inline void controller_link_impl::cancel_find_controller()
    {
        logger_.debug("cancel find controller");
        io_service_.post([this]()
        {
            start_timer_.reset();
            find_first_controller_.reset();
        });
    }

    inline void controller_link_impl::rename_controller(unique_controller_id::const_reference id, std::string name)
    {
        logger_.debug("rename known controller: ", id, "; exist: ", msw::yes_no(project_manager_.has_project(id)), "; new name: ", name);
        io_service_.post([this, id, name]()
        {
            logger_.debug("rename controller: ", id);
            if (project_manager_.has_project(id))
            {
                project_manager_.add_alias(id, name);
                if (controllers_unit_.has_active_controller()) controllers_unit_.active_controller().send_project_alias(name);
            }
            else logger_.warning("controller: ", id, " not found");
        });
    }
    inline void controller_link_impl::remove_controller(unique_controller_id::const_reference id)
    {
        logger_.debug("remove known controller: ", id, "; exist: ", msw::yes_no(project_manager_.has_project(id)));
        io_service_.post([this, id]()
        {
            logger_.debug("remove controller: ", id);
            remove_from_controller(id);
        });
    }
    inline void controller_link_impl::remove_all_controllers()
    {
        logger_.debug("remove all controllers");
        io_service_.post([this]()
        {
            logger_.debug("remove controllers");
            std::list<unique_controller_id::type> const current_projects = project_manager_.projects_id();
            for (unique_controller_id::type id : current_projects)
                remove_from_controller(id);
            MSW_ASSERT(!project_manager_.has_projects());
        });
    }

    inline void controller_link_impl::connect_to(unique_controller_id::const_reference id)
    {
        logger_.debug("connect to: ", id);
        io_service_.post([this, id]()
        {
            callback_service_.cancel_on_known_controllers  ();
            callback_service_.cancel_on_unknown_controllers();
            start_timer_.reset();
            find_first_controller_.reset();
            self_connect_to(id);
        });
    }
    inline void controller_link_impl::disconnect()
    {
        logger_.debug("disconnect");
        io_service_.post([this]()
        {
            self_disconnect();
        });
    }
    inline int controller_link_impl::updating_progress() const
    {
        if (!updating_progress_) msw::throw_runtime_error("updating progress inaccessible");
        return *updating_progress_;
    }
    inline void controller_link_impl::refresh()
    {
        logger_.debug("refresh");
        io_service_.post([this]()
        {
            if (controllers_unit_.has_active_controller())
                controllers_unit_.active_controller().refresh();
            else
                logger_.warning("refresh failed: no active controller");
        });
    }
    inline void controller_link_impl::send_password(std::string password)
    {
        logger_.debug("send password: ", password);
        io_service_.post([this, password]()
        {
            if (controllers_unit_.has_active_controller())
                controllers_unit_.active_controller().send_password(password);
            else
                logger_.warning("send password failed: no active controller");
        });
    }
    inline void controller_link_impl::send_project(std::string project)
    {
        logger_.debug("send project");
        //logger_.debug("project content: ", project);
        io_service_.post([this, project]()
        {
            if (controllers_unit_.has_active_controller())
                controllers_unit_.active_controller().send_project(project);
            else
                logger_.warning("send project failed: no active controller");
        });
    }
    inline void controller_link_impl::send_property(property_address address, property pt)
    {
        RUBETEK_CLIENT_CONTROLLER_LINK_ON_LOG("send property");
        property_list list;
        list.push_back(pt);
        send_property(address, list);
    }
    inline void controller_link_impl::send_property(property_address address, property_list list)
    {
        RUBETEK_CLIENT_CONTROLLER_LINK_ON_LOG("send property list");
        io_service_.post([this, address, list]()
        {
            if (controllers_unit_.has_active_controller())
                controllers_unit_.active_controller().send_property(address, list);
            else
                logger_.warning("send property list failed: no active controller");
        });
    }
    inline void controller_link_impl::send_command(msw::range<msw::byte const> command)
    {
        RUBETEK_CLIENT_CONTROLLER_LINK_ON_LOG("send module command", "; size: ", command.size());
        msw::buffer<msw::byte> cmd(command);
        io_service_.post([this, cmd]()
        {
            if (controllers_unit_.has_active_controller())
                controllers_unit_.active_controller().send_module_command(cmd.all());
            else
                logger_.warning("send module command failed: no active controller");
        });
    }
    inline void controller_link_impl::send_server_directly(server_packet_type type, std::string command)
    {
        RUBETEK_CLIENT_CONTROLLER_LINK_ON_LOG("send server directly: command type: ", static_cast<int>(type), " size: ", command.size());
        io_service_.post([this, type, command]()
        {
            remote_unit_.send_server_directly(type, command);
        });
    }
    inline void controller_link_impl::send_server_auxiliary(std::string command)
    {
        RUBETEK_CLIENT_CONTROLLER_LINK_ON_LOG("send server auxiliary; command size: ", command.size());
        io_service_.post([this, command]()
        {
            remote_unit_.send_server_auxiliary(command);
        });
    }
    inline void controller_link_impl::send_notification_id(std::string id)
    {
        RUBETEK_CLIENT_CONTROLLER_LINK_ON_LOG("send notification id: ", id);
        io_service_.post([this, id]()
        {
            if (controllers_unit_.has_active_controller())
                controllers_unit_.active_controller().send_notification_id(id);
            else
                logger_.warning("send notification id failed: no active controller");
        });
    }

    inline void controller_link_impl::remove_client(unique_controller_id::const_reference id)
    {
        RUBETEK_CLIENT_CONTROLLER_LINK_ON_LOG("remove client: ", id);
        io_service_.post([this, id]()
        {
            if (controllers_unit_.has_active_controller())
                controllers_unit_.active_controller().remove_client(id);
            else
                logger_.warning("remove client failed: no active controller");
        });
    }

    inline void controller_link_impl::synchronize_projects(account_projects lst)
    {
        std::string s;
        for (account_project_info inf : lst)
            s += msw::format(inf.id, "(", inf.name, "); ");
        if (lst.empty()) s = "empty";
        logger_.debug("synchronize projects: ", s);
        if (!lst.empty())
        {
            io_service_.post([this, lst]()
            {
                self_synchronize_projects(lst);
            });
        }
    }

    inline void controller_link_impl::process_events()
    {
        //RUBETEK_CLIENT_CONTROLLER_LINK_ON_LOG("process_events");
        callback_service_.run();
        //RUBETEK_CLIENT_CONTROLLER_LINK_ON_LOG("process_events exit");
    }

    inline std::list<unique_controller_id::type> controller_link_impl::projects_id_list(std::string const& work_path)
    {
        return project_manager_type(deduce_projects_path(work_path)).projects_id();
    }

    inline bool controller_link_impl::check_configuration() const
    {
        //if (configurator_.has_recent_controller())
        //  return project_manager_.has_project(configurator_.recent_controller());
        return true;
    }
    inline void controller_link_impl::start(bool enable_find_on_empty)
    {
        logger_.debug("start");
        logger_.debug("enable find on empty: ", msw::yes_no(enable_find_on_empty));
        if (project_manager_.has_projects())
        {
            logger_.debug("has projects: yes");
            if (configurator_.auto_connect_by_location())
            {
                logger_.debug("auto connect by location");
                callback_service_.on_find_controller();
                start_timer_ = msw::make_unique<msw::timer>
                (
                    io_service_
                    , msw::chrono::seconds(1)
                    , [this]()
                    {
                        known_controller_list const list = controllers_unit_.known_list();
                        std::list<unique_controller_id::type> known_local_list;
                        for (known_controller_info const& info : list)
                            if (info.info.accessibility == controller_availability::local)
                                known_local_list.push_back(info.info.id);
                        if (!known_local_list.empty())
                        {
                            if
                            (
                                (known_local_list.size() > 1)
                                && configurator_.auto_connect_to_recent()
                                && configurator_.has_recent_controller()
                                && (std::find(known_local_list.begin(), known_local_list.end(), configurator_.recent_controller()) != known_local_list.end())
                            )
                            self_connect_to(configurator_.recent_controller());
                            else self_connect_to(known_local_list.front());
                        }
                        else
                        {
                            if (configurator_.auto_connect_to_recent() && configurator_.has_recent_controller()) self_connect_to(configurator_.recent_controller());
                            else
                            {
                                if (project_manager_.has_projects()) self_show_known_controllers();
                                else
                                {
                                    unknown_controller_list const unknown_list = controllers_unit_.unknown_list();
                                    if (unknown_list.empty()) find_first_controller_.set();
                                    else
                                    {
                                        if (unknown_list.size() == 1) self_connect_to(unknown_list.front().id);
                                        else self_show_unknown_controllers();
                                    }
                                }
                            }
                        }
                        start_timer_.reset();
                    }
                );
            }
            else
            {
                if (configurator_.auto_connect_to_recent() && configurator_.has_recent_controller()) self_connect_to(configurator_.recent_controller());
                else self_show_known_controllers();
            }
        }
        else
        {
            logger_.debug("no projects");
            if (enable_find_on_empty)
            {
                callback_service_.on_find_controller();
                find_first_controller_.set();
            }
        }
    }
    inline void controller_link_impl::controller_activated()
    {
        logger_.debug("controller activated");
        unique_controller_id::type const id = controllers_unit_.active_controller_id();
        bool const demo_mode = controllers_unit_.active_controller().demo_mode();
        if (!project_manager_.has_project(id)) project_manager_.add_project(id, demo_mode);
        project_manager_.update_name(id, controllers_unit_.active_controller().name());
        if (!demo_mode) configurator_.recent_controller(id);
        self_on_any_controller_change_state();
    }
    inline void controller_link_impl::active_controller_changed_connection_status()
    {
        logger_.debug("active controller changed connection status");
        if (controllers_unit_.has_active_controller())
        {
            unique_controller_id::type const id = controllers_unit_.active_controller_id();
            controller_availability const availability = controllers_unit_.availability_of(id);
            if (availability == controller_availability::none)
            {
                callback_service_.cancel_on_controller_error    ();
                callback_service_.cancel_on_password_request    ();
                callback_service_.cancel_on_project_error       ();
                callback_service_.cancel_on_project_updating    ();
                callback_service_.cancel_on_ready               ();
                callback_service_.cancel_on_properties_received ();
            }
            callback_service_.on_controller_connection_status(availability, controllers_unit_.ip_address_of(id));
        }
        else callback_service_.on_controller_connection_status(controller_availability::none, "");
    }
    inline void controller_link_impl::remove_from_controller(unique_controller_id::const_reference id)
    {
        logger_.debug("remove from controller: ", id);
        if (project_manager_.has_project(id))
        {
            if (wait_for_connect_                     && (*wait_for_connect_                == id)) wait_for_connect_ = boost::none          ;
            if (configurator_.has_recent_controller() && (configurator_.recent_controller() == id)) configurator_.remove_recent_controller() ;
            controllers_unit_ .remove_known_controller (id) ; // remove_known_controller -> make_unknown -> remove_remote_link
            project_manager_  .remove_project          (id) ;
            remote_unit_      .close_channel           (id) ;
            self_on_any_controller_change_state();
        }
        else logger_.warning("controller: ", id, " not found");
    }

    inline void controller_link_impl::self_connect_to(unique_controller_id::type id)
    {
        logger_.debug("connect to controller: ", id);
        configurator_.recent_controller(id);
        wait_for_connect_ = boost::none;
        if (controllers_unit_.has_active_controller()) controllers_unit_.disconnect_active_controller();
        if (controllers_unit_.has_controller(id))
        {
            if (controllers_unit_.can_connect_to(id))
            {
                project_manager<unique_controller_id::type>::project_info project_info;
                if (project_manager_.has_project(id)) project_info = project_manager_.get_project_info(id);
                controllers_unit_.connect_to
                (
                      id
                    , this
                    , context_.device
                    , project_info.project_crc
                    , project_info.alias
                );
            }
            else
            {
                wait_for_connect_ = id;
            }
        }
        else
        {
            logger_.info("connect to unknown and local unavailable controller: ", id);
            controllers_unit_.add_unknown_controller(id);
            remote_unit_.open_channel(id);
            wait_for_connect_ = id;
        }
        callback_service_.on_connecting_to_controller
        (
            controllers_unit_.make_info(id)
            , project_manager_.has_project(id) ? project_manager_.alias(id) : std::string()
        );
        active_controller_changed_connection_status();
    }
    inline void controller_link_impl::self_disconnect()
    {
        logger_.debug("self disconnect");
        wait_for_connect_ = boost::none;
        if (controllers_unit_.has_active_controller())
            controllers_unit_.disconnect_active_controller();
    }
    inline void controller_link_impl::self_show_unknown_controllers()
    {
        logger_.debug("self show unknown controllers");
        show_list_ = show_list::unknown;
        callback_service_.cancel_on_known_controllers ();
        callback_service_.cancel_on_all_controllers   ();
        callback_service_.on_unknown_controllers(controllers_unit_.unknown_list());
    }
    inline void controller_link_impl::self_show_known_controllers()
    {
        logger_.debug("self show known controllers");
        show_list_ = show_list::known;
        callback_service_.cancel_on_unknown_controllers ();
        callback_service_.cancel_on_all_controllers     ();
        callback_service_.on_known_controllers(controllers_unit_.known_list());
    }
    inline void controller_link_impl::self_show_all_controllers()
    {
        logger_.debug("self show all controllers");
        show_list_ = show_list::all;
        callback_service_.cancel_on_known_controllers   ();
        callback_service_.cancel_on_unknown_controllers ();
        callback_service_.on_all_controllers(controllers_unit_.known_list(), controllers_unit_.unknown_list());
    }
    inline void controller_link_impl::self_cancel_show_list()
    {
        logger_.debug("self cancel show list");
        show_list_ = show_list::none;
        callback_service_.cancel_on_known_controllers   ();
        callback_service_.cancel_on_unknown_controllers ();
        callback_service_.cancel_on_all_controllers     ();
    }
    inline void controller_link_impl::self_synchronize_projects(account_projects const& lst)
    {
        logger_.debug("synchronize projects");
        std::list<unique_controller_id::type> const current_projects = project_manager_.projects_id();
        std::list<unique_controller_id::type> projects_for_remove;
        {
            for (auto id : current_projects)
            {
                bool remove_it = true;
                for (account_project_info inf : lst)
                {
                    if (inf.id == id)
                    {
                        remove_it = false;
                        break;
                    }
                }
                if (remove_it) projects_for_remove.push_back(id);
            }
            if (!projects_for_remove.empty())
            {
                std::string s;
                for (auto id : projects_for_remove) s += id + ";";
                logger_.info("projects for remove: ", s);
            }
            for (auto id : projects_for_remove) remove_from_controller(id);
        }
        account_projects projects_for_add;
        {
            for (account_project_info inf : lst)
            {
                bool add_it = true;
                for (auto id : current_projects)
                {
                    if (id == inf.id)
                    {
                        add_it = false;
                        break;
                    }
                }
                if (add_it) projects_for_add.push_back(inf);
            }
            if (!projects_for_add.empty())
            {
                std::string s;
                for (auto inf : projects_for_add) s += inf.id + ";";
                logger_.info("projects for add: ", s);
            }
            for (auto inf : projects_for_add)
            {
                project_manager_.add_project(inf.id, false);
                project_manager_.update_name(inf.id, inf.name);
                if (controllers_unit_.has_controller(inf.id))
                {
                    if (!controllers_unit_.can_connect_to(inf.id)) logger_.error("can't connect to unknown controller");
                }
                else
                {
                    remote_unit_.open_channel(inf.id);
                    controllers_unit_.add_known_controller(inf.id);
                }
                controllers_unit_.set_auto_known(inf.id);
            }
        }
        logger_.debug("synchronize projects completed");
    }
    inline void controller_link_impl::self_on_any_controller_change_state()
    {
        RUBETEK_CLIENT_CONTROLLER_LINK_ON_LOG("self on any controller change state");
        if (show_list_ == show_list::known)
        {
            self_show_known_controllers();
        }
        else
        {
            if (show_list_ == show_list::unknown)
            {
                self_show_unknown_controllers();
            }
            else
            {
                if (show_list_ == show_list::all)
                {
                    self_show_all_controllers();
                }
            }
        }
        if (wait_for_connect_ && controllers_unit_.has_controller(*wait_for_connect_) && controllers_unit_.can_connect_to(*wait_for_connect_))
        {
            /*
                add controllers_unit_.has_controller(id)
                fix: client подключается к неизвестному CC, на этапе ввода пароля происходит разрыв соединения
                информация о СС удаляется из controllers_unit_ (т.к. он неизвестный и нет соединений с ним),
                вызывается can_connect_to, а СС такого нет.
            */
            self_connect_to(*wait_for_connect_);
        }
    }

    inline void controller_link_impl::controllers_unit_on_any_controller_change_status()
    {
        RUBETEK_CLIENT_CONTROLLER_LINK_ON_LOG("controllers unit: on any controller change status");
        if (find_first_controller_)
        {
            find_first_controller_.reset();
            start_timer_ = msw::make_unique<msw::timer>
            (
                io_service_
                , msw::chrono::seconds(1)
                , [this]()
                {
                    unknown_controller_list const list = controllers_unit_.unknown_list();
                    if (list.empty()) find_first_controller_.set();
                    else
                    {
                        if (list.size() == 1) self_connect_to(list.front().id);
                        else self_show_unknown_controllers();
                    }
                    start_timer_.reset();
                }
            );
        }
        self_on_any_controller_change_state();
    }
    inline void controller_link_impl::controllers_unit_on_became_known(unique_controller_id::const_reference id)
    {
        RUBETEK_CLIENT_CONTROLLER_LINK_ON_LOG("controllers unit: on became known");
        /*
        has_channel: при вызове connect_to в случае 'connect to unknown and local unavailable controller' вызвается open_channel
        */
        if (!remote_unit_.has_channel(id)) remote_unit_.open_channel(id);
    }
    inline void controller_link_impl::controllers_unit_on_removed_from_controller(unique_controller_id::const_reference id)
    {
        RUBETEK_CLIENT_CONTROLLER_LINK_ON_LOG("controllers unit: on removed from controller");
        remove_from_controller(id);
    }
    inline std::string controller_link_impl::controllers_unit_on_get_alias(unique_controller_id::const_reference id)
    {
        RUBETEK_CLIENT_CONTROLLER_LINK_ON_LOG("controllers unit: on get alias");
        if (project_manager_.has_project(id)) return project_manager_.alias(id);
        logger_.debug("on get alias: project not found");
        return "";
    }
    inline std::string controller_link_impl::controllers_unit_on_get_name(unique_controller_id::const_reference id)
    {
        RUBETEK_CLIENT_CONTROLLER_LINK_ON_LOG("controllers unit: on get name");
        if (project_manager_.has_project(id)) return project_manager_.get_project_info(id).name;
        logger_.warning("on get name: project not found");
        return "";
    }
    inline void controller_link_impl::controllers_unit_on_disconnected_active_controller()
    {
        RUBETEK_CLIENT_CONTROLLER_LINK_ON_LOG("controllers unit: on disconnected active controller");
        wait_for_connect_ = controllers_unit_.active_controller().id();
        active_controller_changed_connection_status();
    }
    inline void controller_link_impl::controllers_unit_on_active_controller_change_connection()
    {
        RUBETEK_CLIENT_CONTROLLER_LINK_ON_LOG("controllers unit: on active controller change connection");
        active_controller_changed_connection_status();
    }
    inline void controller_link_impl::controllers_unit_on_loading_progress(int updating_progress)
    {
        MSW_ASSERT(updating_progress_);
        *updating_progress_ = updating_progress;
    }
    inline void controller_link_impl::controllers_unit_on_rename(unique_controller_id::const_reference id)
    {
        RUBETEK_CLIENT_CONTROLLER_LINK_ON_LOG("controllers unit: on rename");
        std::string const name = controllers_unit_.make_info(id).name;
        project_manager_.update_name(id, name);
        if
        (
            (wait_for_connect_ && (*wait_for_connect_ == id))
            || (controllers_unit_.has_active_controller() && (controllers_unit_.active_controller().id() == id))
        )
        {
            callback_service_.on_controller_rename(name);
        }
        if (show_list_ == show_list::known)
        {
            self_show_known_controllers();
        }
        else
        {
            if (show_list_ == show_list::unknown)
            {
                self_show_unknown_controllers();
            }
            else
            {
                if (show_list_ == show_list::all)
                {
                    self_show_all_controllers();
                }
            }
        }
    }

    inline bool controller_link_impl::local_unit_on_check_project(unique_controller_id::const_reference id)
    {
        RUBETEK_CLIENT_CONTROLLER_LINK_ON_LOG("local unit: on check project");
        return project_manager_.has_project(id);
    }
    inline void controller_link_impl::local_unit_on_remove_project(unique_controller_id::const_reference id)
    {
        RUBETEK_CLIENT_CONTROLLER_LINK_ON_LOG("local unit: on remove project");
        remove_from_controller(id);
    }
    inline void controller_link_impl::local_unit_on_add_controller
    (
          unique_controller_id::const_reference id
        , local_link_selector*                  link_selector
        , bool                                  known
    )
    {
        RUBETEK_CLIENT_CONTROLLER_LINK_ON_LOG("local unit: on add controller");
        controllers_unit_.add_local_link(id, link_selector, known);
    }
    inline void controller_link_impl::local_unit_on_remove_controller(unique_controller_id::const_reference id)
    {
        RUBETEK_CLIENT_CONTROLLER_LINK_ON_LOG("local unit: on remove controller");
        if (controllers_unit_.has_controller(id)) controllers_unit_.remove_local_link(id);
    }

    inline void controller_link_impl::remote_unit_on_connection_status(server_connection_status connection_status)
    {
        RUBETEK_CLIENT_CONTROLLER_LINK_ON_LOG("remote unit: on connection status");
        if (connection_status == server_connection_status::connected)
        {
            std::list<unique_controller_id::type> projects_id = project_manager_.projects_id();
            for (unique_controller_id::const_reference id : projects_id)
                remote_unit_.open_channel(id);
        }
        callback_service_.on_server_connection_status(connection_status);
    }
    inline void controller_link_impl::remote_unit_on_channel_opened
    (
          unique_controller_id::const_reference id
        , remote_link_interface<>*              remote_link
        , std::string const&                    ip
        , unique_channel_id_type const&         channel_id
    )
    {
        RUBETEK_CLIENT_CONTROLLER_LINK_ON_LOG("remote unit: on channel opened");
        controllers_unit_.add_remote_link(id, remote_link, ip, channel_id);
    }
    inline void controller_link_impl::remote_unit_on_channel_closed(unique_channel_id_type id)
    {
        RUBETEK_CLIENT_CONTROLLER_LINK_ON_LOG("remote unit: on channel closed");
        controllers_unit_.remove_remote_link(id);
    }
    inline void controller_link_impl::remote_unit_on_directly_packet(server_packet_type packet_type, std::string const& packet)
    {
        RUBETEK_CLIENT_CONTROLLER_LINK_ON_LOG("remote unit: on server directly");
        callback_service_.on_server_directly(packet_type, packet);
    }
    inline void controller_link_impl::remote_unit_on_auxiliary_packet(std::string const& packet)
    {
        RUBETEK_CLIENT_CONTROLLER_LINK_ON_LOG("remote unit: on server auxiliary");
        callback_service_.on_server_auxiliary(packet);
    }
    inline void controller_link_impl::remote_unit_on_close_due_to_malfunction_proto()
    {
        RUBETEK_CLIENT_CONTROLLER_LINK_ON_LOG("remote unit: on close due to malfunction proto");
        callback_service_.on_update_required();
    }

    inline void controller_link_impl::active_controller_on_query_password(option_was_removed was_removed)
    {
        RUBETEK_CLIENT_CONTROLLER_LINK_ON_LOG("on query password");
        callback_service_.on_password_request(was_removed);
    }
    inline void controller_link_impl::active_controller_on_no_project()
    {
        RUBETEK_CLIENT_CONTROLLER_LINK_ON_LOG("on no project");
        controller_activated();
        callback_service_.on_project_error(project_error::no_on_controller);
    }
    inline void controller_link_impl::active_controller_on_no_required_resolution()
    {
        RUBETEK_CLIENT_CONTROLLER_LINK_ON_LOG("on no required resolution");
        controller_activated();
        callback_service_.on_project_error(project_error::no_required_resolution);
    }
    inline void controller_link_impl::active_controller_on_update_project()
    {
        RUBETEK_CLIENT_CONTROLLER_LINK_ON_LOG("on update project");
        controller_activated();
        updating_progress_ = msw::make_unique<int>(0);//////////updating_progress_ where updating_progress_.reset() ?
        callback_service_.on_project_updating();
    }
    inline void controller_link_impl::active_controller_on_loading_progress(int updating_progress)
    {
        MSW_ASSERT(updating_progress_);
        *updating_progress_ = updating_progress;
    }
    inline void controller_link_impl::active_controller_on_project(msw::range<msw::byte const> project, unsigned crc)
    {
        logger_.debug("on project");
        //logger_.debug("project content: ", msw::bytes_as_string(project.all()));
        if (controllers_unit_.has_active_controller())
        {
            auto id = controllers_unit_.active_controller_id();
            if (!project_manager_.has_project(id)) project_manager_.add_project(id, controllers_unit_.active_controller().demo_mode());
            project_manager_.update_project(id, crc, project);
        }
        else logger_.warning("on project: no current controller");
    }
    inline void controller_link_impl::active_controller_on_ready()
    {
        logger_.debug("on ready");
        controller_activated();
        if (!controllers_unit_.has_active_controller()) throw std::runtime_error("no active controller");
        auto project_files = project_manager_.get_project_files(controllers_unit_.active_controller_id());
        callback_service_.on_ready(project_files.project);
    }
    inline void controller_link_impl::active_controller_on_property(property_address address, property_list const& list)
    {
        RUBETEK_CLIENT_CONTROLLER_LINK_ON_LOG("on property");
        callback_service_.on_properties_received(address, list);
    }
    inline void controller_link_impl::active_controller_on_module_command(std::string const& cmd)
    {
        RUBETEK_CLIENT_CONTROLLER_LINK_ON_LOG("on module command");
        callback_service_.on_module_command(cmd);
    }

}}
