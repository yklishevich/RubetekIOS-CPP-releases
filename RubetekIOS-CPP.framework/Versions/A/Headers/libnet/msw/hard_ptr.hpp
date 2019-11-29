#pragma once 

#include <array>
#include <type_traits>

#include <msw/assert.hpp>
#include <msw/noncopyable.hpp>
#include <msw/algorithm/copy.hpp>
#include <msw/dimension/trigger.hpp>


namespace msw
{
    struct enable_hard_move {};
    struct hard_ptr_create_tag
    {
        hard_ptr_create_tag()
        {}
    };
    hard_ptr_create_tag const hard_ptr_create;

    template <typename T>
    struct hard_ptr
        : noncopyable
    {
        static_assert(std::is_base_of<enable_hard_move, T>::value, "T must be inherited from enable_hard_move");

        typedef hard_ptr<T> self_type       ;
        typedef T           value_type      ;
        typedef T*          pointer         ;
        typedef T const*    const_pointer   ;
        typedef T&          reference       ;
        typedef T const&    const_reference ;

        hard_ptr()
            : has_(false)
        {}
        ~hard_ptr()
        {
            destroy();
        }

        template <typename ...Args>
        explicit hard_ptr(hard_ptr_create_tag, Args&&... args)
            : has_(false)
        {
            create(std::forward<Args>(args)...);
        }

        hard_ptr(self_type&& other)
            : has_(std::move(other.has_))
        {
            copy(other.m_, m_);
        }
        self_type& operator = (self_type&& other)
        {
            if (&other != this)
            {
                copy(other.m_, m_);
                has_ = std::move(other.has_);
            }
            return *this;
        }

        explicit operator bool () const
        {
            return static_cast<bool>(has_);
        }

        void destroy()
        {
            if (has_) ptr()->~value_type();
            has_.reset();
        }
        template <typename ...Args>
        void create(Args&&... /*args*/)
        {
            //destroy();
            //MSW_ON_DEBUG(void* p = new (native_ptr()) value_type(std::forward<Args>(args)...));
            //MSW_ASSERT(p == native_ptr());
            //has_.set();
        }

        const_pointer operator -> () const
        {
            return cptr();
        }
        pointer operator -> ()
        {
            return ptr();
        }

        const_reference operator * () const
        {
            return *cptr();
        }
        reference operator * ()
        {
            return *ptr();
        }

    private:

        const_pointer cptr() const
        {
            MSW_ASSERT(has_);
            return reinterpret_cast<const_pointer>(m_.data());
        }
        pointer ptr()
        {
            MSW_ASSERT(has_);
            return reinterpret_cast<pointer>(m_.data());
        }
        void* native_ptr()
        {
            return m_.data();
        }

        trigger                                         has_;
        std::array<unsigned char, sizeof(value_type)>   m_  ;
    };
}
