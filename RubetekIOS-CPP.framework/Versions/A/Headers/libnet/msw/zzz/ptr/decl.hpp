#pragma once

#include <type_traits>                 // std::remove_const

#include <msw/config.hpp>
#include <msw/int.hpp>
#include <msw/plain_types.hpp>         // byte
#include <msw/size.hpp>
#include <msw/zzz/ptr/implication.hpp>
#ifdef MSW_NO_EXPLICIT_CONVERSION_OPERATORS
#   include <msw/safe_bool.hpp>
#endif


namespace msw
{
    template <typename T>
    struct ptr
    {
        typedef T*                                                 native_pointer ;
        typedef T                                                  value_type     ;
        typedef ptr<T>                                             self_type      ;
        typedef ptr<typename std::remove_const<value_type>::type>  mutable_type   ;
        typedef T&                                                 reference      ;
        typedef size<typename std::remove_const<value_type>::type> size_type      ;
        typedef uint                                               number_type    ;

                                                                       ptr                     ()                       ;
                                explicit                               ptr                     (native_pointer)         ;

                                                                       ptr                     (self_type const&)       ;
                                self_type&                             operator =              (self_type const&)       ;

                                                                       ptr                     (self_type&&)            ;
                                self_type&                             operator =              (self_type&&)            ;

        template <typename U>                                          ptr                     (ptr<U> const&)          ;
        template <typename U>   self_type&                             operator =              (ptr<U> const&)          ;

        template <typename U>                                          ptr                     (ptr<U>&&)               ;
        template <typename U>   self_type&                             operator =              (ptr<U>&&)               ;

                                bool                                   null                    ()                 const ;
                                bool                                   not_null                ()                 const ;
                                native_pointer                         native                  ()                 const ;
        template <typename U>   typename zzz::ptr_redused<T, U>::type  to_pointer              ()                 const ;
                                number_type                            to_number               ()                 const ;
        template <typename Ptr> size_type                              distance_to             (Ptr)              const ;
        template <typename Ptr> size<byte>                             adjacent_distance       (Ptr)              const ;
        template <typename Ptr> bool                                   is_adjacent_to          (Ptr)              const ;
        template <typename Ptr> typename zzz::ptr_common<T, Ptr>::type upper_adjacent_until    (Ptr)              const ;
        template <typename Ptr> typename zzz::ptr_common<T, Ptr>::type lower_adjacent_after    (Ptr)              const ;

                                mutable_type                           remove_const            ()                 const ;

                                void                                   clear                   ()                       ;
                                void                                   swap                    (self_type&)             ;

#ifdef MSW_NO_EXPLICIT_CONVERSION_OPERATORS
        typedef typename safe_bool<self_type>::type safe_bool_type;
                                                                       operator safe_bool_type ()                 const ;
#else
                                explicit                               operator bool           ()                 const ;
#endif
                                reference                              operator *              ()                 const ;
                                reference                              operator []             (size_type)        const ;

                                self_type&                             operator ++             ()                       ;
                                self_type                              operator ++             (int)                    ;
                                self_type&                             operator --             ()                       ;
                                self_type                              operator --             (int)                    ;

                                self_type&                             operator +=             (size_type)              ;
                                self_type&                             operator -=             (size_type)              ;

                                self_type                              operator +              (size_type)        const ;
                                self_type                              operator -              (size_type)        const ;

        template <typename U>   size_type                              operator -              (ptr<U>)           const ;

#define MSW_PTR_OPERATOR_COMPARE(o) \
        template <typename U>   bool                                   operator o              (ptr<U>)           const ;
        MSW_PTR_OPERATOR_COMPARE( == )
        MSW_PTR_OPERATOR_COMPARE( != )
        MSW_PTR_OPERATOR_COMPARE( <  )
        MSW_PTR_OPERATOR_COMPARE( >  )
        MSW_PTR_OPERATOR_COMPARE( >= )
        MSW_PTR_OPERATOR_COMPARE( <= )
#undef MSW_PTR_OPERATOR_COMPARE

    private:
        native_pointer p_;
    };
}
