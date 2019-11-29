#pragma once

#include <type_traits>                        // std::conditional, std::is_const

#include <msw/int.hpp>
#include <msw/on_debug.hpp>
#include <msw/plain_traits.hpp>
#include <msw/zzz/position/decl.hpp>
#include <msw/zzz/block/decl.hpp>
#include <msw/zzz/range/construct_decl.hpp>
#include <msw/zzz/range/implication.hpp>
#ifdef MSW_DEBUG
#   include <msw/zzz/validator/link/decl.hpp>
#endif


namespace msw
{
    template <typename T>
    struct range
    {
        typedef          range<T>                         self_type       ;
        typedef          range<T const>                   const_range     ;
        typedef          msw::position<T>                 position        ;
        typedef          msw::position<T const>           const_position  ;

        typedef typename plain_traits<T>::value_type      value_type      ;
        typedef typename plain_traits<T>::size_type       size_type       ;
        typedef typename plain_traits<T>::pointer         pointer         ;
        typedef typename plain_traits<T>::const_pointer   const_pointer   ;
        typedef typename plain_traits<T>::reference       reference       ;
        typedef typename plain_traits<T>::const_reference const_reference ;
        typedef          uint                             index_type      ;

        typedef          zzz::block<value_type>           block_type      ;

                                                                           range              ()                                                         ;

                                                                           range              (self_type const&)                                         ;
                              self_type&                                   operator =         (self_type const&)                                         ;

                                                                           range              (self_type&&)                                              ;
                              self_type&                                   operator =         (self_type&&)                                              ;

        template <typename U>                                              range              (range<U> const&)                                          ;
        template <typename U> self_type&                                   operator =         (range<U> const&)                                          ;

        template <typename U>                                              range              (range<U>&&)                                               ;
        template <typename U> self_type&                                   operator =         (range<U>&&)                                               ;

                              pointer                                      data               ()                                                   const ;
        template <typename U> typename zzz::range_redused<T, U>::pointer   data               ()                                                   const ;
                              size_type                                    size               ()                                                   const ;
        template <typename U> typename zzz::range_redused<T, U>::size_type size               ()                                                   const ;
                              bool                                         empty              ()                                                   const ;
                              bool                                         has_items          ()                                                   const ;

                              reference                                    item               (index_type)                                         const ;
        template <typename U> typename zzz::range_redused<T, U>::reference item               (index_type)                                         const ;

                              reference                                    operator []        (index_type)                                         const ;

        template <typename U> typename zzz::range_redused<T, U>::reference as                 ()                                                   const ;

                              position                                     begin              ()                                                   const ;
                              position                                     begin              (index_type)                                         const ;
//      template <typename U> typename zzz::range_redused<T, U>::position  begin              ()                                                   const ;
//      template <typename U> typename zzz::range_redused<T, U>::position  begin              (index_type)                                         const ;
                              position                                     end                ()                                                   const ;
                              position                                     end                (index_type)                                         const ;
//      template <typename U> typename zzz::range_redused<T, U>::position  end                ()                                                   const ;
//      template <typename U> typename zzz::range_redused<T, U>::position  end                (index_type)                                         const ;

                              self_type                                    all                ()                                                   const ;
        template <typename U> typename zzz::range_redused<T, U>::type      all                ()                                                   const ;

                              reference                                    front              ()                                                   const ;
        template <typename U> typename zzz::range_redused<T, U>::reference front              ()                                                   const ;
                              self_type                                    front              (size_type count)                                    const ;
        template <typename U> typename zzz::range_redused<T, U>::type      front              (typename zzz::range_redused<T, U>::size_type count) const ;
                              self_type                                    front_max          (size_type count)                                    const ;
        template <typename U> typename zzz::range_redused<T, U>::type      front_max          (typename zzz::range_redused<T, U>::size_type count) const ;
        template <typename U> typename zzz::range_redused<T, U>::type      front_max          ()                                                   const ;

                              reference                                    back               ()                                                   const ;
        template <typename U> typename zzz::range_redused<T, U>::reference back               ()                                                   const ;
                              self_type                                    back               (size_type count)                                    const ;
        template <typename U> typename zzz::range_redused<T, U>::type      back               (typename zzz::range_redused<T, U>::size_type count) const ;
                              self_type                                    back_max           (size_type count)                                    const ;
        template <typename U> typename zzz::range_redused<T, U>::type      back_max           (typename zzz::range_redused<T, U>::size_type count) const ;
        template <typename U> typename zzz::range_redused<T, U>::type      back_max           ()                                                   const ;

