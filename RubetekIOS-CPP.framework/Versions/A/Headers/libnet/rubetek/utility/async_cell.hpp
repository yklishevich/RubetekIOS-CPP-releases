#pragma once

#include <mutex>
#include <memory>
#include <condition_variable>


namespace rubetek {
namespace utility {

	template <typename T>
	struct async_cell
		: utility::noncopyable
	{
		typedef T value_type;

		template <typename U>	void		put		(U&&)	;

								value_type	eject	()		;

	private:

		std::mutex					m_		;
		std::condition_variable		cv_		;
		std::unique_ptr<value_type>	value_	;
	};

}}


namespace rubetek {
namespace utility {

	template <typename T>
	template <typename U>
	void async_cell<T>::put(U&& value)
	{
		std::unique_lock<std::mutex> lock(m_);
		value_.reset(new value_type(std::forward<U>(value)));
		cv_.notify_one();
	}

	template <typename T>
	typename async_cell<T>::value_type async_cell<T>::eject()
	{
		std::unique_lock<std::mutex> lock(m_);
		cv_.wait(lock, [this] { return static_cast<bool>(value_); });
		value_type value(std::move(*value_));
		value_.reset();
		return std::move(value);
	}

}}
