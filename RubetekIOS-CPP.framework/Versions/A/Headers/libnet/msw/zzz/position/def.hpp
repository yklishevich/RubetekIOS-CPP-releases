#pragma once

#include <msw/on_debug.hpp>
#include <msw/zzz/block/make.hpp>
#include <msw/zzz/range/decl.hpp>
#include <msw/zzz/position/decl.hpp>
#include <msw/zzz/bit_ptr/fwd.hpp>
#ifdef MSW_DEBUG
    #include <msw/zzz/validator/link/def.hpp>
#endif


namespace msw
{
    template <typename T>
    position<T>::position()
    {}

    template <typename T>
    position<T>::position(self_type const& other)
        : ptr_(other.ptr_)
        MSW_ON_DEBUG
        (
            , validator_(other.validator_)
        )
    {}
    template <typename T>
    typename position<T>::self_type& position<T>::operator = (self_type const& other)
    {
        ptr_ = other.ptr_;
        MSW_ON_DEBUG
        (
            validator_ = other.validator_;
        )
        return *this;
    }

    template <typename T>
    position<T>::position(self_type&& other)
        : ptr_(std::move(other.ptr_))
        MSW_ON_DEBUG
        (
            , validator_(std::move(other.validator_))
        )
    {}
    template <typename T>
    typename position<T>::self_type& position<T>::operator = (self_type&& other)
    {
        ptr_ = std::move(other.ptr_);
        MSW_ON_DEBUG
        (
            validator_ = std::move(other.validator_);
        )
        return *this;
    }

    template <typename T>
    template <typename U>
    position<T>::position(position<U> const& other)
        : ptr_(other.ptr_)
        MSW_ON_DEBUG
        (
            , validator_(other.validator_)
        )
    {}
    template <typename T>
    template <typename U>
    typename position<T>::self_type& position<T>::operator = (position<U> const& other)
    {
        ptr_ = other.ptr_;
        MSW_ON_DEBUG
        (
            validator_ = other.validator_;
        )
        return *this;
    }

    template <typename T>
    template <typename U>
    position<T>::position(position<U>&& other)
        : ptr_(std::move(other.ptr_))
        MSW_ON_DEBUG
        (
            , validator_(std::move(other.validator_))
        )
    {}
    template <typename T>
    template <typename U>
    typename position<T>::self_type& position<T>::operator = (position<U>&& other)
    {
        ptr_ = std::move(other.ptr_);
        MSW_ON_DEBUG
        (
            validator_ = std::move(other.validator_);
        )
        return *this;
    }

    template <typename T>
    typename position<T>::pointer position<T>::to_pointer() const
    {
        MSW_ASSERT(validator_.check());
        return ptr_;
    }

    template <typename T>
    template <typename U>
    typename position<T>::template redused<U>::type position<T>::to_position() const
    {
        return zzz::construct_position(ptr_.template to_pointer<U>() MSW_ON_DEBUG(, validator_));
    }

    template <typename T>
    template <typename U>
    typename position<T>::size_type position<T>::operator - (position<U> other) const
    {
        MSW_ASSERT(validator_.check());
        MSW_ASSERT(other.validator_.check());
        MSW_ASSERT(validator_.same_root(other.validator_));
        MSW_ASSERT(ptr_.is_adjacent_to(other.ptr_));
        return ptr_ - other.ptr_;
    }
    template <>
    template <typename U>
    typename position<bit>::size_type position<bit>::operator - (position<U> other) const
    {
        MSW_ASSERT(validator_.check());
        MSW_ASSERT(other.validator_.check());
        MSW_ASSERT(validator_.same_root(other.validator_));
        return ptr_ - other.ptr_;
    }
    template <>
    template <typename U>
    typename position<bit const>::size_type position<bit const>::operator - (position<U> other) const
    {
        MSW_ASSERT(validator_.check());
        MSW_ASSERT(other.validator_.check());
        MSW_ASSERT(validator_.same_root(other.validator_));
        return ptr_ - other.ptr_;
    }

