#pragma once

#include <msw/config.hpp>
#include <msw/on_debug.hpp>
#include <msw/make_range.hpp>
#include <msw/algorithm/copy.hpp>
#include <msw/algorithm/copy_to_back.hpp>
#include <msw/zzz/buffer/decl.hpp>
#include <msw/zzz/position/def.hpp>
#include <msw/zzz/allocator/def.hpp>
#include <msw/zzz/range/def.hpp>
#ifdef MSW_DEBUG
    #include <msw/zzz/validator/root/def.hpp>
#endif


namespace msw
{
    template <typename T>
    buffer<T>::buffer()
    {}
    template <typename T>
    buffer<T>::buffer(size_type size)
#ifdef MSW_NO_DELEGATING_CONSTRUCTORS
        : block_(allocator_type::allocate(size))
        , ptr_(block_.first().template to_pointer<value_type>() + size)
        MSW_ON_DEBUG
        (
            , validator_(zzz::validator_root::init())
        )
#else
        : buffer<T>(size, size)
#endif
    {}
    template <typename T>
    buffer<T>::buffer(size_type size, size_type capacity)
        : block_(allocator_type::allocate(capacity))
        , ptr_(block_.first().template to_pointer<value_type>() + size)
        MSW_ON_DEBUG
        (
            , validator_(zzz::validator_root::init())
        )
    {
        MSW_ASSERT(capacity >= size);
    }

    template <typename T>
    buffer<T>::~buffer()
    {
        delete[] block_.first().native();
    }

    template <typename T>
    buffer<T>::buffer(self_type&& other)
        : block_(std::move(other.block_))
        , ptr_(std::move(other.ptr_))
        MSW_ON_DEBUG
        (
            , validator_(std::move(other.validator_))
        )
    {}
    template <typename T>
    typename buffer<T>::self_type& buffer<T>::operator = (self_type&& other)
    {
        if (&other != this)
        {
            delete[] block_.first().native();
            block_ = std::move(other.block_);
            ptr_ = std::move(other.ptr_);
            MSW_ON_DEBUG
            (
                validator_ = std::move(other.validator_);
            )
        }
        return *this;
    }

    template <typename T>
    buffer<T>::buffer(self_type const& other)
#ifdef MSW_NO_DELEGATING_CONSTRUCTORS
        : block_(allocator_type::allocate(other.size()))
        , ptr_(block_.first().template to_pointer<value_type>() + other.size())
        MSW_ON_DEBUG
        (
            , validator_(zzz::validator_root::init())
        )
#else
        : buffer<T>(other.size())
#endif
    {
        copy(other, *this);
    }
    template <typename T>
    typename buffer<T>::self_type& buffer<T>::operator = (self_type const& other)
    {
        if (capacity() < other.size())
            *this = self_type(other);
        else
            assign_silent(other.all());
        return *this;
    }

    template <typename T>
    buffer<T>::buffer(const_range r)
#ifdef MSW_NO_DELEGATING_CONSTRUCTORS
        : block_(allocator_type::allocate(r.size()))
        , ptr_(block_.first().template to_pointer<value_type>() + r.size())
        MSW_ON_DEBUG
        (
            , validator_(zzz::validator_root::init())
        )
#else
        : buffer<T>(r.size())
#endif
    {
        copy(r, *this);
    }
    template <typename T>
    typename buffer<T>::self_type& buffer<T>::operator = (const_range r)
    {
        if (capacity() < r.size())
            *this = self_type(r);
        else
            assign_silent(r);
        return *this;
    }
    template <typename T>
    buffer<T>::buffer(range r)
#ifdef MSW_NO_DELEGATING_CONSTRUCTORS
        : block_(allocator_type::allocate(r.size()))
        , ptr_(block_.first().template to_pointer<value_type>() + r.size())
        MSW_ON_DEBUG
        (
            , validator_(zzz::validator_root::init())
        )
#else
        : buffer<T>(r.size())
#endif
    {
        copy(r, *this);
    }
    template <typename T>
    typename buffer<T>::self_type& buffer<T>::operator = (range r)
    {
        if (capacity() < r.size())
            *this = self_type(r);
        else
            assign_silent(r);
        return *this;
    }

