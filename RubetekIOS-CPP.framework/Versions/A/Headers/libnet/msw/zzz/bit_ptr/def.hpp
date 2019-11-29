#pragma once

#include <msw/int.hpp>
#include <msw/assert.hpp>
#include <msw/zzz/ptr/def.hpp>
#include <msw/zzz/bit_ptr/decl.hpp>


namespace msw
{
    template <typename T>
    bit_ptr_t<T>::bit_ptr_t()
        : base_   ( nullptr )
        , offset_ ( 0       )
    {}
    template <typename T>
    bit_ptr_t<T>::bit_ptr_t(base_pointer base, offset_type offset)
        : base_   ( base   )
        , offset_ ( offset )
    {}

    template <typename T>
    bit_ptr_t<T>::bit_ptr_t(self_type const& p)
        : base_   ( p.base()   )
        , offset_ ( p.offset() )
    {}
    template <typename T>
    typename bit_ptr_t<T>::self_type& bit_ptr_t<T>::operator = (self_type const& p)
    {
        base_ = p.base();
        offset_ = p.offset();
        return *this;
    }

    template <typename T>
    bit_ptr_t<T>::bit_ptr_t(self_type&& p)
        : base_(p.base())
        , offset_(p.offset())
    {
        p.clear();
    }
    template <typename T>
    typename bit_ptr_t<T>::self_type& bit_ptr_t<T>::operator = (self_type&& p)
    {
        if (&p != this)
        {
            base_ = p.base();
            offset_ = p.offset();
            p.clear();
        }
        return *this;
    }

    template <typename T>
    template <typename U>
    bit_ptr_t<T>::bit_ptr_t(bit_ptr_t<U> const& p)
        : base_(p.base())
        , offset_(p.offset())
    {}
    template <typename T>
    template <typename U>
    typename bit_ptr_t<T>::self_type& bit_ptr_t<T>::operator = (bit_ptr_t<U> const& p)
    {
        base_ = p.base();
        offset_ = p.offset();
        return *this;
    }

    template <typename T>
    template <typename U>
    bit_ptr_t<T>::bit_ptr_t(bit_ptr_t<U>&& p)
        : base_(p.base())
        , offset_(p.offset())
    {
        p.clear();
    }
    template <typename T>
    template <typename U>
    typename bit_ptr_t<T>::self_type& bit_ptr_t<T>::operator = (bit_ptr_t<U>&& p)
    {
        base_ = p.base();
        offset_ = p.offset();
        p.clear();
        return *this;
    }

    template <typename T>
    typename bit_ptr_t<T>::base_pointer bit_ptr_t<T>::base() const
    {
        return base_;
    }
    template <typename T>
    typename bit_ptr_t<T>::offset_type bit_ptr_t<T>::offset() const
    {
        return offset_;
    }
    template <typename T>
    typename bit_ptr_t<T>::offset_type bit_ptr_t<T>::reverse_offset() const
    {
        return (bit::base_capacity - offset_) % bit::base_capacity;
    }

    template <typename T>
    ptr<T> bit_ptr_t<T>::base_ptr() const
    {
        return ptr<T>(base_);
    }

    template <typename T>
    bool bit_ptr_t<T>::null() const
    {
        return (base_ == nullptr) && (offset_ == 0);
    }
    template <typename T>
    bool bit_ptr_t<T>::not_null() const
    {
        return !null();
    }
    template <typename T>
    bool bit_ptr_t<T>::even() const
    {
        return offset_ == 0;
    }
    template <typename T>
    bool bit_ptr_t<T>::not_even() const
    {
        return offset_ != 0;
    }

    template <typename T>
    template <typename U>
    typename zzz::bit_ptr_redused<T, U>::type bit_ptr_t<T>::to_pointer() const
    {
        MSW_ASSERT(even());
        return typename zzz::bit_ptr_redused<T, U>::type(reinterpret_cast<typename zzz::bit_ptr_redused<T, U>::type::native_pointer>(base_));
    }
    template <>
    template <>
    inline zzz::bit_ptr_redused<byte, bit>::type bit_ptr_t<byte>::to_pointer<bit>() const
    {
        return *this;
    }
    template <>
    template <>
    inline zzz::bit_ptr_redused<byte, bit const>::type bit_ptr_t<byte>::to_pointer<bit const>() const
    {
        return *this;
    }
    template <>
    template <>
    inline zzz::bit_ptr_redused<byte const, bit>::type bit_ptr_t<byte const>::to_pointer<bit>() const
    {
        return *this;
    }
    template <>
    template <>
    inline zzz::bit_ptr_redused<byte const, bit const>::type bit_ptr_t<byte const>::to_pointer<bit const>() const
    {
        return *this;
    }

