#include <mutex>
#include <string>
#include <functional>

#include <rubetek/config.hpp>

#include <msw/std/mutex.hpp>

#include <rubetek/log.hpp>
#include <rubetek/essence/property.hpp>
#include <rubetek/client/project_error.hpp>
#include <rubetek/client/controller_info.hpp>
#include <rubetek/client/controller_error.hpp>
#include <rubetek/essence/unique_id/controller.hpp>
#include <rubetek/client/server_connection_status.hpp>
#include <rubetek/client/controller_link_back_interface.hpp>
#ifndef RUBETEK_HEADER_ONLY
#   include <rubetek/client/client/callback_service.hpp>
#endif


#ifdef RUBETEK_CLIENT_CALLBACK_SERVICE_LOG
#   define RUBETEK_CLIENT_CALLBACK_SERVICE_ON_LOG(...) logger_.debug(__VA_ARGS__);
#else
#   define RUBETEK_CLIENT_CALLBACK_SERVICE_ON_LOG(...)
#endif


namespace rubetek {
namespace client  {

    inline callback_service::callback_service(controller_link_back_interface* back_interface)
        : logger_         (
                                "callback service"
#ifdef RUBETEK_CLIENT_CALLBACK_SERVICE_LOG
                              , log::level::debug
#endif
                          )
        , back_interface_ ( back_interface )
    {}

#define RUBETEK_CLIENT_CALLBACK_SERVICE_POST(Fn)\
    inline void callback_service::Fn()\
    {\
        RUBETEK_CLIENT_CALLBACK_SERVICE_ON_LOG("post ", #Fn);\
        msw::unique_lock<mutex_type> lock(mutex_);\
        fn_##Fn() = [this]()\
        {\
            back_interface_->Fn();\
        };\
    }
#define RUBETEK_CLIENT_CALLBACK_SERVICE_POST_1(Fn, T)\
    inline void callback_service::Fn(T v)\
    {\
        RUBETEK_CLIENT_CALLBACK_SERVICE_ON_LOG("post ", #Fn);\
        msw::unique_lock<mutex_type> lock(mutex_);\
        fn_##Fn() = [this, v]()\
        {\
            back_interface_->Fn(v);\
        };\
    }
#define RUBETEK_CLIENT_CALLBACK_SERVICE_POST_2(Fn, T, T2)\
    inline void callback_service::Fn(T v, T2 v2)\
    {\
        RUBETEK_CLIENT_CALLBACK_SERVICE_ON_LOG("post ", #Fn);\
        msw::unique_lock<mutex_type> lock(mutex_);\
        fn_##Fn() = [this, v, v2]()\
        {\
            back_interface_->Fn(v, v2);\
        };\
    }
    RUBETEK_CLIENT_CALLBACK_SERVICE_POST   (on_find_controller                                                               )
    RUBETEK_CLIENT_CALLBACK_SERVICE_POST_1 (on_unknown_controllers          , unknown_controller_list                        )
    RUBETEK_CLIENT_CALLBACK_SERVICE_POST_1 (on_known_controllers            , known_controller_list                          )
    RUBETEK_CLIENT_CALLBACK_SERVICE_POST_2 (on_all_controllers              , known_controller_list, unknown_controller_list )
    RUBETEK_CLIENT_CALLBACK_SERVICE_POST_1 (on_notify_about_location        , known_controller_list                          )
    RUBETEK_CLIENT_CALLBACK_SERVICE_POST_1 (on_server_connection_status     , server_connection_status                       )
    RUBETEK_CLIENT_CALLBACK_SERVICE_POST_2 (on_connecting_to_controller     , controller_info, std::string                   )
    RUBETEK_CLIENT_CALLBACK_SERVICE_POST_2 (on_controller_connection_status , controller_availability, std::string           )
    RUBETEK_CLIENT_CALLBACK_SERVICE_POST_1 (on_controller_rename            , std::string const&                             )
    RUBETEK_CLIENT_CALLBACK_SERVICE_POST_1 (on_controller_error             , controller_error                               )
    RUBETEK_CLIENT_CALLBACK_SERVICE_POST_1 (on_password_request             , option_was_removed                             )
    RUBETEK_CLIENT_CALLBACK_SERVICE_POST_1 (on_project_error                , project_error                                  )
    RUBETEK_CLIENT_CALLBACK_SERVICE_POST   (on_project_updating                                                              )
    RUBETEK_CLIENT_CALLBACK_SERVICE_POST_1 (on_ready                        , std::string                                    )
    inline void callback_service::on_properties_received(property_address address, property_list list)
    {
        RUBETEK_CLIENT_CALLBACK_SERVICE_ON_LOG("post on_properties_received");
        msw::unique_lock<mutex_type> lock(mutex_);
        property_blocks_.push({address, list});
    }
    inline void callback_service::on_module_command(std::string const& cmd)
    {
        RUBETEK_CLIENT_CALLBACK_SERVICE_ON_LOG("post on_module_command");
        msw::unique_lock<mutex_type> lock(mutex_);
        module_commands_.push(cmd);
    }
    inline void callback_service::on_server_directly(server_packet_type packet_type, std::string const& packet)
    {
        RUBETEK_CLIENT_CALLBACK_SERVICE_ON_LOG("post on_server_directly");
        msw::unique_lock<mutex_type> lock(mutex_);
        server_directly_packets_.push({packet_type, packet});
    }
    inline void callback_service::on_server_auxiliary(std::string const& packet)
    {
        RUBETEK_CLIENT_CALLBACK_SERVICE_ON_LOG("post on_server_auxiliary");
        msw::unique_lock<mutex_type> lock(mutex_);
        server_auxiliary_packets_.push(packet);
    }
    RUBETEK_CLIENT_CALLBACK_SERVICE_POST (on_update_required)
#undef RUBETEK_CLIENT_CALLBACK_SERVICE_POST_2
#undef RUBETEK_CLIENT_CALLBACK_SERVICE_POST_1
#undef RUBETEK_CLIENT_CALLBACK_SERVICE_POST

    inline void callback_service::run()
    {
        //RUBETEK_CLIENT_CALLBACK_SERVICE_ON_LOG("run");
        msw::unique_lock<mutex_type> lock(mutex_);
        for (auto& p : events_)
        {
            if (p.second)
            {
                RUBETEK_CLIENT_CALLBACK_SERVICE_ON_LOG(p.first);
                event e = std::move(p.second);
#ifdef MSW_NO_MOVE_STD_FUNCTION
                p.second = nullptr;
#endif
                e();
                RUBETEK_CLIENT_CALLBACK_SERVICE_ON_LOG(p.first, " exit");
            }
        }
        while (!property_blocks_.empty())
        {
            RUBETEK_CLIENT_CALLBACK_SERVICE_ON_LOG("on_properties_received");
            back_interface_->on_properties_received(property_blocks_.front().address, property_blocks_.front().list);
            RUBETEK_CLIENT_CALLBACK_SERVICE_ON_LOG("on_properties_received", " exit");
            property_blocks_.pop();
        }
        while (!module_commands_.empty())
        {
            RUBETEK_CLIENT_CALLBACK_SERVICE_ON_LOG("on_module_command");
            back_interface_->on_module_command(module_commands_.front());
            RUBETEK_CLIENT_CALLBACK_SERVICE_ON_LOG("on_module_command", " exit");
            module_commands_.pop();
        }
        while (!server_directly_packets_.empty())
        {
            RUBETEK_CLIENT_CALLBACK_SERVICE_ON_LOG("on_server_directly");
            back_interface_->on_server_directly(server_directly_packets_.front().first, server_directly_packets_.front().second);
            RUBETEK_CLIENT_CALLBACK_SERVICE_ON_LOG("on_server_directly", " exit");
            server_directly_packets_.pop();
        }
        while (!server_auxiliary_packets_.empty())
        {
            RUBETEK_CLIENT_CALLBACK_SERVICE_ON_LOG("on_server_auxiliary");
            back_interface_->on_server_auxiliary(server_auxiliary_packets_.front());
            RUBETEK_CLIENT_CALLBACK_SERVICE_ON_LOG("on_server_auxiliary", " exit");
            server_auxiliary_packets_.pop();
        }
        //RUBETEK_CLIENT_CALLBACK_SERVICE_ON_LOG("run exit");
    }

    inline void callback_service::cancel_on_properties_received()
    {
        RUBETEK_CLIENT_CALLBACK_SERVICE_ON_LOG("cancel_on_properties_received");
        msw::unique_lock<mutex_type> lock(mutex_);
        decltype(property_blocks_) tmp;
        property_blocks_.swap(tmp);
    }

#define RUBETEK_CLIENT_CALLBACK_SERVICE_FN(Index, Fn)\
    inline void callback_service::cancel_##Fn()\
    {\
        RUBETEK_CLIENT_CALLBACK_SERVICE_ON_LOG("cancel_"#Fn);\
        msw::unique_lock<mutex_type> lock(mutex_);\
        events_[Index].second = nullptr;\
    }\
    inline callback_service::event& callback_service::fn_##Fn()\
    {\
        RUBETEK_CLIENT_CALLBACK_SERVICE_ON_LOG("fn_"#Fn);\
        events_[Index].first = #Fn;\
        return events_[Index].second;\
    }
    RUBETEK_CLIENT_CALLBACK_SERVICE_FN(0  , on_find_controller              )
    RUBETEK_CLIENT_CALLBACK_SERVICE_FN(1  , on_unknown_controllers          )
    RUBETEK_CLIENT_CALLBACK_SERVICE_FN(2  , on_known_controllers            )
    RUBETEK_CLIENT_CALLBACK_SERVICE_FN(3  , on_all_controllers              )
    RUBETEK_CLIENT_CALLBACK_SERVICE_FN(4  , on_notify_about_location        )
    RUBETEK_CLIENT_CALLBACK_SERVICE_FN(5  , on_server_connection_status     )
    RUBETEK_CLIENT_CALLBACK_SERVICE_FN(6  , on_connecting_to_controller     )
    RUBETEK_CLIENT_CALLBACK_SERVICE_FN(7  , on_controller_connection_status )
    RUBETEK_CLIENT_CALLBACK_SERVICE_FN(8  , on_controller_rename            )
    RUBETEK_CLIENT_CALLBACK_SERVICE_FN(9  , on_controller_error             )
    RUBETEK_CLIENT_CALLBACK_SERVICE_FN(10 , on_password_request             )
    RUBETEK_CLIENT_CALLBACK_SERVICE_FN(11 , on_project_error                )
    RUBETEK_CLIENT_CALLBACK_SERVICE_FN(12 , on_project_updating             )
    RUBETEK_CLIENT_CALLBACK_SERVICE_FN(13 , on_ready                        )
    RUBETEK_CLIENT_CALLBACK_SERVICE_FN(14 , on_update_required              )
#undef RUBETEK_CLIENT_CALLBACK_SERVICE_FN

}}
