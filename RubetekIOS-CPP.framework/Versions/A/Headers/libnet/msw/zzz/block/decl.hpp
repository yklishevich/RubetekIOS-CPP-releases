#pragma once 

#include <type_traits>          // std::conditional, std::is_const

#include <msw/plain_traits.hpp>


namespace msw {
namespace zzz {

    template <typename T>
    struct block
    {
        typedef block<T>                             self_type  ;
        typedef typename plain_traits<T>::value_type value_type ;
        typedef typename plain_traits<T>::size_type  size_type  ;
        typedef typename plain_traits<T>::pointer    pointer    ;

        template <typename U>
        struct redused
        {
            typedef typename std::conditional
            <
                  std::is_const<T>::value
                , U const
                , U
            >::type value_type;
        };

                                         block             ()                                    ;
                                         block             (pointer first, pointer last)         ;
                                         block             (pointer first, size_type size)       ;
                                         block             (size_type size, pointer last)        ;

                                         block             (self_type const&)                    ;
                              self_type& operator =        (self_type const&)                    ;

                                         block             (self_type&&)                         ;
                              self_type& operator =        (self_type&&)                         ;

        template <typename U>            block             (block<U> const&)                     ;
        template <typename U> self_type& operator =        (block<U> const&)                     ;

        template <typename U>            block             (block<U>&&)                          ;
        template <typename U> self_type& operator =        (block<U>&&)                          ;

                              void       swap              (self_type&)                          ;

                              size_type  size              ()                              const ;
                              bool       empty             ()                              const ;
                              bool       has_items         ()                              const ;

                              pointer    first             ()                              const ;
                              pointer    last              ()                              const ;

                              pointer    inc_first         ()                                    ;
                              pointer    dec_last          ()                                    ;

                              pointer    inc_first         (size_type)                           ;
                              pointer    dec_last          (size_type)                           ;

                              void       set_first         (pointer)                             ;
                              void       set_last          (pointer)                             ;

        template <typename U> block<U>   to_block          ()                              const ;

    private:
                              bool       check_orientation ()                              const ;
                              bool       check             ()                              const ;

        template <typename U> friend struct block; // for move

        pointer first_ ;
        pointer last_  ;
    };

}}
