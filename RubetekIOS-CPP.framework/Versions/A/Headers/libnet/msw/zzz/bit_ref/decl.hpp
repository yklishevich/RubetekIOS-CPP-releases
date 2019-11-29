#pragma once

#include <type_traits>          // std::is_same, std::remove_const

#include <msw/config.hpp>
#include <msw/plain_types.hpp>  // bit
#include <msw/zzz/ptr/fwd.hpp>
#ifdef MSW_NO_EXPLICIT_CONVERSION_OPERATORS
#   include <msw/safe_bool.hpp>
#endif


namespace msw
{
    template <typename T>
    struct bit_ref_t
    {
        static_assert(std::is_same<typename std::remove_const<T>::type, bit::base_type>::value, "T must be only byte");

        typedef          bit_ref_t<T>            self_type      ;
        typedef          bit                     value_type     ;
        typedef typename value_type::offset_type offset_type    ;
        typedef typename value_type::base_type   base_type      ;
        typedef          T&                      base_reference ;
        typedef          ptr<T>                  pointer        ;

                              explicit       bit_ref_t               (base_reference, offset_type = 0)       ;

                              self_type&     operator =              (self_type const&)                      ;

        template <typename U>                bit_ref_t               (bit_ref_t<U> const&)                   ;
        template <typename U> self_type&     operator =              (bit_ref_t<U> const&)                   ;

                              self_type&     operator =              (bool)                                  ;

                              base_reference base                    ()                                const ;
                              offset_type    offset                  ()                                const ;

                              pointer        operator &              ()                                const ;

#ifdef MSW_NO_EXPLICIT_CONVERSION_OPERATORS
        typedef typename safe_bool<self_type>::type safe_bool_type;
                                             operator safe_bool_type ()                                const ;
#else
                              explicit       operator bool           ()                                const ;
#endif

                              bool           null                    ()                                const ;

                              void           set                     ()                                      ;
                              void           reset                   ()                                      ;
                              void           invert                  ()                                      ;

#define MSW_BIT_REF_OPERATOR_COMPARE(o) \
        template <typename U> bool            operator o             (bit_ref_t<U>)                    const ;
        MSW_BIT_REF_OPERATOR_COMPARE( == )
        MSW_BIT_REF_OPERATOR_COMPARE( != )
        MSW_BIT_REF_OPERATOR_COMPARE( <  )
        MSW_BIT_REF_OPERATOR_COMPARE( >  )
        MSW_BIT_REF_OPERATOR_COMPARE( >= )
        MSW_BIT_REF_OPERATOR_COMPARE( <= )
#undef MSW_BIT_REF_OPERATOR_COMPARE

    private:
        base_reference base_   ;
        offset_type    offset_ ;
    };
}