    template <typename T>
    typename buffer<T>::self_type& buffer<T>::assign_silent(const_range r)
    {
        resize_silent(r.size());
        copy(r, *this);
        return *this;
    }
    template <typename T>
    typename buffer<T>::self_type& buffer<T>::assign_silent(range r)
    {
        resize_silent(r.size());
        copy(r, *this);
        return *this;
    }

    template <typename T>
    typename buffer<T>::pointer buffer<T>::data()
    {
        return raw_first();
    }
    template <typename T>
    template <typename U>
    typename plain_traits<U>::pointer buffer<T>::data()
    {
        return data().template to_pointer<U>();
    }
    template <typename T>
    typename buffer<T>::const_pointer buffer<T>::data() const
    {
        return raw_first();
    }
    template <typename T>
    template <typename U>
    typename plain_traits<U>::const_pointer buffer<T>::data() const
    {
        return data().template to_pointer<U>();
    }
    template <typename T>
    typename buffer<T>::size_type buffer<T>::size() const
    {       
        return ptr_ - raw_first();
    }
    template <typename T>
    template <typename U>
    typename plain_traits<U>::size_type buffer<T>::size() const
    {
        return typename plain_traits<U>::size_type(size());
    }
    template <typename T>
    bool buffer<T>::empty() const
    {
        return ptr_ == raw_first();
    }
    template <typename T>
    bool buffer<T>::has_items() const
    {
        return !empty();
    }
    template <typename T>
    typename buffer<T>::size_type buffer<T>::capacity() const
    {
        return raw_last() - raw_first();
    }
    template <typename T>
    template <typename U>
    typename plain_traits<U>::size_type buffer<T>::capacity() const
    {
        return typename plain_traits<U>::size_type(capacity());
    }
    template <typename T>
    typename buffer<T>::size_type buffer<T>::free_space() const
    {
        return raw_last() - ptr_;
    }
    template <typename T>
    template <typename U>
    typename plain_traits<U>::size_type buffer<T>::free_space() const
    {
        return typename plain_traits<U>::size_type(capacity());
    }
    template <typename T>
    bool buffer<T>::no_free_space() const
    {
        return ptr_ == raw_last();
    }
    template <typename T>
    bool buffer<T>::has_free_space() const
    {
        return ptr_ != raw_last();
    }

    template <typename T>
    typename buffer<T>::range buffer<T>::all()
    {
        return zzz::construct_range(zzz::make_block(raw_first(), ptr_) MSW_ON_DEBUG(, validator_.make_link()));
    }
    template <typename T>
    template <typename U>
    msw::range<U> buffer<T>::all()
    {
        return all().template all<U>();
    }
    template <typename T>
    typename buffer<T>::const_range buffer<T>::all() const
    {
        return zzz::construct_range(zzz::make_block(raw_first(), ptr_) MSW_ON_DEBUG(, validator_.make_link()));
    }
    template <typename T>
    template <typename U>
    msw::range<U const> buffer<T>::all() const
    {
        return all().template all<U>();
    }

