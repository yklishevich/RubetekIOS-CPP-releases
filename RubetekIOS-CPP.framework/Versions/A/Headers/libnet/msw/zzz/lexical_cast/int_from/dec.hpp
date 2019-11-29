#pragma once

#include <string>


namespace msw {
namespace zzz {

    template <typename T>
    struct int_from_dec_impl
    {
        static_assert(sizeof(T) < 4, "size of T must be less 4");

        T operator () (std::string const& s) const
        {
            return static_cast<T>(std::stoi(s));
        }
    };
    template <>
    struct int_from_dec_impl<int>
    {
        int operator () (std::string const& s) const
        {
            return std::stoi(s);
        }
    };
    template <>
    struct int_from_dec_impl<unsigned>
    {
        unsigned operator () (std::string const& s) const
        {
            return std::stoul(s);
        }
    };
    template <>
    struct int_from_dec_impl<long long>
    {
        long long operator () (std::string const& s) const
        {
            return std::stoll(s);
        }
    };
    template <>
    struct int_from_dec_impl<unsigned long long>
    {
        unsigned long long operator () (std::string const& s) const
        {
            return std::stoull(s);
        }
    };
}}
