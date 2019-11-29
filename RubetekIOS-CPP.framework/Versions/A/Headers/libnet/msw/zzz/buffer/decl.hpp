#pragma once

#include <type_traits>                        // std::is_const

#include <msw/int.hpp>
#include <msw/on_debug.hpp>
#include <msw/zzz/range/decl.hpp>
#include <msw/zzz/position/decl.hpp>
#include <msw/zzz/allocator/decl.hpp>
#ifdef MSW_DEBUG
#   include <msw/zzz/validator/root/decl.hpp>
#endif


namespace msw
{
    template <typename T>
    struct buffer
    {
        static_assert(!std::is_const<T>::value, "container value must be non const");

        typedef          buffer<T>                       self_type       ;
        typedef          zzz::allocator<T>               allocator_type  ;

        typedef typename allocator_type::value_type      value_type      ;
        typedef typename allocator_type::pointer         pointer         ;
        typedef typename allocator_type::const_pointer   const_pointer   ;
        typedef typename allocator_type::reference       reference       ;
        typedef typename allocator_type::const_reference const_reference ;
        typedef typename allocator_type::size_type       size_type       ;

        typedef          uint                            index_type      ;
        typedef          uint                            repeat_count    ;

        typedef          msw::range<value_type>          range           ;
        typedef          msw::range<value_type const>    const_range     ;

        typedef          msw::position<T>                position        ;
        typedef          msw::position<T const>          const_position  ;


                                                                        buffer               ()                                                ;
                              explicit                                  buffer               (size_type size)                                  ;
                                                                        buffer               (size_type size, size_type capacity)              ;

                                                                        ~buffer              ()                                                ;

                                                                        buffer               (self_type&&)                                     ;
                              self_type&                                operator =           (self_type&&)                                     ;

                                                                        buffer               (self_type const&)                                ;
                              self_type&                                operator =           (self_type const&)                                ;

                              explicit                                  buffer               (const_range)                                     ;
                              self_type&                                operator =           (const_range)                                     ;
                              explicit                                  buffer               (range)                                           ;
                              self_type&                                operator =           (range)                                           ;

                              self_type&                                assign_silent        (const_range)                                     ;
                              self_type&                                assign_silent        (range)                                           ;

                              pointer                                   data                 ()                                                ;
        template <typename U> typename plain_traits<U>::pointer         data                 ()                                                ;
                              const_pointer                             data                 ()                                          const ;
        template <typename U> typename plain_traits<U>::const_pointer   data                 ()                                          const ;
                              size_type                                 size                 ()                                          const ;
        template <typename U> typename plain_traits<U>::size_type       size                 ()                                          const ;
                              bool                                      empty                ()                                          const ;
                              bool                                      has_items            ()                                          const ;
                              size_type                                 capacity             ()                                          const ;
        template <typename U> typename plain_traits<U>::size_type       capacity             ()                                          const ;
                              size_type                                 free_space           ()                                          const ;
        template <typename U> typename plain_traits<U>::size_type       free_space           ()                                          const ;
                              bool                                      no_free_space        ()                                          const ;
                              bool                                      has_free_space       ()                                          const ;

                              range                                     all                  ()                                                ;
        template <typename U> msw::range<U>                             all                  ()                                                ;
                              const_range                               all                  ()                                          const ;
        template <typename U> msw::range<U const>                       all                  ()                                          const ;

                              position                                  begin                ()                                                ;
                              position                                  begin                (index_type)                                      ;
//      template <typename U> msw::position<U>                          begin                ()                                                ;
//      template <typename U> msw::position<U>                          begin                (index_type)                                      ;
                              const_position                            begin                ()                                          const ;
                              const_position                            begin                (index_type)                                const ;
//      template <typename U> msw::position<U const>                    begin                ()                                          const ;
//      template <typename U> msw::position<U const>                    begin                (index_type)                                const ;

                              position                                  end                  ()                                                ;
                              position                                  end                  (index_type)                                      ;
//      template <typename U> msw::position<U>                          end                  ()                                                ;
//      template <typename U> msw::position<U>                          end                  (index_type)                                      ;
                              const_position                            end                  ()                                          const ;
                              const_position                            end                  (index_type)                                const ;
//      template <typename U> msw::position<U const>                    end                  ()                                          const ;
//      template <typename U> msw::position<U const>                    end                  (index_type)                                const ;

                              reference                                 item                 (index_type)                                      ;
        template <typename U> typename plain_traits<U>::reference       item                 (index_type)                                      ;
                              const_reference                           item                 (index_type)                                const ;
        template <typename U> typename plain_traits<U>::const_reference item                 (index_type)                                const ;

                              reference                                 operator []          (index_type)                                      ;
                              const_reference                           operator []          (index_type)                                const ;