    template <typename T>
    typename buffer<T>::position buffer<T>::begin()
    {
        return zzz::construct_position(raw_first() MSW_ON_DEBUG(, validator_.make_link()));
    }
    template <typename T>
    typename buffer<T>::position buffer<T>::begin(index_type index)
    {
        MSW_ASSERT(size() > size_type(index));
        return zzz::construct_position(raw_first() + index MSW_ON_DEBUG(, validator_.make_link()));
    }
    //template <typename T>
    //template <typename U>
    //msw::position<U> buffer<T>::begin()
    //{
    //  return begin().to_position<U>();
    //}
    //template <typename T>
    //template <typename U>
    //msw::position<U> buffer<T>::begin(index_type index)
    //{
    //  return all<U>().begin(index);
    //}
    template <typename T>
    typename buffer<T>::const_position buffer<T>::begin() const
    {
        return zzz::construct_position(raw_first() MSW_ON_DEBUG(, validator_.make_link()));
    }
    template <typename T>
    typename buffer<T>::const_position buffer<T>::begin(index_type index) const
    {
        MSW_ASSERT(size() > size_type(index));
        return zzz::construct_position(raw_first() + index MSW_ON_DEBUG(, validator_.make_link()));
    }
    //template <typename T>
    //template <typename U>
    //msw::position<U const> buffer<T>::begin() const
    //{
    //  return begin().to_position<U>();
    //}
    //template <typename T>
    //template <typename U>
    //msw::position<U const> buffer<T>::begin(index_type index) const
    //{
    //  return all<U>().begin(index);
    //}

    template <typename T>
    typename buffer<T>::position buffer<T>::end()
    {
        return zzz::construct_position(ptr_ MSW_ON_DEBUG(, validator_.make_link()));
    }
    template <typename T>
    typename buffer<T>::position buffer<T>::end(index_type index)
    {
        MSW_ASSERT(size() > size_type(index));
        return zzz::construct_position(ptr_ - index MSW_ON_DEBUG(, validator_.make_link()));
    }
    //template <typename T>
    //template <typename U>
    //msw::position<U> buffer<T>::end()
    //{
    //  return end().to_position<U>();
    //}
    template <typename T>
    typename buffer<T>::const_position buffer<T>::end() const
    {
        return zzz::construct_position(ptr_ MSW_ON_DEBUG(, validator_.make_link()));
    }
    template <typename T>
    typename buffer<T>::const_position buffer<T>::end(index_type index) const
    {
        MSW_ASSERT(size() > size_type(index));
        return zzz::construct_position(ptr_ - index MSW_ON_DEBUG(, validator_.make_link()));
    }
    //template <typename T>
    //template <typename U>
    //msw::position<U const> buffer<T>::end() const
    //{
    //  return end().to_position<U const>();
    //}

    template <typename T>
    typename buffer<T>::reference buffer<T>::item(index_type index)
    {
        MSW_ASSERT(size() > size_type(index));
        return data()[index];
    }
    template <typename T>
    template <typename U>
    typename plain_traits<U>::reference buffer<T>::item(index_type index)
    {
        return all().item(index);
    }
    template <typename T>
    typename buffer<T>::const_reference buffer<T>::item(index_type index) const
    {
        MSW_ASSERT(size() > size_type(index));
        return data()[index];
    }
    template <typename T>
    template <typename U>
    typename plain_traits<U>::const_reference buffer<T>::item(index_type index) const
    {
        return all().item(index);
    }

    template <typename T>
    typename buffer<T>::reference buffer<T>::operator[](index_type index)
    {
        MSW_ASSERT(size() > size_type(index));
        return data()[index];
    }
    template <typename T>
    typename buffer<T>::const_reference buffer<T>::operator[](index_type index) const
    {
        MSW_ASSERT(size() > size_type(index));
        return data()[index];
    }