    template <typename T>
    void bit_ptr_t<T>::clear()
    {
        base_ = nullptr;
        offset_ = 0;
    }
    template <typename T>
    void bit_ptr_t<T>::swap(self_type& other)
    {
        std::swap(base_, other.base_);
        std::swap(offset_, other.offset_);
    }

    template <typename T>
#ifdef MSW_NO_EXPLICIT_CONVERSION_OPERATORS
    bit_ptr_t<T>::operator safe_bool_type () const
#else
    bit_ptr_t<T>::operator bool () const
#endif
    {
        return (base_ != nullptr) || offset_;
    }

    template <typename T>
    typename bit_ptr_t<T>::reference bit_ptr_t<T>::operator * () const
    {
        MSW_WARY_ASSERT(static_cast<bool>(*this));
        return reference(*base_, offset_);
    }
    template <typename T>
    typename bit_ptr_t<T>::reference bit_ptr_t<T>::operator [] (size_type offset) const
    {
        return *(self_type(*this) += offset);
    }

    template <typename T>
    typename bit_ptr_t<T>::self_type& bit_ptr_t<T>::operator ++ ()
    {
        ++offset_;
        offset_ %= bit::base_capacity;
        base_ += (offset_ == 0);
        return *this;
    }
    template <typename T>
    typename bit_ptr_t<T>::self_type bit_ptr_t<T>::operator ++ (int)
    {
        self_type resutl(*this);
        ++(*this);
        return resutl;
    }
    template <typename T>
    typename bit_ptr_t<T>::self_type& bit_ptr_t<T>::operator -- ()
    {
        --offset_;
        offset_ %= bit::base_capacity;
        base_ -= (offset_ == 0);
        return *this;
    }
    template <typename T>
    typename bit_ptr_t<T>::self_type bit_ptr_t<T>::operator -- (int)
    {
        self_type resutl(*this);
        --(*this);
        return resutl;
    }

    template <typename T>
    typename bit_ptr_t<T>::self_type& bit_ptr_t<T>::operator += (size_type s)
    {
        offset_type ext_res = offset_ + s.count() % bit::base_capacity;
        base_ += (s.count() / bit::base_capacity + ext_res / bit::base_capacity);
        offset_ = (ext_res % bit::base_capacity);
        return *this;
    }
    template <typename T>
    typename bit_ptr_t<T>::self_type& bit_ptr_t<T>::operator -= (size_type s)
    {
        offset_type ext_res = bit::base_capacity + offset_ - s.count() % bit::base_capacity;
        base_ -= (s.count() / bit::base_capacity + ((ext_res / bit::base_capacity) == 0));
        offset_ = (ext_res % bit::base_capacity);
        return *this;
    }

#define MSW_BIT_PTR_ARITHMETIAL_OPERATOR(o) \
    template <typename T> \
    typename bit_ptr_t<T>::self_type bit_ptr_t<T>::operator o (size_type s) const \
    { \
        self_type ret(*this); \
        ret o##= s; \
        return ret; \
    }
    MSW_BIT_PTR_ARITHMETIAL_OPERATOR( + )
    MSW_BIT_PTR_ARITHMETIAL_OPERATOR( - )
#undef MSW_BIT_PTR_ARITHMETIAL_OPERATOR

    template <typename T>
    template <typename U>
    typename bit_ptr_t<T>::size_type bit_ptr_t<T>::operator - (bit_ptr_t<U> p) const
    {
        MSW_ASSERT(*this >= p);
        return (base_ - p.base()) * bit::base_capacity + offset_ - p.offset();
    }

#define MSW_PTR_COMPARE_OPERATOR(o) \
    template <typename T> \
    template <typename U> \
    bool bit_ptr_t<T>::operator o (bit_ptr_t<U> p) const \
    { \
        return base_ == p.base() ? offset_ o p.offset() : base_ o p.base(); \
    }
    MSW_PTR_COMPARE_OPERATOR( == )
    MSW_PTR_COMPARE_OPERATOR( != )
    MSW_PTR_COMPARE_OPERATOR( <  )
    MSW_PTR_COMPARE_OPERATOR( >  )
    MSW_PTR_COMPARE_OPERATOR( <= )
    MSW_PTR_COMPARE_OPERATOR( >= )
#undef MSW_PTR_COMPARE_OPERATOR

}
