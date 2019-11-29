#pragma once

#include <msw/plain_types.hpp>


namespace rubetek {
namespace ihap {

	enum class cmd : msw::byte
	{
		controller_info
		, remove
		, activate
		, inactivate
		, password
		, no_project
		, no_required_resolution
		, project_size
		, large_begin
		, large_block
		, large_end
		, loading_cancel
		, project
		, resources
		, ready
		, property
		, refresh
	};

}}
