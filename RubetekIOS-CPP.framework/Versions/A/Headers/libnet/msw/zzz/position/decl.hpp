#pragma once

#include <type_traits>                         // std::conditional, std::is_const

#include <msw/on_debug.hpp>
#include <msw/plain_traits.hpp>
#include <msw/zzz/ptr/decl.hpp>
#include <msw/zzz/range/fwd.hpp>
#include <msw/zzz/position/construct_decl.hpp>
#ifdef MSW_DEBUG
#   include <msw/zzz/validator/link/decl.hpp>
#endif


namespace msw
{
    template <typename T>
    struct position
    {
        typedef position<T>                          self_type  ;
        typedef typename plain_traits<T>::value_type value_type ;
        typedef typename plain_traits<T>::size_type  size_type  ;
        typedef typename plain_traits<T>::pointer    pointer    ;
        typedef typename plain_traits<T>::reference  reference  ;

        template <typename U>
        struct redused
        {
            typedef typename std::conditional
            <
                  std::is_const<T>::value
                , U const
                , U
            >::type                      value_type ;
            typedef position<value_type> type       ;
        };

        template <typename U>
        struct common
        {
            typedef typename std::conditional
            <
                  std::is_const<T>::value
                , T
                , U
            >::type                        value_type ;
            typedef msw::range<value_type> range      ;
        };

                                                          position    ()                         ;

                                                          position    (self_type const&)         ;
                                self_type&                operator =  (self_type const&)         ;

                                                          position    (self_type&&)              ;
                                self_type&                operator =  (self_type&&)              ;

        template <typename U>                             position    (position<U> const&)       ;
        template <typename U>   self_type&                operator =  (position<U> const&)       ;

        template <typename U>                             position    (position<U>&&)            ;
        template <typename U>   self_type&                operator =  (position<U>&&)            ;

        pointer                                           to_pointer  ()                   const ;

        template <typename U>   typename redused<U>::type to_position ()                   const ;

        template <typename U>   size_type                 operator -  (position<U>)        const ;

        template <typename U>   typename common<U>::range operator +  (position<U>)        const ;

        bool                                              null        ()                   const ;
        bool                                              not_null    ()                   const ;

#define MSW_POSITION_OPERATOR_COMPARE(o) \
        template <typename U>   bool                      operator o  (position<U>)        const ;
        MSW_POSITION_OPERATOR_COMPARE( == )
        MSW_POSITION_OPERATOR_COMPARE( != )
        MSW_POSITION_OPERATOR_COMPARE( <  )
        MSW_POSITION_OPERATOR_COMPARE( >  )
        MSW_POSITION_OPERATOR_COMPARE( >= )
        MSW_POSITION_OPERATOR_COMPARE( <= )
#undef MSW_POSITION_OPERATOR_COMPARE

    private:

        template <typename Pointer MSW_ON_DEBUG(, typename Validator)>
        explicit position(Pointer MSW_ON_DEBUG(, Validator&&));

        template <typename U> friend struct position; // for move

        template <typename Pointer MSW_ON_DEBUG(, typename Validator)>
        friend
        position<typename Pointer::value_type> zzz::construct_position(Pointer MSW_ON_DEBUG(, Validator&&));

        pointer ptr_;

        MSW_ON_DEBUG
        (
            zzz::validator_link validator_;
        )
    };
}