    template <typename T>
    typename buffer<T>::reference buffer<T>::front()
    {
        return all().front();
    }
    template <typename T>
    template <typename U>
    typename plain_traits<U>::reference buffer<T>::front()
    {
        return all().template front<U>();
    }
    template <typename T>
    typename buffer<T>::range buffer<T>::front(size_type count)
    {
        return all().front(count);
    }
    template <typename T>
    template <typename U>
    typename plain_traits<U>::reference buffer<T>::front(typename plain_traits<U>::size_type count)
    {
        return all().front(count);
    }
    template <typename T>
    typename buffer<T>::range buffer<T>::front_max(size_type count)
    {
        return all().front_max(count);
    }
    template <typename T>
    template <typename U>
    msw::range<U> buffer<T>::front_max(typename plain_traits<U>::size_type count)
    {
        return all().template front_max<U>(count);
    }
    template <typename T>
    template <typename U>
    msw::range<U> buffer<T>::front_max()
    {
        return all().template front_max<U>();
    }
    template <typename T>
    typename buffer<T>::const_reference buffer<T>::front() const
    {
        return all().front();
    }
    template <typename T>
    template <typename U>
    typename plain_traits<U>::const_reference buffer<T>::front() const
    {
        return all().template front<U>();
    }
    template <typename T>
    typename buffer<T>::const_range buffer<T>::front(size_type count) const
    {
        return all().front(count);
    }
    template <typename T>
    template <typename U>
    typename plain_traits<U>::const_reference buffer<T>::front(typename plain_traits<U>::size_type count) const
    {
        return all().front(count);
    }
    template <typename T>
    typename buffer<T>::const_range buffer<T>::front_max(size_type count) const
    {
        return all().front_max(count);
    }
    template <typename T>
    template <typename U>
    msw::range<U const> buffer<T>::front_max(typename plain_traits<U>::size_type count) const
    {
        return all().template front_max<U>(count);
    }
    template <typename T>
    template <typename U>
    msw::range<U const> buffer<T>::front_max() const
    {
        return all().template front_max<U>();
    }

    template <typename T>
    typename buffer<T>::reference buffer<T>::back()
    {
        return all().back();
    }
    template <typename T>
    template <typename U>
    typename plain_traits<U>::reference buffer<T>::back()
    {
        return all().template back<U>();
    }
    template <typename T>
    typename buffer<T>::range buffer<T>::back(size_type count)
    {
        return all().back(count);
    }
    template <typename T>
    template <typename U>
    typename plain_traits<U>::reference buffer<T>::back(typename plain_traits<U>::size_type count)
    {
        return all().back(count);
    }
    template <typename T>
    typename buffer<T>::range buffer<T>::back_max(size_type count)
    {
        return all().back_max(count);
    }
    template <typename T>
    template <typename U>
    msw::range<U> buffer<T>::back_max(typename plain_traits<U>::size_type count)
    {
        return all().template back_max<U>(count);
    }
    template <typename T>
    template <typename U>
    msw::range<U> buffer<T>::back_max()
    {
        return all().template back_max<U>();
    }
    template <typename T>
    typename buffer<T>::const_reference buffer<T>::back() const
    {
        return all().back();
    }
    template <typename T>
    template <typename U>
    typename plain_traits<U>::const_reference buffer<T>::back() const
    {
        return all().template back<U>();
    }
    template <typename T>
    typename buffer<T>::const_range buffer<T>::back(size_type count) const
    {
        return all().back(count);
    }
    template <typename T>
    template <typename U>
    typename plain_traits<U>::const_reference buffer<T>::back(typename plain_traits<U>::size_type count) const
    {
        return all().back(count);
    }
    template <typename T>
    typename buffer<T>::const_range buffer<T>::back_max(size_type count) const
    {
        return all().back_max(count);
    }
    template <typename T>
    template <typename U>
    msw::range<U const> buffer<T>::back_max(typename plain_traits<U>::size_type count) const
    {
        return all().template back_max<U>(count);
    }
    template <typename T>
    template <typename U>
    msw::range<U const> buffer<T>::back_max() const
    {
        return all().template back_max<U>();
    }

