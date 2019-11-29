#pragma once

#include <type_traits>          // std::is_same

#include <msw/config.hpp>
#include <msw/assert.hpp>
#include <msw/plain_types.hpp>  // byte
#include <msw/zzz/ptr/decl.hpp>


namespace msw
{
    template <typename T>
    ptr<T>::ptr()
        : p_(nullptr)
    {}
    template <typename T>
    ptr<T>::ptr(native_pointer p)
        : p_(p)
    {}

    template <typename T>
    ptr<T>::ptr(self_type const& p)
        : p_(p.native())
    {}
    template <typename T>
    typename ptr<T>::self_type& ptr<T>::operator = (self_type const& p)
    {
        p_ = p.native();
        return *this;
    }

    template <typename T>
    ptr<T>::ptr(self_type&& p)
        : p_(p.native())
    {
        p.clear();
    }
    template <typename T>
    typename ptr<T>::self_type& ptr<T>::operator = (self_type&& p)
    {
        if (&p != this)
        {
            p_ = p.native();
            p.clear();
        }
        return *this;
    }

    template <typename T>
    template <typename U>
    ptr<T>::ptr(ptr<U> const& p)
        : p_(p.native())
    {}
    template <typename T>
    template <typename U>
    typename ptr<T>::self_type& ptr<T>::operator = (ptr<U> const& p)
    {
        p_ = p.native();
        return *this;
    }

    template <typename T>
    template <typename U>
    ptr<T>::ptr(ptr<U>&& p)
        : p_(p.native())
    {
        p.clear();
    }
    template <typename T>
    template <typename U>
    typename ptr<T>::self_type& ptr<T>::operator = (ptr<U>&& p)
    {
        p_ = p.native();
        p.clear();
        return *this;
    }

    template <typename T>
    bool ptr<T>::null() const
    {
        return p_ == nullptr;
    }
    template <typename T>
    bool ptr<T>::not_null() const
    {
        return p_ != nullptr;
    }
    template <typename T>
    typename ptr<T>::native_pointer ptr<T>::native() const
    {
        return p_;
    }
    template <typename T>
    template <typename U>
    typename zzz::ptr_redused<T, U>::type ptr<T>::to_pointer() const
    {
        return typename zzz::ptr_redused<T, U>::type(reinterpret_cast<typename zzz::ptr_redused<T, U>::native_pointer>(p_));
    }
    template <typename T>
    typename ptr<T>::number_type ptr<T>::to_number() const
    {
        static_assert(sizeof(number_type) == sizeof(native_pointer), "can't convert pointer to number");
        return reinterpret_cast<number_type>(p_);
    }
    template <typename T>
    template <typename Ptr>
    typename ptr<T>::size_type ptr<T>::distance_to(Ptr ptr) const
    {
        static_assert(std::is_same<value_type const, typename Ptr::value_type const>::value, "pointers must be same");
        return ptr > *this ? ptr - *this : *this - ptr;
    }

    template <typename T>
    template <typename Ptr>
    size<byte> ptr<T>::adjacent_distance(Ptr ptr) const
    {
        static_assert(std::is_same<value_type const, typename Ptr::value_type const>::value, "pointers must be same");
        return to_pointer<byte>().distance_to(ptr.template to_pointer<byte>()) % size_type(1);
    }
    template <>
    template <typename Ptr>
    size<byte> ptr<byte>::adjacent_distance(Ptr) const
    {
        static_assert(std::is_same<value_type const, typename Ptr::value_type const>::value, "pointers must be same");
        return size<byte>(0);
    }
    template <>
    template <typename Ptr>
    size<byte> ptr<byte const>::adjacent_distance(Ptr) const
    {
        static_assert(std::is_same<value_type const, typename Ptr::value_type const>::value, "pointers must be same");
        return size<byte>(0);
    }

    template <typename T>
    template <typename Ptr>
    bool ptr<T>::is_adjacent_to(Ptr ptr) const
    {
        return adjacent_distance(ptr).null();
    }
    template <>
    template <typename Ptr>
    bool ptr<byte>::is_adjacent_to(Ptr) const
    {
        return true;
    }
    template <>
    template <typename Ptr>
    bool ptr<byte const>::is_adjacent_to(Ptr) const
    {
        return true;
    }

    template <typename T>
    template <typename Ptr>
    typename zzz::ptr_common<T, Ptr>::type ptr<T>::upper_adjacent_until(Ptr ptr) const
    {
        static_assert(std::is_same<value_type const, typename Ptr::value_type const>::value, "pointers must be same");
        MSW_WARY_ASSERT(ptr >= *this);
        return (ptr.template to_pointer<byte>() -= adjacent_distance(ptr)).template to_pointer<value_type>();
    }
    template <>
    template <typename Ptr>
    typename zzz::ptr_common<byte, Ptr>::type ptr<byte>::upper_adjacent_until(Ptr ptr) const
    {
        static_assert(std::is_same<value_type const, typename Ptr::value_type const>::value, "pointers must be same");
        MSW_WARY_ASSERT(ptr >= *this);
        return ptr;
    }
    template <>
    template <typename Ptr>
    typename zzz::ptr_common<byte const, Ptr>::type ptr<byte const>::upper_adjacent_until(Ptr ptr) const
    {
        static_assert(std::is_same<value_type const, typename Ptr::value_type const>::value, "pointers must be same");
        MSW_WARY_ASSERT(ptr >= *this);
        return ptr;
    }

