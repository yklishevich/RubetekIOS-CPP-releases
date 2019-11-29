#pragma once

#include <unordered_set>

#include <msw/config.hpp>
#include <msw/noncopyable.hpp>
#include <msw/dimension/trigger.hpp>

#include <rubetek/socket/tcp_connector.hpp>


namespace rubetek
{
	struct tcp_connector_set
		: msw::noncopyable
	{
		typedef std::unordered_set<tcp_connector::ptr>	tcp_connectors	;
		typedef tcp_connector::on_connected				on_connected	;
		typedef tcp_connectors::iterator				iterator		;
		typedef tcp_connectors::const_iterator			const_iterator	;

		explicit		tcp_connector_set	(boost::asio::io_service&)								;
						~tcp_connector_set	()														;

		void			make				(boost::asio::ip::tcp::endpoint, on_connected)			;

		iterator		begin				()														;
		const_iterator	begin				()												const	;
		iterator		end					()														;
		const_iterator	end					()												const	;

	private:
#ifdef MSW_NO_LAMBDA
		void		on_close				(tcp_connector::ptr)									;
#endif

		msw::trigger				destructurization_	;
		boost::asio::io_service&	io_service_			;
		tcp_connectors				tcp_connectors_		;
	};
}


namespace rubetek
{
	inline tcp_connector_set::tcp_connector_set(boost::asio::io_service& io_service)
		: destructurization_(false)
		, io_service_		(io_service)
	{}
	inline tcp_connector_set::~tcp_connector_set()
	{
		destructurization_.set();
		for (tcp_connector::ptr const& ptr : tcp_connectors_)
			ptr->close();
	}

	inline void tcp_connector_set::make(boost::asio::ip::tcp::endpoint endpoint, on_connected on_connected)
	{
		tcp_connectors_.insert(tcp_connector::make
			(
				io_service_
				, endpoint
				, on_connected
#ifdef MSW_NO_LAMBDA
				, std::bind(&tcp_connector_set::on_close, this, std::placeholders::_1)
#else
				, [this](tcp_connector::ptr ptr)
				{
					if (!destructurization_) tcp_connectors_.erase(ptr);
				}
			)
#endif
		);
	}

	inline tcp_connector_set::iterator tcp_connector_set::begin()
	{
		return tcp_connectors_.begin();
	}
	inline tcp_connector_set::const_iterator tcp_connector_set::begin() const
	{
		return tcp_connectors_.begin();
	}
	inline tcp_connector_set::iterator tcp_connector_set::end()
	{
		return tcp_connectors_.end();
	}
	inline tcp_connector_set::const_iterator tcp_connector_set::end() const
	{
		return tcp_connectors_.end();
	}

#ifdef MSW_NO_LAMBDA
	inline void tcp_connector_set::on_close(tcp_connector::ptr)
	{
		tcp_connector_.reset();
	}
#endif
}