    template <typename T>
    typename buffer<T>::range buffer<T>::from_to_front(size_type count)
    {
        return all().from_to_front(count);
    }
    template <typename T>
    typename buffer<T>::range buffer<T>::from_to_back(size_type count)
    {
        return all().from_to_back(count);
    }
    template <typename T>
    typename buffer<T>::range buffer<T>::from_to(size_type first, size_type last)
    {
        return all().from_to(first, last);
    }
    template <typename T>
    typename buffer<T>::range buffer<T>::from_count(size_type first, size_type count)
    {
        return all().from_count(first, count);
    }
    template <typename T>
    typename buffer<T>::const_range buffer<T>::from_to_front(size_type count) const
    {
        return all().from_to_front(count);
    }
    template <typename T>
    typename buffer<T>::const_range buffer<T>::from_to_back(size_type count) const
    {
        return all().from_to_back(count);
    }
    template <typename T>
    typename buffer<T>::const_range buffer<T>::from_to(size_type first, size_type last) const
    {
        return all().from_to(first, last);
    }
    template <typename T>
    typename buffer<T>::const_range buffer<T>::from_count(size_type first, size_type count) const
    {
        return all().from_count(first, count);
    }

    template <typename T>
    void buffer<T>::shrink_to_fit()
    {
        *this = self_type(*this);
    }
    template <typename T>
    void buffer<T>::reserve(size_type count)
    {
        if (capacity() < count)
        {
            self_type tmp(size(), count);
            copy(*this, tmp);
            swap(tmp);
        }
    }

    template <typename T>
    void buffer<T>::increase_size_silent(size_type count)
    {
        MSW_ASSERT((raw_last() - ptr_) >= count);
        ptr_ += count;
    }
    template <typename T>
    void buffer<T>::resize_silent(size_type count)
    {
        MSW_ASSERT(capacity() >= count);
        ptr_ = raw_first() + count;
    }

    template <typename T>
    void buffer<T>::increase_size(size_type count)
    {
        resize(size() + count);
    }
    template <typename T>
    void buffer<T>::resize(size_type count)
    {
        reserve(count);
        resize_silent(count);
    }

    template <typename T>
    void buffer<T>::push_back_silent(const_reference v)
    {
        MSW_ASSERT(has_free_space());
        *ptr_++ = v;
    }
    template <typename T>
    void buffer<T>::push_back_silent(reference v)
    {
        push_back_silent(static_cast<const_reference>(v));
    }
    template <typename T>
    void buffer<T>::push_back_silent(const_range r)
    {
        increase_size_silent(r.size());
        copy_to_back(r, *this);
    }
    template <typename T>
    void buffer<T>::push_back_silent(range r)
    {
        push_back_silent(static_cast<const_range>(r));
    }
    template <typename T>
    template <typename U>
    void buffer<T>::push_back_silent(U const& what)
    {
        push_back_silent(make_range<value_type>(what));
    }
    template <typename T>
    template <typename U>
    void buffer<T>::push_back_silent(U const& what, repeat_count count)
    {
        while(count--) push_back_silent(what);
    }

    template <typename T>
    void buffer<T>::push_back_jump(const_range r, size_type jump)
    {
        if (free_space() >= r.size())
        {
            push_back_silent(r);
        }
        else
        {
            self_type tmp(size() + std::max(r.size(), jump));
            copy(*this, tmp);
            copy_to_back(r, tmp);
            swap(tmp);
        }
    }

