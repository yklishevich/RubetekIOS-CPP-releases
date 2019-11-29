#include <string>

#include <msw/make_range_native.hpp>
#include <msw/lexical_cast/bytes_as_string.hpp>

#include <rubetek/config.hpp>
#ifndef RUBETEK_HEADER_ONLY
#   include <rubetek/client/controller_link.hpp>
#endif
#include <rubetek/essence/property.hpp>
#include <rubetek/essence/unique_id/client.hpp>
#include <rubetek/essence/unique_id/controller.hpp>
#include <rubetek/client/controller_link_context.hpp>
#include <rubetek/client/impl/controller_link_impl.hpp>
#include <rubetek/client/controller_link_back_interface.hpp>


namespace rubetek {
namespace client  {

    RUBETEK_INLINE_OF_IMPL controller_link::controller_link
    (
          controller_link_back_interface*   back_interface
        , unique_client_id::const_reference client_id
        , controller_link_context const&    context
        , bool                              focus
        , bool                              disable_start_logic
        , bool                              disable_find_on_empty
    )
        : impl_
        (
            new controller_link_impl
            (
                  back_interface
                , client_id
                , context
                , focus
                , disable_start_logic
                , disable_find_on_empty
            )
        )
    {}
    RUBETEK_INLINE_OF_IMPL controller_link::~controller_link()
    {
        delete impl_;
    }

    RUBETEK_INLINE_OF_IMPL void controller_link::auto_connect_by_location(bool yes)
    {
        impl_->auto_connect_by_location(yes);
    }
    RUBETEK_INLINE_OF_IMPL bool controller_link::auto_connect_by_location() const
    {
        return impl_->auto_connect_by_location();
    }
    RUBETEK_INLINE_OF_IMPL void controller_link::auto_connect_to_recent(bool yes)
    {
        impl_->auto_connect_to_recent(yes);
    }
    RUBETEK_INLINE_OF_IMPL bool controller_link::auto_connect_to_recent() const
    {
        return impl_->auto_connect_to_recent();
    }

    RUBETEK_INLINE_OF_IMPL void controller_link::focus(bool yes)
    {
        impl_->focus(yes);
    }

    RUBETEK_INLINE_OF_IMPL void controller_link::show_unknown_controllers()
    {
        impl_->show_unknown_controllers();
    }
    RUBETEK_INLINE_OF_IMPL void controller_link::show_known_controllers()
    {
        impl_->show_known_controllers();
    }
    RUBETEK_INLINE_OF_IMPL void controller_link::show_all_controllers()
    {
        impl_->show_all_controllers();
    }
    RUBETEK_INLINE_OF_IMPL void controller_link::cancel_show_list()
    {
        impl_->cancel_show_list();
    }
    RUBETEK_INLINE_OF_IMPL void controller_link::cancel_find_controller()
    {
        impl_->cancel_find_controller();
    }

    RUBETEK_INLINE_OF_IMPL void controller_link::rename_controller(unique_controller_id::const_reference id, std::string name)
    {
        impl_->rename_controller(id, name);
    }
    RUBETEK_INLINE_OF_IMPL void controller_link::remove_controller(unique_controller_id::const_reference id)
    {
        impl_->remove_controller(id);
    }
    RUBETEK_INLINE_OF_IMPL void controller_link::remove_all_controllers()
    {
        impl_->remove_all_controllers();
    }

    RUBETEK_INLINE_OF_IMPL void controller_link::connect_to(unique_controller_id::const_reference id)
    {
        impl_->connect_to(id);
    }
    RUBETEK_INLINE_OF_IMPL void controller_link::disconnect()
    {
        impl_->disconnect();
    }
    RUBETEK_INLINE_OF_IMPL int controller_link::updating_progress() const
    {
        return impl_->updating_progress();
    }
    RUBETEK_INLINE_OF_IMPL void controller_link::refresh()
    {
        impl_->refresh();
    }
    RUBETEK_INLINE_OF_IMPL void controller_link::send_password(std::string password)
    {
        impl_->send_password(password);
    }
    RUBETEK_INLINE_OF_IMPL void controller_link::send_project(std::string const& project)
    {
        impl_->send_project(project);
    }
    RUBETEK_INLINE_OF_IMPL void controller_link::send_property(property_address address, property pt)
    {
        impl_->send_property(address, pt);
    }
    RUBETEK_INLINE_OF_IMPL void controller_link::send_property(property_address address, property_list list)
    {
        impl_->send_property(address, list);
    }
#ifndef RUBETEK_CONTROLLER_LINK_LIBRARY
    RUBETEK_INLINE_OF_IMPL void controller_link::send_command(msw::range<msw::byte const> cmd)
    {
        impl_->send_command(cmd);
    }
    RUBETEK_INLINE_OF_IMPL void controller_link::send_command(msw::byte const* data, msw::uint size)
    {
        send_command(msw::make_range_native(data, size));
    }
#else
    RUBETEK_INLINE_OF_IMPL void controller_link::send_command(unsigned char const* data, unsigned size)
    {
        send_command(std::string(data, data + size));
    }
#endif
    RUBETEK_INLINE_OF_IMPL void controller_link::send_command(std::string const& s)
    {
#ifndef RUBETEK_CONTROLLER_LINK_LIBRARY
        send_command(msw::make_range<msw::byte>(s));
#else
        impl_->send_command(msw::make_range<msw::byte>(s));
#endif
    }
    RUBETEK_INLINE_OF_IMPL void controller_link::send_server_directly(proto::PacketType::type type, std::string const& s)
    {
        impl_->send_server_directly(type, s);
    }
    RUBETEK_INLINE_OF_IMPL void controller_link::send_server_auxiliary(std::string const& s)
    {
        impl_->send_server_auxiliary(s);
    }
    RUBETEK_INLINE_OF_IMPL void controller_link::send_notification_id(std::string const& id)
    {
        impl_->send_notification_id(id);
    }

    RUBETEK_INLINE_OF_IMPL void controller_link::remove_client(unique_controller_id::const_reference id)
    {
        impl_->remove_client(id);
    }

    RUBETEK_INLINE_OF_IMPL void controller_link::synchronize_projects(account_projects const& lst)
    {
        impl_->synchronize_projects(lst);
    }

    RUBETEK_INLINE_OF_IMPL void controller_link::process_events()
    {
        impl_->process_events();
    }

    RUBETEK_INLINE_OF_IMPL std::list<unique_controller_id::type> controller_link::projects_id_list(std::string const& work_path)
    {
        return controller_link_impl::projects_id_list(work_path);
    }
}}
