#pragma once

#include <memory>


namespace rubetek {
namespace utility {

	template <typename Data, typename Holder>
	struct private_box
	{
		typedef		private_box<Data, Holder> self;

					private_box	(self&&)		;
		self&		operator =	(self&&)		;

					private_box	(self const&)	;
		self&		operator =	(self const&)	;

	private:

					private_box	()				;
		explicit	private_box	(Data const&)	;
		explicit	private_box	(Data&&)		;

		friend Holder;
		Data data;
	};

}}


namespace rubetek {
namespace utility {

	template <typename Data, typename Holder>
	private_box<Data, Holder>::private_box(self&& rhs)
		: data(std::move(rhs.data))
	{}
	template <typename Data, typename Holder>
	typename private_box<Data, Holder>::self& private_box<Data, Holder>::operator = (self&& rhs)
	{
		data = std::move(rhs.data);
		return *this;
	}

	template <typename Data, typename Holder>
	private_box<Data, Holder>::private_box(self const& rhs)
		: data(rhs.data)
	{}
	template <typename Data, typename Holder>
	typename private_box<Data, Holder>::self& private_box<Data, Holder>::operator = (self const& rhs)
	{
		data = rhs.data;
		return *this;
	}

	template <typename Data, typename Holder>
	private_box<Data, Holder>::private_box()
		: data()
	{}
	template <typename Data, typename Holder>
	private_box<Data, Holder>::private_box(Data const& d)
		: data(d)
	{}
	template <typename Data, typename Holder>
	private_box<Data, Holder>::private_box(Data&& d)
		: data(std::move(d))
	{}

}}