    template <typename T>
    void buffer<T>::push_back(const_reference v)
    {
        if (no_free_space())
        {
            self_type tmp(size() + 1);
            copy(*this, tmp);
            tmp.back() = v;
            swap(tmp);
        }
        else *ptr_++ = v;
    }
    template <typename T>
    void buffer<T>::push_back(reference v)
    {
        push_back(static_cast<const_reference>(v));
    }
    template <typename T>
    void buffer<T>::push_back(const_range r)
    {
        if (free_space() >= r.size())
        {
            push_back_silent(r);
        }
        else
        {
            self_type tmp(size() + r.size());
            copy(*this, tmp);
            copy_to_back(r, tmp);
            swap(tmp);
        }
    }
    template <typename T>
    void buffer<T>::push_back(range r)
    {
        push_back(static_cast<const_range>(r));
    }
    template <typename T>
    template <typename U>
    void buffer<T>::push_back(U const& what)
    {
        push_back(make_range(what).template all<value_type>());
    }
    template <typename T>
    void buffer<T>::push_back(const_reference v, repeat_count count)
    {
        if (free_space() >= size_type(count))
        {
            push_back_silent(v, count);
        }
        else
        {
            self_type tmp(size(), size() + size_type(count));
            copy(*this, tmp);
            tmp.push_back_silent(v, count);
            swap(tmp);
        }
    }
    template <typename T>
    void buffer<T>::push_back(reference v, repeat_count count)
    {
        push_back(static_cast<const_reference>(v), count);
    }
    template <typename T>
    void buffer<T>::push_back(const_range r, repeat_count count)
    {
        if (free_space() >= (r.size() * count))
        {
            push_back_silent(r, count);
        }
        else
        {
            self_type tmp(size(), size() + r.size() * count);
            copy(*this, tmp);
            copy_to_back(r, tmp);
            tmp.push_back_silent(r, count);
            swap(tmp);
        }
    }
    template <typename T>
    void buffer<T>::push_back(range r, repeat_count count)
    {
        push_back(static_cast<const_range>(r), count);
    }
    template <typename T>
    template <typename U>
    void buffer<T>::push_back(U const& what, repeat_count count)
    {
        push_back(make_range(what).template all<T>(), count);
    }

    template <typename T>
    void buffer<T>::pop_back()
    {
        MSW_ASSERT(ptr_ != raw_first());
        --ptr_;
    }
    template <typename T>
    void buffer<T>::pop_back(size_type count)
    {
        MSW_ASSERT(size() >= count);
        ptr_ -= count;
    }

    template <typename T>
    void buffer<T>::erase(const_position what)
    {
        MSW_ASSERT((what.to_pointer() >= raw_first()) && (what.to_pointer() < ptr_));
        const_range src = what + end();
        src.pop_front();
        //copy(src, const_cast(what) + end());
        copy(src, back(src.size() + 1));
        --ptr_;
    }
    template <typename T>
    void buffer<T>::erase(index_type index)
    {
        erase(begin(index));
    }
    template <typename T>
    void buffer<T>::erase(const_range what)
    {
        //copy(what.end() + end(), const_cast(what.begin()) + end());
        copy(what.end() + end(), begin((what.begin() - begin()).count()) + end());
        ptr_ -= what.size();
    }
    template <typename T>
    void buffer<T>::erase_from_to_front(const_position pos)
    {
        erase_from_to_front(pos - begin());
    }
    template <typename T>
    void buffer<T>::erase_from_to_front(size_type pos)
    {
        MSW_ASSERT(size() >= pos);
        copy(begin(pos.count()) + end(), *this);
        ptr_ -= pos.count();
    }
    template <typename T>
    void buffer<T>::erase_from_to_back(const_position new_end)
    {
        ptr_ = new_end.to_pointer().remove_const();
    }

    template <typename T>
    void buffer<T>::clear()
    {
        ptr_ = raw_first();
    }

    template <typename T>
    void buffer<T>::swap(self_type& other)
    {
        block_.swap(other.block_);
        ptr_.swap(other.ptr_);
        MSW_ON_DEBUG
        (
            validator_.swap(other.validator_);
        )
    }


    template <typename T>
    typename buffer<T>::pointer buffer<T>::raw_first()
    {
        return block_.first().template to_pointer<value_type>();
    }
    template <typename T>
    typename buffer<T>::const_pointer buffer<T>::raw_first() const
    {
        return block_.first().template to_pointer<value_type>();
    }
    template <typename T>
    typename buffer<T>::pointer buffer<T>::raw_last()
    {
        return block_.last().template to_pointer<value_type>();
    }
    template <typename T>
    typename buffer<T>::const_pointer buffer<T>::raw_last() const
    {
        return block_.last().template to_pointer<value_type>();
    }
}
