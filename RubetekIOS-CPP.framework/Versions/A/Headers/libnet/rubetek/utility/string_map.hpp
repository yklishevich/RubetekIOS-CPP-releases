#pragma once

#include <string>
#include <stdexcept>
#include <unordered_map>


namespace rubetek {
namespace utility {

	template <typename T>
	struct string_map
	{
		explicit	string_map	(std::string const& exception_prefix)			;

		void		add			(std::string const& name, T const&)				;
		bool		has			(std::string const& name)				const	;
		T&			get			(std::string const& name)						;
		T const&	get			(std::string const& name)				const	;
		template <typename Fn>
		void		for_each	(Fn)											;
		template <typename Fn>
		void		for_each	(Fn)									const	;

	protected:
		string_map<T>& operator = (string_map<T> const&) {}

		std::string const					exception_prefix_	;
		std::unordered_map<std::string, T>	map_				;
	};

}}


namespace rubetek {
namespace utility {

	template <typename T>
	string_map<T>::string_map(std::string const& exception_prefix)
		: exception_prefix_(exception_prefix)
	{}

	template <typename T>
	void string_map<T>::add(std::string const& name, T const& value)
	{
		auto it = map_.find(name);
		if (it != map_.end()) throw std::runtime_error(exception_prefix_ + " <" + name + "> already exist");
		map_.insert(std::make_pair(name, value));
	}
	template <typename T>
	bool string_map<T>::has(std::string const& name) const
	{
		return map_.find(name) != map_.end();
	}
	template <typename T>
	T& string_map<T>::get(std::string const& name)
	{
		auto it = map_.find(name);
		if (it == map_.end()) throw std::runtime_error(exception_prefix_ + " <" + name + "> not found");
		return it->second;
	}
	template <typename T>
	T const& string_map<T>::get(std::string const& name) const
	{
		auto it = map_.find(name);
		if (it == map_.end()) throw std::runtime_error(exception_prefix_ + " <" + name + "> not found");
		return it->second;
	}
	template <typename T>
	template <typename Fn>
	void string_map<T>::for_each(Fn fn)
	{
		for (auto& v : map_) fn(v);
	}
	template <typename T>
	template <typename Fn>
	void string_map<T>::for_each(Fn fn) const
	{
		for (auto const& v : map_) fn(v);
	}

}}