                              self_type                                    from_to_front      (size_type count)                                    const ;
                              self_type                                    from_to_back       (size_type count)                                    const ;
                              self_type                                    from_back_to_front (size_type count)                                    const ;
                              self_type                                    from_to            (size_type first, size_type last)                    const ;
                              self_type                                    from_count         (size_type first, size_type count)                   const ;

                              reference                                    pop_front          ()                                                         ;
        template <typename U> typename zzz::range_redused<T, U>::reference pop_front          ()                                                         ;
                              self_type                                    pop_front          (size_type count)                                          ;
        template <typename U> typename zzz::range_redused<T, U>::type      pop_front          (typename zzz::range_redused<T, U>::size_type count)       ;
                              self_type                                    pop_front_max      (size_type count)                                          ;
        template <typename U> typename zzz::range_redused<T, U>::type      pop_front_max      (typename zzz::range_redused<T, U>::size_type count)       ;
        template <typename U> typename zzz::range_redused<T, U>::type      pop_front_max      ()                                                         ;

                              reference                                    pop_back           ()                                                         ;
        template <typename U> typename zzz::range_redused<T, U>::reference pop_back           ()                                                         ;
                              self_type                                    pop_back           (size_type count)                                          ;
        template <typename U> typename zzz::range_redused<T, U>::type      pop_back           (typename zzz::range_redused<T, U>::size_type count)       ;
                              self_type                                    pop_back_max       (size_type count)                                          ;
        template <typename U> typename zzz::range_redused<T, U>::type      pop_back_max       (typename zzz::range_redused<T, U>::size_type count)       ;
        template <typename U> typename zzz::range_redused<T, U>::type      pop_back_max       ()                                                         ;

                              self_type                                    pop_from_to_front  (size_type count)                                          ;/// ??? pop_front
                              self_type                                    pop_from_to_back   (size_type count)                                          ;/// ??? pop_back

                              self_type&                                   crop_front         (size_type count)                                          ;
        template <typename U> typename zzz::range_redused<T, U>::type      crop_front         (typename zzz::range_redused<T, U>::size_type count)       ;
                              self_type&                                   crop_front_max     (size_type count)                                          ;
        template <typename U> typename zzz::range_redused<T, U>::type      crop_front_max     (typename zzz::range_redused<T, U>::size_type count)       ;
        template <typename U> typename zzz::range_redused<T, U>::type      crop_front_max     ()                                                         ;

                              self_type&                                   crop_back          (size_type count)                                          ;
        template <typename U> typename zzz::range_redused<T, U>::type      crop_back          (typename zzz::range_redused<T, U>::size_type count)       ;
                              self_type&                                   crop_back_max      (size_type count)                                          ;
        template <typename U> typename zzz::range_redused<T, U>::type      crop_back_max      (typename zzz::range_redused<T, U>::size_type count)       ;
        template <typename U> typename zzz::range_redused<T, U>::type      crop_back_max      ()                                                         ;

                              self_type&                                   crop_from_to_front (size_type back)                                           ;
                              self_type&                                   crop_from_to_back  (size_type first)                                          ;
//                            self_type&                                   from_back_to_front (size_type first)                                          ;
                              self_type&                                   crop_from_to       (size_type first, size_type last)                          ;
                              self_type&                                   crop_from_count    (size_type first, size_type count)                         ;

                              void                                         shrink_forward     ()                                                         ;
                              void                                         shrink_backward    ()                                                         ;

                              void                                         clear              ()                                                         ;

    private:
        template <typename Block MSW_ON_DEBUG(, typename Validator)>
                              explicit                                     range              (Block&& MSW_ON_DEBUG(, Validator&&), zzz::construct_range_tag)       ;

                              bool                                         check              ()                                                              const ;

        template <typename U> friend struct range; // for move

        template <typename Block MSW_ON_DEBUG(, typename Validator)>
        friend
        range<typename std::remove_reference<Block>::type::value_type> zzz::construct_range(Block&& MSW_ON_DEBUG(, Validator&&));

        block_type block_;

        MSW_ON_DEBUG
        (
            zzz::validator_link validator_;
        )
    };
}
