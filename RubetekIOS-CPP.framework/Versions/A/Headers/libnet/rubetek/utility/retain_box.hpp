#pragma once

#include <memory>


namespace rubetek {
namespace utility {

	template <typename T>
	struct retain_box
	{
		typedef retain_box<T>	self_type	;
		typedef T				value_type	;

							retain_box	()							;

							retain_box	(self_type const&)			;
		self_type&			operator =	(self_type const&)			;

							retain_box	(self_type&&)				;
		self_type&			operator =	(self_type&&)				;

		explicit			retain_box	(value_type const&)			;
		self_type&			operator =	(value_type const&)			;

		explicit			retain_box	(value_type&&)				;
		self_type&			operator =	(value_type&&)				;

		value_type const&	get			()					const	;
		value_type&			get			()							;
		bool				empty		()					const	;
		void				clear		()							;

	private:
		std::unique_ptr<value_type> value_;
	};

}}


namespace rubetek {
namespace utility {

	template <typename T>
	retain_box<T>::retain_box()
	{}

	template <typename T>
	retain_box<T>::retain_box(self_type const& rhs)
		: value_(new value_type(rhs.get()))
	{}
	template <typename T>
	typename retain_box<T>::self_type& retain_box<T>::operator = (self_type const& rhs)
	{
		value_.reset(new value_type());
		*value_ = rhs.get();
		return *this;
	}

	template <typename T>
	retain_box<T>::retain_box(self_type&& rhs)
		: value_(std::move(rhs.value_))
	{}
	template <typename T>
	typename retain_box<T>::self_type& retain_box<T>::operator = (self_type&& rhs)
	{
		value_ = std::move(rhs.value_);
		return *this;
	}

	template <typename T>
	retain_box<T>::retain_box(value_type const& value)
		: value_(new value_type(value))
	{}
	template <typename T>
	typename retain_box<T>::self_type& retain_box<T>::operator = (value_type const& value)
	{
		value_.reset(new value_type(value));
		return *this;
	}

	template <typename T>
	retain_box<T>::retain_box(value_type&& value)
		: value_(new value_type(value))
	{}
	template <typename T>
	typename retain_box<T>::self_type& retain_box<T>::operator = (value_type&& value)
	{
		value_.reset(new value_type(value));
		return *this;
	}

	template <typename T>
	typename retain_box<T>::value_type const& retain_box<T>::get() const
	{
		assert(!empty());
		return *value_;
	}
	template <typename T>
	typename retain_box<T>::value_type& retain_box<T>::get()
	{
		assert(!empty());
		return *value_;
	}

	template <typename T>
	bool retain_box<T>::empty() const
	{
		return !value_;
	}

	template <typename T>
	void retain_box<T>::clear()
	{
		value_.reset();
	}

}}
