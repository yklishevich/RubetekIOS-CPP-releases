#pragma once

#include <cstddef>


namespace rubetek {
namespace utility {

	struct raw_memory
	{
		typedef unsigned char const*	data_type;
		typedef unsigned				size_type;

													raw_memory	()									;
													raw_memory	(data_type, std::size_t)			;
		template <typename Container>				raw_memory	(Container const&)					;

										size_type	size		()							const	;
										data_type	data		()							const	;
										data_type	begin		()							const	;
										data_type	end			()							const	;

	private:
		data_type data_;
		size_type size_;
	};

}}


namespace rubetek {
namespace utility {

	inline raw_memory::raw_memory()
		: data_()
		, size_()
	{}
	inline raw_memory::raw_memory(data_type data, std::size_t size)
		: data_(data)
		, size_(static_cast<size_type>(size))
	{}
	template <typename Container>
	raw_memory::raw_memory(Container const& c)
		: data_(c.data())
		, size_(static_cast<size_type>(c.size()))
	{}

	inline raw_memory::data_type raw_memory::data() const
	{
		return data_;
	}
	inline raw_memory::size_type raw_memory::size() const
	{
		return size_;
	}
	inline raw_memory::data_type raw_memory::begin() const
	{
		return data();
	}
	inline raw_memory::data_type raw_memory::end() const
	{
		return data() + size();
	}

}}
