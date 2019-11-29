#pragma once

#include <msw/int.hpp>


namespace rubetek
{
	typedef msw::uint	unique_object_id;
	typedef msw::uint64	unify_object_id	;

	template <typename Id>
	Id get_object_id(void const* obj)
	{
		static_assert(sizeof(Id) >= sizeof(void*), "sizeof Id must be not less sizeof pointer");
		return reinterpret_cast<Id>(obj);
	}

	inline unique_object_id get_unique_object_id(void const* obj)
	{
		return get_object_id<unique_object_id>(obj);
	}
	inline unify_object_id get_unify_object_id(void const* obj)
	{
		return get_object_id<unify_object_id>(obj);
	}
}
