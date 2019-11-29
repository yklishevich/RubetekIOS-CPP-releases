#pragma once

#include <algorithm>                   // std::min, std::max

#include <msw/zzz/size/decl.hpp>       // size
#include <msw/zzz/size/count_cast.hpp>
#include <msw/zzz/size/compare.hpp>
#include <msw/type_traits/is_size.hpp>


namespace msw
{
    template <typename T>
    size<T>::size()
        : count_()
    {}
    template <typename T>
    size<T>::size(count_type count)
        : count_(count)
    {}
    template <typename T>
    template <typename U>
    size<T>::size(U u)
        : count_(zzz::count_cast::calc_count<base_type>(u))
    {}

    template <typename T>
    bool size<T>::null() const
    {
        return count_ == 0;
    }
    template <typename T>
    bool size<T>::not_null() const
    {
        return count_ != 0;
    }
    template <typename T>
    bool size<T>::even(self_type s) const
    {
        return (*this % s).null();
    }
    template <typename T>
    bool size<T>::not_even(self_type s) const
    {
        return (*this % s).not_null();
    }
    template <typename T>
    typename size<T>::count_type size<T>::count() const
    {
        return count_;
    }
    template <typename T>
    template <typename U>
    size<U> size<T>::fit_in() const
    {
        return size<U>(zzz::count_cast::fit_in<base_type, U>(count_));
    }
    template <typename T>
    template <typename U>
    size<U> size<T>::fit_in_self() const
    {
        return size<U>(zzz::count_cast::fit_in_self<base_type, U>(count_));
    }

    template <typename T>
    typename size<T>::self_type size<T>::min(self_type s) const
    {
        return std::min(count(), s.count());
    }
    template <typename T>
    typename size<T>::self_type size<T>::max(self_type s) const
    {
        return std::max(count(), s.count());
    }

    template <typename T>
    typename size<T>::self_type size<T>::mod_back(self_type s) const
    {
        self_type ret(*this);
        ret %= s;
        if (ret.not_null()) ret = s - ret;
        return ret;
    }

    template <typename T>
    void size<T>::clear()
    {
        count_ = 0;
    }

    template <typename T>
    void size<T>::swap(self_type&& other)
    {
        count_type c = count_;
        count_ = other.count_;
        other.count_ = c;
    }

    template <typename T>
    typename size<T>::self_type& size<T>::operator ++ ()
    {
        ++count_;
        MSW_WARY_ASSERT(count_);
        return *this;
    }
    template <typename T>
    typename size<T>::self_type size<T>::operator ++ (int)
    {
        self_type resutl(*this);
        ++(*this);
        return resutl;
    }
    template <typename T>
    typename size<T>::self_type& size<T>::operator -- ()
    {
        MSW_WARY_ASSERT(count_);
        --count_;
        return *this;
    }
    template <typename T>
    typename size<T>::self_type size<T>::operator -- (int)
    {
        self_type resutl(*this);
        --(*this);
        return resutl;
    }

    template <typename T>
    typename size<T>::self_type& size<T>::operator += (self_type s)
    {
        MSW_WARY_ASSERT((count_ + s.count()) >= count_);
        count_ += s.count();
        return *this;
    }
    template <typename T>
    typename size<T>::self_type& size<T>::operator -= (self_type s)
    {
        MSW_WARY_ASSERT((count_ - s.count()) <= count_);
        count_ -= s.count();
        return *this;
    }

#define MSW_SIZE_ARITHMETIAL_OPERATOR(o) \
    template <typename T> \
    typename size<T>::self_type size<T>::operator o (self_type s) const \
    { \
        self_type ret(*this); \
        ret o##= s; \
        return ret; \
    }
    MSW_SIZE_ARITHMETIAL_OPERATOR(+)
    MSW_SIZE_ARITHMETIAL_OPERATOR(-)
#undef MSW_SIZE_ARITHMETIAL_OPERATOR

    template <typename T>
    typename size<T>::self_type& size<T>::operator %= (self_type s)
    {
        MSW_WARY_ASSERT(s.count());
        count_ %= s.count();
        return *this;
    }
    template <typename T>
    typename size<T>::self_type size<T>::operator % (self_type s) const
    {
        return self_type(*this) %= s;
    }

    template <typename T>
    typename size<T>::self_type& size<T>::operator *= (count_type c)
    {
        count_ *= c;
        return *this;
    }
    template <typename T>
    typename size<T>::self_type size<T>::operator * (count_type c) const
    {
        return self_type(*this) *= c;
    }

#define MSW_SIZE_COMPARE_OPERATOR(o) \
    template <typename T> \
    bool size<T>::operator o (self_type s) const \
    { \
        return count() o s.count(); \
    } \
    template <typename T> \
    template <typename Sz> \
    bool size<T>::operator o (Sz s) const \
    { \
        static_assert(is_size<Sz>::value, "Sz should be size"); \
        typedef typename zzz::compare_size::common<base_type, typename Sz::base_type>::type common_base_type; \
        return size<common_base_type>(*this) o size<common_base_type>(s);\
    }
    MSW_SIZE_COMPARE_OPERATOR(==)
    MSW_SIZE_COMPARE_OPERATOR(!=)
    MSW_SIZE_COMPARE_OPERATOR(<)
    MSW_SIZE_COMPARE_OPERATOR(>)
    MSW_SIZE_COMPARE_OPERATOR(<=)
    MSW_SIZE_COMPARE_OPERATOR(>=)
#undef MSW_SIZE_COMPARE_OPERATOR
}
