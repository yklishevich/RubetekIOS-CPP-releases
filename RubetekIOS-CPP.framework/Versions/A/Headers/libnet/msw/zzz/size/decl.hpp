#pragma once

#include <cstddef>                      // std::size_t
#include <type_traits>                  // std::is_unsigned, std::is_const

#include <msw/int.hpp>
#include <msw/plain_types.hpp>          // bit
#include <msw/type_traits/is_plain.hpp>


namespace msw
{
    template <typename T>
    struct size
    {
        static_assert(!std::is_const<T>::value, "T should be non const");
        static_assert(is_plain<T>::value, "base type should be plain type");

        typedef T       base_type  ;
        typedef size<T> self_type  ;
        typedef uint    count_type ;

        static_assert(std::is_unsigned<count_type>::value, "count_type should be unsigned type");

                                          size        ()                  ;
                                          size        (count_type)        ;
        template <typename U>             size        (U)                 ;

                               bool       null        ()            const ;
                               bool       not_null    ()            const ;
                               bool       even        (self_type)   const ;
                               bool       not_even    (self_type)   const ;
                               count_type count       ()            const ;
        template <typename U>  size<U>    fit_in      ()            const ;
        template <typename U>  size<U>    fit_in_self ()            const ;

                               self_type  min         (self_type)   const ;
                               self_type  max         (self_type)   const ;

                               self_type  mod_back    (self_type)   const ;

                               void       clear       ()                  ;

                               void       swap        (self_type&&)       ;

                               self_type& operator ++ ()                  ;
                               self_type  operator ++ (int)               ;
                               self_type& operator -- ()                  ;
                               self_type  operator -- (int)               ;

                               self_type& operator += (self_type)         ;
                               self_type& operator -= (self_type)         ;

                               self_type  operator +  (self_type)   const ;
                               self_type  operator -  (self_type)   const ;

                               self_type& operator %= (self_type)         ;
                               self_type  operator %  (self_type)   const ;

                               self_type& operator *= (count_type)        ;
                               self_type  operator *  (count_type)  const ;

#define MSW_SIZE_OPERATOR_COMPARE(o) \
                               bool       operator o  (self_type)   const ; \
        template <typename Sz> bool       operator o  (Sz)          const ;
        MSW_SIZE_OPERATOR_COMPARE(==)
        MSW_SIZE_OPERATOR_COMPARE(!=)
        MSW_SIZE_OPERATOR_COMPARE(< )
        MSW_SIZE_OPERATOR_COMPARE(> )
        MSW_SIZE_OPERATOR_COMPARE(>=)
        MSW_SIZE_OPERATOR_COMPARE(<=)
#undef MSW_SIZE_OPERATOR_COMPARE

    private:
        count_type count_;
    };

    typedef size<bit> bit_size;
}
