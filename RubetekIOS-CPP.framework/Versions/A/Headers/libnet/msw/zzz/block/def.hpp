#pragma once 

#include <msw/assert.hpp>
#include <msw/zzz/block/decl.hpp>
#include <msw/zzz/bit_ptr/fwd.hpp>


namespace msw {
namespace zzz {

    template <typename T>
    block<T>::block()
    {}
    template <typename T>
    block<T>::block(pointer first, pointer last)
        : first_(first)
        , last_(last)
    {
        MSW_ASSERT(check());
    }
    template <typename T>
    block<T>::block(pointer first, size_type size)
        : first_(first)
        , last_(first + size)
    {
        MSW_ASSERT(check_orientation());
    }
    template <typename T>
    block<T>::block(size_type size, pointer last)
        : first_(last - size)
        , last_(last)
    {
        MSW_ASSERT(check_orientation());
    }
    template <>
    inline block<bit>::block(size_type size, pointer last)
        : first_(last - size)
        , last_(last)
    {
        MSW_ASSERT(check_orientation());
    }

    template <typename T>
    block<T>::block(self_type const& other)
        : first_(other.first())
        , last_(other.last())
    {
        MSW_ASSERT(check());
    }
    template <typename T>
    typename block<T>::self_type& block<T>::operator = (self_type const& other)
    {
        first_ = other.first();
        last_ = other.last();
        return *this;
    }

    template <typename T>
    block<T>::block(self_type&& other)
        : first_(std::move(other.first_))
        , last_(std::move(other.last_))
    {}
    template <typename T>
    typename block<T>::self_type& block<T>::operator = (self_type&& other)
    {
        first_ = std::move(other.first_);
        last_ = std::move(other.last_);
        return *this;
    }

    template <typename T>
    template <typename U>
    block<T>::block(block<U> const& other)
        : first_(other.first())
        , last_(other.last())
    {
        MSW_ASSERT(check());
    }
    template <typename T>
    template <typename U>
    typename block<T>::self_type& block<T>::operator = (block<U> const& other)
    {
        first_ = other.first();
        last_ = other.last();
        return *this;
    }

    template <typename T>
    template <typename U>
    block<T>::block(block<U>&& other)
        : first_(std::move(other.first_))
        , last_(std::move(other.last_))
    {}
    template <typename T>
    template <typename U>
    typename block<T>::self_type& block<T>::operator = (block<U>&& other)
    {
        first_ = std::move(other.first_);
        last_ = std::move(other.last_);
        return *this;
    }

    template <typename T>
    void block<T>::swap(self_type& other)
    {
        first_.swap(other.first_);
        last_.swap(other.last_);
    }

    template <typename T>
    typename block<T>::size_type block<T>::size() const
    {
        return last() - first();
    }
    template <typename T>
    bool block<T>::empty() const
    {
        return last() == first();
    }
    template <typename T>
    bool block<T>::has_items() const
    {
        return last() != first();
    }

    template <typename T>
    typename block<T>::pointer block<T>::first() const
    {
        return first_;
    }
    template <typename T>
    typename block<T>::pointer block<T>::last() const
    {
        return last_;
    }

    template <typename T>
    typename block<T>::pointer block<T>::inc_first()
    {
        MSW_WARY_ASSERT(has_items());
        return ++first_;
    }
    template <typename T>
    typename block<T>::pointer block<T>::dec_last()
    {
        MSW_WARY_ASSERT(has_items());
        return --last_;
    }

    template <typename T>
    typename block<T>::pointer block<T>::inc_first(size_type s)
    {
        MSW_WARY_ASSERT(size() >= s);
        return first_ += s;
    }
    template <typename T>
    typename block<T>::pointer block<T>::dec_last(size_type s)
    {
        MSW_WARY_ASSERT(size() >= s);
        return last_ -= s;
    }

    template <typename T>
    void block<T>::set_first(pointer p)
    {
        first_ = p;
        MSW_ASSERT(check());
    }
    template <typename T>
    void block<T>::set_last(pointer p)
    {
        last_ = p;
        MSW_ASSERT(check());
    }

    template <typename T>
    template <typename U>
    block<U> block<T>::to_block() const
    {
        return block<typename redused<U>::value_type>(first_.template to_pointer<U>(), last_.template to_pointer<U>());
    }


    template <typename T>
    bool block<T>::check_orientation() const
    {
        return last_ >= first_;
    }
    template <typename T>
    bool block<T>::check() const
    {
        return check_orientation() && first().is_adjacent_to(last());
    }
    template <>
    inline bool block<bit>::check() const
    {
        return check_orientation();
    }
    template <>
    inline bool block<bit const>::check() const
    {
        return check_orientation();
    }

}}
