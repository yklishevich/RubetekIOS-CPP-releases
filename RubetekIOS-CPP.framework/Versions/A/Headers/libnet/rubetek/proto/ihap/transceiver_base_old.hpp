#pragma once

#include <msw/buffer.hpp>
#include <msw/noncopyable.hpp>
#include <msw/callback/byte_range.hpp>

#include <rubetek/proto/ihap/ihap.hpp>
#include <rubetek/essence/property.hpp>
#include <rubetek/essence/device_properties.hpp>
#include <rubetek/utility/package/property.hpp>
#include <rubetek/utility/package/list.hpp>
#include <rubetek/utility/package/device_properties.hpp>


namespace rubetek
{
	struct ihap_transceiver_base_old
		: msw::noncopyable
	{
		typedef msw::callback::byte_const_range ihap_packet_ready;

		enum class packet_result
		{
			success
			, bad_size
			, unknown_command
		};

		explicit				ihap_transceiver_base_old		(ihap_packet_ready)								;

				void			process_ihap_packet				(msw::range<msw::byte const>)					;
				void			send_property					(property_address, property_list const&)		;
				void			send_device_properties			(device_properties_list const&)					;

	protected:
								~ihap_transceiver_base_old			() {}

				void			send_cmd						(ihap::cmd)										;
				void			send_packet						()												;
				bool			process_property				(msw::range<msw::byte const>)					;

	private:
		virtual	packet_result	internal_process_ihap_packet	(ihap::cmd, msw::range<msw::byte const>) = 0	;
		virtual	void			base_on_property				(property_address, property_list const&) = 0	;

		ihap_packet_ready		ihap_packet_ready_	;
	protected:
		msw::buffer<msw::byte>	packet_				;
	};
}


//#define RUBETEK_IHAP_ENABLE_PROPERTIES_LIST


namespace rubetek
{
	inline ihap_transceiver_base_old::ihap_transceiver_base_old(ihap_packet_ready ihap_packet_ready)
		: ihap_packet_ready_	(ihap_packet_ready)
		, packet_				(0, msw::KB)
	{}

	inline void ihap_transceiver_base_old::process_ihap_packet(msw::range<msw::byte const> packet)
	{
		if (packet.empty()) throw std::runtime_error("IHAP: packet is empty");
		ihap::cmd const cmd = packet.pop_front<ihap::cmd>();
		packet_result result = internal_process_ihap_packet(cmd, packet);
		if (result == packet_result::bad_size) throw std::runtime_error("IHAP: bad packet size");
		if (result == packet_result::unknown_command) throw std::runtime_error("IHAP: unknown command");
	}

	inline void ihap_transceiver_base_old::send_property(property_address address, property_list const& list)
	{
#ifdef RUBETEK_IHAP_ENABLE_PROPERTIES_LIST
		device_properties_list lst;
		lst.push_back({address, list});
		send_device_properties(lst);
#else
		packet_.clear();
		packet_.push_back(ihap::cmd::property);
		packet_.push_back(address);
		packet_.push_back(pack(list));
		send_packet();
#endif
	}
	inline void ihap_transceiver_base_old::send_device_properties(device_properties_list const& list)
	{
		packet_.clear();
		packet_.push_back(ihap::cmd::property);
		packet_.push_back(pack(list));
		send_packet();
	}

	inline void ihap_transceiver_base_old::send_cmd(ihap::cmd cmd)
	{
		ihap_packet_ready_(msw::make_range<msw::byte>(cmd));
	}
	inline void ihap_transceiver_base_old::send_packet()
	{
		MSW_ASSERT(packet_.has_items());
		ihap_packet_ready_(packet_.all());
		packet_.clear();
	}
	inline bool ihap_transceiver_base_old::process_property(msw::range<msw::byte const> packet)
	{
#ifdef RUBETEK_IHAP_ENABLE_PROPERTIES_LIST
		utility::raw_memory mem(packet.data().native(), packet.size().count());
		device_properties_list const list = unpack<device_properties_list>(mem);
		for (device_properties const& properties : list)
			base_on_property(properties.address, properties.properties);
#else
		if (packet.size() < msw::size<property_address>(1)) throw std::runtime_error("IHAP: bad property packet len");
		property_address const address = packet.pop_front<property_address>();
		utility::raw_memory mem(packet.data().native(), packet.size().count());
		property_list const list = unpack<property_list>(mem);
		base_on_property(address, list);
#endif
		return true;
	}
}
