#pragma once

#include <cassert>
#include <typeinfo>

#include <rubetek/utility/ctti.hpp>
#include <rubetek/utility/buffer.hpp>
#include <rubetek/utility/package/package.hpp>


namespace rubetek
{
    struct multiform
    {
                                         multiform   ()               ;
        template <typename T>            multiform   (T const&)       ;
        template <typename T> multiform& operator =  (T const&)       ;

                              bool       empty       ()         const ;
                              int        ctti_typeid ()         const ;
        template <typename T> void       pack        (T const&)       ;
        template <typename T> T          unpack      ()         const ;

    private:
        friend struct package<multiform>;

        int    ctti_typeid_ ;
        buffer data_        ;
    };
}


namespace rubetek
{
    inline multiform::multiform()
        : ctti_typeid_(-1)
    {}
    template <typename T>
    multiform::multiform(T const& v)
    {
        pack(v);
    }
    template <typename T>
    multiform& multiform::operator = (T const& v)
    {
        pack(v);
        return *this;
    }

    inline bool multiform::empty() const
    {
        return data_.empty();
    }

    inline int multiform::ctti_typeid() const
    {
        assert(!empty());
        return ctti_typeid_;
    }

    template <typename T>
    void multiform::pack(T const& v)
    {
        ctti_typeid_ = ctti::global_typeid<T>();
        data_ = rubetek::pack(v);
    }

    template <typename T>
    T multiform::unpack() const
    {
        assert(!empty());
        if (ctti::global_typeid<T>() != ctti_typeid()) throw std::runtime_error("can't unpack multiform");
        return rubetek::unpack<T>(data_);
    }
}