    template <typename T>
    template <typename U>
    typename position<T>::template common<U>::range position<T>::operator + (position<U> other) const
    {
        MSW_ASSERT(validator_.check());
        MSW_ASSERT(other.validator_.check());
        MSW_ASSERT(validator_.same_root(other.validator_));
        MSW_ASSERT(ptr_.is_adjacent_to(other.ptr_));
        return zzz::construct_range(zzz::make_block(ptr_, other.ptr_) MSW_ON_DEBUG(, validator_));
    }
    template <>
    template <typename U>
    typename position<bit>::common<U>::range position<bit>::operator + (position<U> other) const
    {
        MSW_ASSERT(validator_.check());
        MSW_ASSERT(other.validator_.check());
        MSW_ASSERT(validator_.same_root(other.validator_));
        return zzz::construct_range(zzz::make_block(ptr_, other.ptr_) MSW_ON_DEBUG(, validator_));
    }
    template <>
    template <typename U>
    typename position<bit const>::common<U>::range position<bit const>::operator + (position<U> other) const
    {
        MSW_ASSERT(validator_.check());
        MSW_ASSERT(other.validator_.check());
        MSW_ASSERT(validator_.same_root(other.validator_));
        return zzz::construct_range(zzz::make_block(ptr_, other.ptr_) MSW_ON_DEBUG(, validator_));
    }

    template <typename T>
    bool position<T>::null() const
    {
        return ptr_.null();
    }
    template <typename T>
    bool position<T>::not_null() const
    {
        return ptr_.not_null();
    }

#define MSW_POSITION_OPERATOR_COMPARE(o) \
    template <typename T> \
    template <typename U> \
    bool position<T>::operator o (position<U> other) const \
    { \
        MSW_ASSERT(validator_.check()) \
        MSW_ASSERT(other.validator_.check()) \
        MSW_ASSERT(validator_.same_root(other.validator_)) \
        MSW_ASSERT(ptr_.is_adjacent_to(other.ptr_)) \
        return ptr_ o other.ptr_; \
    }
    MSW_POSITION_OPERATOR_COMPARE( == )
    MSW_POSITION_OPERATOR_COMPARE( != )
    MSW_POSITION_OPERATOR_COMPARE( <  )
    MSW_POSITION_OPERATOR_COMPARE( >  )
    MSW_POSITION_OPERATOR_COMPARE( <= )
    MSW_POSITION_OPERATOR_COMPARE( >= )
#undef MSW_POSITION_OPERATOR_COMPARE
#define MSW_POSITION_OPERATOR_COMPARE(o) \
    template <> \
    template <typename U> \
    bool position<bit>::operator o (position<U> other) const \
    { \
        MSW_ASSERT(validator_.check()) \
        MSW_ASSERT(other.validator_.check()) \
        MSW_ASSERT(validator_.same_root(other.validator_)) \
        return ptr_ o other.ptr_; \
    }
    MSW_POSITION_OPERATOR_COMPARE( == )
    MSW_POSITION_OPERATOR_COMPARE( != )
    MSW_POSITION_OPERATOR_COMPARE( <  )
    MSW_POSITION_OPERATOR_COMPARE( >  )
    MSW_POSITION_OPERATOR_COMPARE( <= )
    MSW_POSITION_OPERATOR_COMPARE( >= )
#undef MSW_POSITION_OPERATOR_COMPARE
#define MSW_POSITION_OPERATOR_COMPARE(o) \
    template <> \
    template <typename U> \
    bool position<bit const>::operator o (position<U> other) const \
    { \
        MSW_ASSERT(validator_.check()) \
        MSW_ASSERT(other.validator_.check()) \
        MSW_ASSERT(validator_.same_root(other.validator_)) \
        return ptr_ o other.ptr_; \
    }
    MSW_POSITION_OPERATOR_COMPARE( == )
    MSW_POSITION_OPERATOR_COMPARE( != )
    MSW_POSITION_OPERATOR_COMPARE( <  )
    MSW_POSITION_OPERATOR_COMPARE( >  )
    MSW_POSITION_OPERATOR_COMPARE( <= )
    MSW_POSITION_OPERATOR_COMPARE( >= )
#undef MSW_POSITION_OPERATOR_COMPARE

    template <typename T>
    template <typename Pointer MSW_ON_DEBUG(, typename Validator)>
    position<T>::position(Pointer p MSW_ON_DEBUG(, Validator&& validator))
        : ptr_(p)
        MSW_ON_DEBUG
        (
            , validator_(std::forward<Validator>(validator))
        )
    {}

}


namespace msw {
namespace zzz {

    template <typename Pointer MSW_ON_DEBUG(, typename Validator)>
    position<typename Pointer::value_type> construct_position(Pointer p MSW_ON_DEBUG(, Validator&& validator))
    {
        return position<typename Pointer::value_type>(p MSW_ON_DEBUG(, std::forward<Validator>(validator)));
    }

}}