                              reference                                 front                ()                                                ;
        template <typename U> typename plain_traits<U>::reference       front                ()                                                ;
                              range                                     front                (size_type count)                                 ;
        template <typename U> typename plain_traits<U>::reference       front                (typename plain_traits<U>::size_type count)       ;
                              range                                     front_max            (size_type count)                                 ;
        template <typename U> msw::range<U>                             front_max            (typename plain_traits<U>::size_type count)       ;
        template <typename U> msw::range<U>                             front_max            ()                                                ;
                              const_reference                           front                ()                                          const ;
        template <typename U> typename plain_traits<U>::const_reference front                ()                                          const ;
                              const_range                               front                (size_type count)                           const ;
        template <typename U> typename plain_traits<U>::const_reference front                (typename plain_traits<U>::size_type count) const ;
                              const_range                               front_max            (size_type count)                           const ;
        template <typename U> msw::range<U const>                       front_max            (typename plain_traits<U>::size_type count) const ;
        template <typename U> msw::range<U const>                       front_max            ()                                          const ;

                              reference                                 back                 ()                                                ;
        template <typename U> typename plain_traits<U>::reference       back                 ()                                                ;
                              range                                     back                 (size_type count)                                 ;
        template <typename U> typename plain_traits<U>::reference       back                 (typename plain_traits<U>::size_type count)       ;
                              range                                     back_max             (size_type count)                                 ;
        template <typename U> msw::range<U>                             back_max             (typename plain_traits<U>::size_type count)       ;
        template <typename U> msw::range<U>                             back_max             ()                                                ;
                              const_reference                           back                 ()                                          const ;
        template <typename U> typename plain_traits<U>::const_reference back                 ()                                          const ;
                              const_range                               back                 (size_type count)                           const ;
        template <typename U> typename plain_traits<U>::const_reference back                 (typename plain_traits<U>::size_type count) const ;
                              const_range                               back_max             (size_type count)                           const ;
        template <typename U> msw::range<U const>                       back_max             (typename plain_traits<U>::size_type count) const ;
        template <typename U> msw::range<U const>                       back_max             ()                                          const ;

                              range                                     from_to_front        (size_type count)                                 ;
                              range                                     from_to_back         (size_type count)                                 ;
                              range                                     from_to              (size_type first, size_type last)                 ;
                              range                                     from_count           (size_type first, size_type count)                ;
                              const_range                               from_to_front        (size_type count)                           const ;
                              const_range                               from_to_back         (size_type count)                           const ;
                              const_range                               from_to              (size_type first, size_type last)           const ;
                              const_range                               from_count           (size_type first, size_type count)          const ;

                              void                                      shrink_to_fit        ()                                                ;
                              void                                      reserve              (size_type)                                       ;

                              void                                      increase_size_silent (size_type)                                       ;
                              void                                      resize_silent        (size_type)                                       ;

                              void                                      increase_size        (size_type)                                       ;
                              void                                      resize               (size_type)                                       ;

                              void                                      push_back_silent     (const_reference)                                 ;
                              void                                      push_back_silent     (reference)                                       ;
                              void                                      push_back_silent     (const_range)                                     ;
                              void                                      push_back_silent     (range)                                           ;
        template <typename U> void                                      push_back_silent     (U const&)                                        ;
        template <typename U> void                                      push_back_silent     (U const&, repeat_count)                          ;

                              void                                      push_back_jump       (const_range, size_type jump)                     ;

                              void                                      push_back            (const_reference)                                 ;
                              void                                      push_back            (reference)                                       ;
                              void                                      push_back            (const_range)                                     ;
                              void                                      push_back            (range)                                           ;
        template <typename U> void                                      push_back            (U const&)                                        ;
                              void                                      push_back            (const_reference, repeat_count)                   ;
                              void                                      push_back            (reference, repeat_count)                         ;
                              void                                      push_back            (const_range, repeat_count)                       ;
                              void                                      push_back            (range, repeat_count)                             ;
        template <typename U> void                                      push_back            (U const&, repeat_count)                          ;

                              void                                      pop_back             ()                                                ;
                              void                                      pop_back             (size_type)                                       ;

                              void                                      erase                (const_position)                                  ;
                              void                                      erase                (index_type)                                      ;
                              void                                      erase                (const_range)                                     ;
                              void                                      erase_from_to_front  (const_position)                                  ;
                              void                                      erase_from_to_front  (size_type)                                       ;
                              void                                      erase_from_to_back   (const_position)                                  ;
//                            void                                      erase_from_to_back   (size_type)                                       ;
//                            void                                      erase_from_to        (const_position, const_position)                  ;
//                            void                                      erase_from_to        (size_type, const_position)                       ;
//                            void                                      erase_from_to        (const_position, size_type)                       ;
//                            void                                      erase_from_to        (size_type, size_type)                            ;
//                            void                                      erase_from_count     (const_position, size_type)                       ;
//                            void                                      erase_from_count     (size_type, size_type)                            ;

                              void                                      clear                ()                                                ;

                              void                                      swap                 (self_type&)                                      ;

    private:

                              pointer                                   raw_first            ()                                                ;
                              const_pointer                             raw_first            ()                                          const ;
                              pointer                                   raw_last             ()                                                ;
                              const_pointer                             raw_last             ()                                          const ;

        typedef typename allocator_type::cluster_block cluster_block;

        cluster_block block_ ;
        pointer       ptr_   ;

        MSW_ON_DEBUG
        (
            zzz::validator_root validator_;
        )
    };
}
