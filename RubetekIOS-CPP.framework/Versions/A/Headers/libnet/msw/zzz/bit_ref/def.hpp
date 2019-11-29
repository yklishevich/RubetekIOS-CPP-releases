#pragma once

#include <msw/zzz/bit_ref/decl.hpp>
#include <msw/zzz/ptr/decl.hpp>


namespace msw
{
    template <typename T>
    bit_ref_t<T>::bit_ref_t(base_reference base, offset_type offset)
        : base_  ( base   )
        , offset_( offset )
    {}

    template <typename T>
    typename bit_ref_t<T>::self_type& bit_ref_t<T>::operator = (self_type const& other)
    {
        if (other) set();
        else reset();
        return *this;
    }

    template <typename T>
    template <typename U>
    bit_ref_t<T>::bit_ref_t(bit_ref_t<U> const& other)
        : base_   ( other.base()   )
        , offset_ ( other.offset() )
    {}
    template <typename T>
    template <typename U>
    typename bit_ref_t<T>::self_type& bit_ref_t<T>::operator = (bit_ref_t<U> const& other)
    {
        if (other) set();
        else reset();
        return *this;
    }

    template <typename T>
    typename bit_ref_t<T>::self_type& bit_ref_t<T>::operator = (bool other)
    {
        if (other) set();
        else reset();
        return *this;
    }

    template <typename T>
    typename bit_ref_t<T>::base_reference bit_ref_t<T>::base() const
    {
        return base_;
    }
    template <typename T>
    typename bit_ref_t<T>::offset_type bit_ref_t<T>::offset() const
    {
        return offset_;
    }

    template <typename T>
    typename bit_ref_t<T>::pointer bit_ref_t<T>::operator & () const
    {
        return pointer(&base_, offset_);
    }

    template <typename T>
#ifdef MSW_NO_EXPLICIT_CONVERSION_OPERATORS
    bit_ref_t<T>::operator safe_bool_type () const
#else
    bit_ref_t<T>::operator bool () const
#endif
    {
        return (base_ >> offset_) & 1;
    }

    template <typename T>
    bool bit_ref_t<T>::null() const
    {
        return !static_cast<bool>(*this);
    }

    template <typename T>
    void bit_ref_t<T>::set()
    {
        base_ |= base_type(1) << offset_;
    }
    template <typename T>
    void bit_ref_t<T>::reset()
    {
        base_ &= ~(base_type(1) << offset_);
    }
    template <typename T>
    void bit_ref_t<T>::invert()
    {
        base_ ^= (1 << offset_);
    }

#define MSW_BIT_REF_COMPARE_OPERATOR(o) \
    template <typename T> \
    template <typename U> \
    bool bit_ref_t<T>::operator o (bit_ref_t<U> other) const \
    { \
        return static_cast<bool>(*this) o static_cast<bool>(other); \
    }
    MSW_BIT_REF_COMPARE_OPERATOR( == )
    MSW_BIT_REF_COMPARE_OPERATOR( != )
    MSW_BIT_REF_COMPARE_OPERATOR( <  )
    MSW_BIT_REF_COMPARE_OPERATOR( >  )
    MSW_BIT_REF_COMPARE_OPERATOR( <= )
    MSW_BIT_REF_COMPARE_OPERATOR( >= )
#undef MSW_BIT_REF_COMPARE_OPERATOR
}