    template <typename T>
    template <typename Ptr>
    typename zzz::ptr_common<T, Ptr>::type ptr<T>::lower_adjacent_after(Ptr ptr) const
    {
        static_assert(std::is_same<value_type const, typename Ptr::value_type const>::value, "pointers must be same");
        MSW_WARY_ASSERT(*this >= ptr);
        return (ptr.template to_pointer<byte>() += adjacent_distance(ptr)).template to_pointer<value_type>();
    }
    template <>
    template <typename Ptr>
    typename zzz::ptr_common<byte, Ptr>::type ptr<byte>::lower_adjacent_after(Ptr ptr) const
    {
        static_assert(std::is_same<value_type const, typename Ptr::value_type const>::value, "pointers must be same");
        return ptr;
    }
    template <>
    template <typename Ptr>
    typename zzz::ptr_common<byte const, Ptr>::type ptr<byte const>::lower_adjacent_after(Ptr ptr) const
    {
        static_assert(std::is_same<value_type const, typename Ptr::value_type const>::value, "pointers must be same");
        return ptr;
    }

    template <typename T>
    typename ptr<T>::mutable_type ptr<T>::remove_const() const
    {
        return mutable_type(const_cast<typename std::remove_const<T>::type*>(p_));
    }

    template <typename T>
    void ptr<T>::clear()
    {
        p_ = nullptr;
    }
    template <typename T>
    void ptr<T>::swap(self_type& other)
    {
        std::swap(p_, other.p_);
    }

    template <typename T>
#ifdef MSW_NO_EXPLICIT_CONVERSION_OPERATORS
    ptr<T>::operator safe_bool_type () const
#else
    ptr<T>::operator bool () const
#endif
    {
        return static_cast<bool>(p_);
    }
    template <typename T>
    typename ptr<T>::reference ptr<T>::operator * () const
    {
        MSW_WARY_ASSERT(p_);
        return *p_;
    }
    template <typename T>
    typename ptr<T>::reference ptr<T>::operator [] (size_type offset) const
    {
        return *(self_type(*this) += offset);
    }

    template <typename T>
    typename ptr<T>::self_type& ptr<T>::operator ++ ()
    {
        ++p_;
        MSW_WARY_ASSERT(p_);
        return *this;
    }
    template <typename T>
    typename ptr<T>::self_type ptr<T>::operator ++ (int)
    {
        self_type resutl(*this);
        ++(*this);
        return resutl;
    }
    template <typename T>
    typename ptr<T>::self_type& ptr<T>::operator -- ()
    {
        MSW_WARY_ASSERT(p_);
        --p_;
        return *this;
    }
    template <typename T>
    typename ptr<T>::self_type ptr<T>::operator -- (int)
    {
        self_type resutl(*this);
        --(*this);
        return resutl;
    }

    template <typename T>
    typename ptr<T>::self_type& ptr<T>::operator += (size_type s)
    {
        MSW_WARY_ASSERT((p_ + s.count()) >= p_);
        p_ += s.count();
        return *this;
    }
    template <typename T>
    typename ptr<T>::self_type& ptr<T>::operator -= (size_type s)
    {
        MSW_WARY_ASSERT((p_ - s.count()) <= p_);
        p_ -= s.count();
        return *this;
    }

#define MSW_PTR_ARITHMETIAL_OPERATOR(o) \
    template <typename T> \
    typename ptr<T>::self_type ptr<T>::operator o (size_type s) const \
    { \
        self_type ret(*this); \
        ret o##= s; \
        return ret; \
    }
    MSW_PTR_ARITHMETIAL_OPERATOR( + )
    MSW_PTR_ARITHMETIAL_OPERATOR( - )
#undef MSW_PTR_ARITHMETIAL_OPERATOR

    template <typename T>
    template <typename U>
    typename ptr<T>::size_type ptr<T>::operator - (ptr<U> p) const
    {
        MSW_ASSERT(*this >= p);
        return p_ - p.native();
    }

#define MSW_PTR_COMPARE_OPERATOR(o) \
    template <typename T> \
    template <typename U> \
    bool ptr<T>::operator o (ptr<U> p) const \
    { \
        return p_ o p.native(); \
    }
    MSW_PTR_COMPARE_OPERATOR( == )
    MSW_PTR_COMPARE_OPERATOR( != )
    MSW_PTR_COMPARE_OPERATOR( <  )
    MSW_PTR_COMPARE_OPERATOR( >  )
    MSW_PTR_COMPARE_OPERATOR( <= )
    MSW_PTR_COMPARE_OPERATOR( >= )
#undef MSW_PTR_COMPARE_OPERATOR
}
