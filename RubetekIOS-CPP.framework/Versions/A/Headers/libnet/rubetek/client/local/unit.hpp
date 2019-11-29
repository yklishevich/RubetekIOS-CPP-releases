#pragma once

#include <msw/noncopyable.hpp>
#include <msw/include/boost/asio.hpp>
#include <msw/asio/socket_options.hpp>
#include <msw/throw_runtime_error.hpp>
#include <msw/lexical_cast/network/endpoint/to_string.hpp>

#include <rubetek/log.hpp>
#include <rubetek/local_link/set.hpp>
#include <rubetek/client/local/beacon.hpp>
#include <rubetek/client/local/session.hpp>
#include <rubetek/socket/ihlp_session_set.hpp>
#include <rubetek/socket/tcp_connector_set.hpp>
#include <rubetek/essence/unique_id/client.hpp>
#include <rubetek/essence/unique_id/controller.hpp>
#include <rubetek/client/local/unit_back_interface.hpp>


namespace rubetek {
namespace client {

	struct local_unit
		: msw::noncopyable
	{
		local_unit	(
						boost::asio::io_service&
						, unique_client_id::const_reference
						, local_unit_back_interface*
					)										;

	private:

		logger                                     const logger_            ;
		unique_client_id::type                     const client_id_         ;
		local_unit_back_interface*                 const back_interface_    ;
		local_link_set<unique_controller_id::type>       local_links_       ;
		ihlp_session_set<local_controller_session>       ihlp_session_set_  ;
		tcp_connector_set                                tcp_connector_set_ ;
		beacon                                           beacon_            ;
	};

}}


namespace rubetek {
namespace client {

	inline local_unit::local_unit
		(
			boost::asio::io_service&            io_service
			, unique_client_id::const_reference client_id
			, local_unit_back_interface*        back_interface
		)
		: logger_         ("local unit", log::level::info)
		, client_id_      (client_id)
		, back_interface_ (back_interface)
		, local_links_
		(
			[this](unique_controller_id::const_reference id, local_link_selector* link_selector)
			{
				logger_.info("added controller: ", id);
				bool const i_have_you = back_interface_->local_unit_on_check_project(id);
				link_selector->set_on_received([this, id, link_selector, i_have_you](msw::range<msw::byte const> packet)
				{
					bool const he_has_me = packet.as<bool>();
					if (i_have_you && !he_has_me) back_interface_->local_unit_on_remove_project(id);
					back_interface_->local_unit_on_add_controller(id, link_selector, he_has_me);
				});
				link_selector->send(msw::make_range<msw::byte>(i_have_you));
			}
			, [this](unique_controller_id::const_reference id)
			{
				logger_.info("removed controller: ", id);
				back_interface_->local_unit_on_remove_controller(id);
			}
		)
		, ihlp_session_set_	 (io_service)
		, tcp_connector_set_ (io_service)
		, beacon_
		(
			io_service
			, [this](boost::asio::ip::tcp::endpoint endpoint, bool novel, bool novel32)
			{
				try
				{
				for (auto const& connector : tcp_connector_set_)
					if (connector->endpoint() == endpoint) return;
				}
				catch (std::exception const& e)
				{
					msw::throw_runtime_error("########## local_unit fail: ", e.what());
				}

				try
				{
				for (auto const& session : ihlp_session_set_)
					if (session->has_remote_endpoint() && (session->remote_endpoint() == endpoint)) return;
				}
				catch (std::exception const& e)
				{
					msw::throw_runtime_error("########## local_unit fail-2: ", e.what());
				}

				tcp_connector_set_.make
				(
					endpoint
					, [this, novel, novel32](boost::asio::ip::tcp::socket socket)
					{
						std::string const socket_name = msw::network_endpoint_to_string(socket.remote_endpoint());
						std::string const ip_address = socket.remote_endpoint().address().to_string();
						msw::socket_options<boost::asio::ip::tcp::socket>(socket).receive_buffer_size(msw::MB);
						ihlp_session_set_.make
						(
							std::move(socket)
							, ihlp::timeouts
							({
								msw::chrono::seconds(5)
								, msw::chrono::seconds(20)
							})
							, socket_name
							, client_id_
							, ip_address
							, local_links_
							, novel
							, novel32
						);
					}
				);
			}
		)
	{}

}}
