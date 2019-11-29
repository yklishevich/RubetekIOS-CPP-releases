#pragma once

#include <list>
#include <mutex>
#include <functional>

#include <rubetek/utility/buffer.hpp>


namespace rubetek {
namespace utility {

	template <typename Id>
	struct lock_buffer_queue
	{
		typedef std::function<void(Id, buffer const&)> buffer_ready_t;

		void push	(Id id, buffer&&)		;
		void eject	(buffer_ready_t);

	private:
		std::list<std::pair<Id, buffer>>	list_	;
		std::mutex									mutex_	;
	};

}}


namespace rubetek {
namespace utility {

	template <typename Id>
	inline void lock_buffer_queue<Id>::push(Id id, buffer&& buf)
	{
		std::lock_guard<std::mutex> lock(mutex_);
		list_.push_back(std::make_pair(id, std::move(buf)));
	}
	template <typename Id>
	inline void lock_buffer_queue<Id>::eject(buffer_ready_t buffer_ready)
	{
		std::lock_guard<std::mutex> lock(mutex_);
		for (auto const& buf : list_)
			buffer_ready(buf.first, buf.second);
		list_.clear();
	}

}}
