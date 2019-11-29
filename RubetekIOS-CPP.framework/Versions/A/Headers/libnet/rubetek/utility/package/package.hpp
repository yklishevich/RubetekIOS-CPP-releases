#pragma once

#include <string>
#include <typeinfo>
#include <stdexcept>

#include <rubetek/utility/buffer.hpp>
#include <rubetek/utility/raw_memory.hpp>


namespace rubetek
{
    template <typename T>
    struct package;

    template <typename T>
    void pack(T const& v, buffer& buf)
    {
        package<T>::pack(v, buf);
    }
    template <typename T>
    buffer pack(T const& v)
    {
        buffer buf;
        package<T>::pack(v, buf);
        return buf;
    }
    template <typename T>
    bool can_unpack(buffer::const_iterator position, buffer::const_iterator end)
    {
        return package<T>::can_unpack(position, end);
    }
    template <typename T>
    T unpack(buffer::const_iterator& position, buffer::const_iterator end)
    {
        if (!package<T>::can_unpack(position, end)) throw std::runtime_error(std::string("can't unpack: <") + typeid(T).name() + ">");
        T v;
        position = package<T>::unpack(v, position, end);
        return v;
    }
    template <typename T>
    T unpack(buffer const& buf)
    {
        auto pos = buf.begin();
        return unpack<T>(pos, buf.end());
    }
    template <typename T>
    T unpack(utility::raw_memory const& raw_memory)
    {
        return unpack<T>(buffer(raw_memory.begin(), raw_memory.end()));
    }
}
