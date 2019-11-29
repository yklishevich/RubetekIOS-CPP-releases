#pragma once

#include <type_traits>                      // std::remove_const

#include <msw/config.hpp>
#include <msw/int.hpp>
#include <msw/plain_types.hpp>              // bit, byte
#include <msw/size.hpp>
#include <msw/bit_ref.hpp>
#include <msw/zzz/ptr/decl.hpp>
#include <msw/zzz/bit_ptr/implication.hpp>
#ifdef MSW_NO_EXPLICIT_CONVERSION_OPERATORS
#   include <msw/safe_bool.hpp>
#endif


namespace msw
{
    template <typename T>
    struct bit_ptr_t
    {
        typedef typename zzz::bit_ptr_value<T>::type value_type   ;
        typedef          bit::offset_type            offset_type  ;
        typedef          T*                          base_pointer ;
        typedef          bit_ptr_t<T>                self_type    ;
        typedef          bit_ref_t<T>                reference    ;
        typedef          bit_size                    size_type    ;

                                                                        bit_ptr_t               ()                                           ;
                              explicit                                  bit_ptr_t               (base_pointer, offset_type offset = 0)       ;

                                                                        bit_ptr_t               (self_type const&)                           ;
                              self_type&                                operator =              (self_type const&)                           ;

                                                                        bit_ptr_t               (self_type&&)                                ;
                              self_type&                                operator =              (self_type&&)                                ;

        template <typename U>                                           bit_ptr_t               (bit_ptr_t<U> const&)                        ;
        template <typename U> self_type&                                operator =              (bit_ptr_t<U> const&)                        ;

        template <typename U>                                           bit_ptr_t               (bit_ptr_t<U>&&)                             ;
        template <typename U> self_type&                                operator =              (bit_ptr_t<U>&&)                             ;

                              base_pointer                              base                    ()                                     const ;
                              offset_type                               offset                  ()                                     const ;
                              offset_type                               reverse_offset          ()                                     const ;

                              ptr<T>                                    base_ptr                ()                                     const ;

                              bool                                      null                    ()                                     const ;
                              bool                                      not_null                ()                                     const ;
                              bool                                      even                    ()                                     const ;
                              bool                                      not_even                ()                                     const ;

        template <typename U> typename zzz::bit_ptr_redused<T, U>::type to_pointer              ()                                     const ;

                              void                                      clear                   ()                                           ;
                              void                                      swap                    (self_type&)                                 ;

#ifdef MSW_NO_EXPLICIT_CONVERSION_OPERATORS
        typedef typename safe_bool<self_type>::type safe_bool_type;
                                                                        operator safe_bool_type ()                                     const ;
#else
                              explicit                                  operator bool           ()                                     const ;
#endif
                              reference                                 operator *              ()                                     const ;
                              reference                                 operator []             (size_type)                            const ;

                              self_type&                                operator ++             ()                                           ;
                              self_type                                 operator ++             (int)                                        ;
                              self_type&                                operator --             ()                                           ;
                              self_type                                 operator --             (int)                                        ;

                              self_type&                                operator +=             (size_type)                                  ;
                              self_type&                                operator -=             (size_type)                                  ;

                              self_type                                 operator +              (size_type)                            const ;
                              self_type                                 operator -              (size_type)                            const ;

        template <typename U> size_type                                 operator -              (bit_ptr_t<U>)                         const ;

#define MSW_BIT_PTR_OPERATOR_COMPARE(o) \
        template <typename U> bool                                      operator o              (bit_ptr_t<U>)                         const ;
        MSW_BIT_PTR_OPERATOR_COMPARE( == )
        MSW_BIT_PTR_OPERATOR_COMPARE( != )
        MSW_BIT_PTR_OPERATOR_COMPARE( <  )
        MSW_BIT_PTR_OPERATOR_COMPARE( >  )
        MSW_BIT_PTR_OPERATOR_COMPARE( >= )
        MSW_BIT_PTR_OPERATOR_COMPARE( <= )
#undef MSW_BIT_PTR_OPERATOR_COMPARE

    private:
        base_pointer base_   ;
        offset_type  offset_ ;
    };

    typedef bit_ptr_t<byte>       bit_ptr       ;
    typedef bit_ptr_t<byte const> bit_const_ptr ;
}
