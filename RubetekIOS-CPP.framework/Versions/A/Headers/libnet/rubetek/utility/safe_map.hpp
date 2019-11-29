#pragma once

#include <string>
#include <map>


namespace rubetek {
namespace utility {

	template <typename Key, typename T>
	struct safe_map
	{
		explicit	safe_map(std::string const& exception_prefix)			;

					safe_map(safe_map&&)									;

		void		add		(Key const&, T const&)							;
		void		add		(Key const&, T&&)								;
		T&			get		(Key const&)									;
		T const&	get		(Key const&)							const	;
		bool		check	(Key const& key)						const	;

	protected:
		safe_map<Key, T>& operator = (safe_map<Key, T> const&) {}

		std::string const	exception_prefix_	;
		std::map<Key, T>	map_				;
	};

}}


namespace rubetek {
namespace utility {

	template <typename Key, typename T>
	safe_map<Key, T>::safe_map(std::string const& exception_prefix)
		: exception_prefix_(exception_prefix)
	{}

	template <typename Key, typename T>
	safe_map<Key, T>::safe_map(safe_map&& other)
		: exception_prefix_(other.exception_prefix_)
		, map_(std::move(other.map_))
	{}

	template <typename Key, typename T>
	void safe_map<Key, T>::add(Key const& key, T const& value)
	{
		auto it = map_.find(key);
		if (it != map_.end()) throw std::runtime_error(exception_prefix_ + ": Key already exist");
		map_.insert(std::make_pair(key, value));
	}

	template <typename Key, typename T>
	void safe_map<Key, T>::add(Key const& key, T&& value)
	{
		auto it = map_.find(key);
		if (it != map_.end()) throw std::runtime_error(exception_prefix_ + ": Key already exist");
		map_.insert(std::make_pair(key, std::move(value)));
	}

	template <typename Key, typename T>
	T& safe_map<Key, T>::get(Key const& key)
	{
		auto it = map_.find(key);
		if (it == map_.end()) throw std::runtime_error(exception_prefix_ + ": Key not found");
		return it->second;
	}

	template <typename Key, typename T>
	T const& safe_map<Key, T>::get(Key const& key) const
	{
		auto it = map_.find(key);
		if (it == map_.end()) throw std::runtime_error(exception_prefix_ + ": Key not found");
		return it->second;
	}

	template <typename Key, typename T>
	bool safe_map<Key, T>::check(Key const& key) const
	{
		return map_.find(key) != map_.end();
	}

}}