#pragma once

#include <type_traits>                        // std::conditional, std::is_const, std::is_same

#include <msw/on_debug.hpp>
#include <msw/plain_traits.hpp>
#include <msw/plain_types.hpp>                // bit
#include <msw/zzz/block/make.hpp>
#include <msw/zzz/position/def.hpp>
#include <msw/zzz/range/decl.hpp>
#ifdef MSW_DEBUG
    #include <msw/zzz/validator/link/def.hpp>
#endif


namespace msw
{
    template <typename T>
    range<T>::range()
    {}

    template <typename T>
    range<T>::range(self_type const& other)
        : block_(other.block_)
        MSW_ON_DEBUG
        (
            , validator_(other.validator_)
        )
    {}
    template <typename T>
    typename range<T>::self_type& range<T>::operator = (self_type const& other)
    {
        block_ = other.block_;
        MSW_ON_DEBUG
        (
            validator_ = other.validator_;
        )
        return *this;
    }

    template <typename T>
    range<T>::range(self_type&& other)
        : block_(std::move(other.block_))
        MSW_ON_DEBUG
        (
            , validator_(std::move(other.validator_))
        )
    {}
    template <typename T>
    typename range<T>::self_type& range<T>::operator = (self_type&& other)
    {
        block_ = std::move(other.block_);
        MSW_ON_DEBUG
        (
            validator_ = std::move(other.validator_);
        )
        return *this;
    }

    template <typename T>
    template <typename U>
    range<T>::range(range<U> const& other)
        : block_(other.block_)
        MSW_ON_DEBUG
        (
            , validator_(other.validator_)
        )
    {}
    template <typename T>
    template <typename U>
    typename range<T>::self_type& range<T>::operator = (range<U> const& other)
    {
        block_ = other.block_;
        MSW_ON_DEBUG
        (
            validator_ = other.validator_;
        )
        return *this;
    }

    template <typename T>
    template <typename U>
    range<T>::range(range<U>&& other)
        : block_(std::move(other.block_))
        MSW_ON_DEBUG
        (
            , validator_(std::move(other.validator_))
        )
    {}
    template <typename T>
    template <typename U>
    typename range<T>::self_type& range<T>::operator = (range<U>&& other)
    {
        block_ = std::move(other.block_);
        MSW_ON_DEBUG
        (
            validator_ = std::move(other.validator_);
        )
        return *this;
    }

    template <typename T>
    typename range<T>::pointer range<T>::data() const
    {
        MSW_ASSERT(check());
        return block_.first();
    }
    template <typename T>
    template <typename U>
    typename zzz::range_redused<T, U>::pointer range<T>::data() const
    {
        return data().template to_pointer<U>();
    }
    template <typename T>
    typename range<T>::size_type range<T>::size() const
    {
        MSW_ASSERT(check());
        return block_.size();
    }
    template <typename T>
    template <typename U>
    typename zzz::range_redused<T, U>::size_type range<T>::size() const
    {
        return typename zzz::range_redused<T, U>::size_type(size());
    }
    template <typename T>
    bool range<T>::empty() const
    {
        MSW_ASSERT(check());
        return block_.empty();
    }
    template <typename T>
    bool range<T>::has_items() const
    {
        return !empty();
    }

    template <typename T>
    typename range<T>::reference range<T>::item(index_type index) const
    {
        return *this[index];
    }
    template <typename T>
    template <typename U>
    typename zzz::range_redused<T, U>::reference range<T>::item(index_type index) const
    {
        typename plain_traits<U>::size_type offset(index);
        MSW_ASSERT(front_max<U>().size() > offset);
        return *(block_.first().template to_pointer<U>() + offset);
    }

    template <typename T>
    typename range<T>::reference range<T>::operator[](index_type index) const
    {
        MSW_ASSERT(size() > size_type(index));
        return block_.first()[index];
    }

    template <typename T>
    template <typename U>
    typename zzz::range_redused<T, U>::reference range<T>::as() const
    {
        MSW_ASSERT(size() == typename plain_traits<U>::size_type(1));
        return *block_.first().template to_pointer<U>();
    }

    template <typename T>
    typename range<T>::position range<T>::begin() const
    {
        return zzz::construct_position(block_.first() MSW_ON_DEBUG(, validator_));
    }
    template <typename T>
    typename range<T>::position range<T>::begin(index_type index) const
    {
        MSW_ASSERT(size() > size_type(index));
        return zzz::construct_position(block_.first() + index MSW_ON_DEBUG(, validator_));
    }
    //template <typename T>
    //template <typename U>
    //typename zzz::range_redused<T, U>::position range<T>::begin() const
    //{
    //  return begin().to_position<U>();
    //}
    //template <typename T>
    //template <typename U>
    //typename zzz::range_redused<T, U>::position range<T>::begin(index_type index) const
    //{
    //  return all<U>().begin(index);
    //}
    template <typename T>
    typename range<T>::position range<T>::end() const
    {
        return zzz::construct_position(block_.last() MSW_ON_DEBUG(, validator_));
    }
    template <typename T>
    typename range<T>::position range<T>::end(index_type index) const
    {
        MSW_ASSERT(size() > size_type(index));
        return zzz::construct_position(block_.last() - index MSW_ON_DEBUG(, validator_));
    }
    //template <typename T>
    //template <typename U>
    //typename zzz::range_redused<T, U>::position range<T>::end() const
    //{
    //  return end().to_position<U>();
    //}
    //template <typename T>
    //template <typename U>
    //typename zzz::range_redused<T, U>::position range<T>::end(index_type index) const
    //{
    //  return end().to_position<U>();
    //}

    template <typename T>
    typename range<T>::self_type range<T>::all() const
    {
        MSW_WARY_ASSERT(check());
        return *this;
    }
    template <typename T>
    template <typename U>
    typename zzz::range_redused<T, U>::type range<T>::all() const
    {
        MSW_WARY_ASSERT(check());
        return zzz::construct_range(block_.template to_block<typename zzz::range_redused<T, U>::value_type>() MSW_ON_DEBUG(, validator_));
    }

    template <typename T>
    typename range<T>::reference range<T>::front() const
    {
        MSW_ASSERT(has_items());
        return *block_.first();
    }
    template <typename T>
    template <typename U>
    typename zzz::range_redused<T, U>::reference range<T>::front() const
    {
        MSW_ASSERT(size() >= typename plain_traits<U>::size_type(1));
        return *block_.first().template to_pointer<U>();
    }
    template <typename T>
    typename range<T>::self_type range<T>::front(size_type count) const
    {
        MSW_ASSERT(size() >= count);
        return self_type(zzz::make_block(block_.first(), count) MSW_ON_DEBUG(, validator_), zzz::construct_range_tag());
    }
    template <typename T>
    template <typename U>
    typename zzz::range_redused<T, U>::type range<T>::front(typename zzz::range_redused<T, U>::size_type count) const
    {
        MSW_ASSERT(size() >= count);
        typedef typename zzz::range_redused<T, U>::value_type dest_value_type;
        return zzz::construct_range(zzz::make_block(block_.first().template to_pointer<dest_value_type>(), count) MSW_ON_DEBUG(, validator_));
    }

    template <typename T>
    typename range<T>::self_type range<T>::front_max(size_type count) const
    {
        return front(size().min(count));
    }
    template <typename T>
    template <typename U>
    typename zzz::range_redused<T, U>::type range<T>::front_max(typename zzz::range_redused<T, U>::size_type count) const
    {
        return front_max<U>().crop_front_max(count);
    }
    template <typename T>
    template <typename U>
    typename zzz::range_redused<T, U>::type range<T>::front_max() const
    {
        MSW_WARY_ASSERT(check());
        auto u_first = block_.first().template to_pointer<U>();
        return zzz::construct_range(zzz::make_block(u_first, u_first.upper_adjacent_until(block_.last().template to_pointer<U>())) MSW_ON_DEBUG(, validator_));
    }
    template <>
    template <typename U>
    typename zzz::range_redused<bit, U>::type range<bit>::front_max() const
    {
        MSW_WARY_ASSERT(check());
        auto u_first = block_.first().template to_pointer<U>();
        return zzz::construct_range(zzz::make_block(u_first, u_first.upper_adjacent_until(block_.last().base_ptr().template to_pointer<U>())) MSW_ON_DEBUG(, validator_));
    }
    template <>
    template <typename U>
    typename zzz::range_redused<bit const, U>::type range<bit const>::front_max() const
    {
        MSW_WARY_ASSERT(check());
        auto u_first = block_.first().template to_pointer<U>();
        return zzz::construct_range(zzz::make_block(u_first, u_first.upper_adjacent_until(block_.last().base_ptr().template to_pointer<U>())) MSW_ON_DEBUG(, validator_));
    }

    template <typename T>
    typename range<T>::reference range<T>::back() const
    {
        MSW_ASSERT(has_items());
        return *(block_.last() - 1);
    }
    template <typename T>
    template <typename U>
    typename zzz::range_redused<T, U>::reference range<T>::back() const
    {
        return back_max<U>().back();
    }
    template <typename T>
    typename range<T>::self_type range<T>::back(size_type count) const
    {
        MSW_ASSERT(size() >= count);
        return self_type(zzz::make_block(count, block_.last()) MSW_ON_DEBUG(, validator_), zzz::construct_range_tag());
    }
    template <typename T>
    template <typename U>
    typename zzz::range_redused<T, U>::type range<T>::back(typename zzz::range_redused<T, U>::size_type count) const
    {
        MSW_ASSERT(size() >= count);
        typedef typename zzz::range_redused<T, U>::value_type dest_value_type;
        return zzz::construct_range(zzz::make_block(count, block_.last().template to_pointer<dest_value_type>()) MSW_ON_DEBUG(, validator_));
    }
    template <typename T>
    typename range<T>::self_type range<T>::back_max(size_type count) const
    {
        return back(size().min(count));
    }
    template <typename T>
    template <typename U>
    typename zzz::range_redused<T, U>::type range<T>::back_max(typename zzz::range_redused<T, U>::size_type count) const
    {
        return back_max<U>().crop_back_max(count);
    }
    template <typename T>
    template <typename U>
    typename zzz::range_redused<T, U>::type range<T>::back_max() const
    {
        MSW_WARY_ASSERT(check());
        auto u_last = block_.last().template to_pointer<U>();
        return zzz::construct_range(zzz::make_block(u_last.lower_adjacent_after(block_.first().template to_pointer<U>()), u_last) MSW_ON_DEBUG(, validator_));
    }
    template <>
    template <typename U>
    typename zzz::range_redused<bit, U>::type range<bit>::back_max() const
    {
        MSW_WARY_ASSERT(check());
        auto u_last = block_.last().template to_pointer<U>();
        return zzz::construct_range(zzz::make_block(u_last.lower_adjacent_after(block_.first().base_ptr().template to_pointer<U>()), u_last) MSW_ON_DEBUG(, validator_));
    }
    template <>
    template <typename U>
    typename zzz::range_redused<bit const, U>::type range<bit const>::back_max() const
    {
        MSW_WARY_ASSERT(check());
        auto u_last = block_.last().template to_pointer<U>();
        return zzz::construct_range(zzz::make_block(u_last.lower_adjacent_after(block_.first().base_ptr().template to_pointer<U>()), u_last) MSW_ON_DEBUG(, validator_));
    }

    template <typename T>
    typename range<T>::self_type range<T>::from_to_front(size_type count) const
    {
        MSW_ASSERT(size() >= count);
        return self_type(zzz::make_block(block_.first(), count) MSW_ON_DEBUG(, validator_), zzz::construct_range_tag());
    }
    template <typename T>
    typename range<T>::self_type range<T>::from_to_back(size_type count) const
    {
        MSW_ASSERT(size() >= count);
        return self_type(zzz::make_block(block_.first() + count, block_.last()) MSW_ON_DEBUG(, validator_), zzz::construct_range_tag());
    }
    template <typename T>
    typename range<T>::self_type range<T>::from_back_to_front(size_type count) const
    {
        MSW_ASSERT(size() >= count);
        return self_type(zzz::make_block(block_.first(), block_.last() - count) MSW_ON_DEBUG(, validator_), zzz::construct_range_tag());
    }
    template <typename T>
    typename range<T>::self_type range<T>::from_to(size_type first, size_type last) const
    {
        MSW_ASSERT(size() > last);
        return self_type(zzz::make_block(block_.first() + first, block_.first() + last) MSW_ON_DEBUG(, validator_), zzz::construct_range_tag());
    }
    template <typename T>
    typename range<T>::self_type range<T>::from_count(size_type first, size_type count) const
    {
        MSW_ASSERT(size() >= (first + count));
        return self_type(zzz::make_block(block_.first() + first, count) MSW_ON_DEBUG(, validator_), zzz::construct_range_tag());
    }

    template <typename T>
    typename range<T>::reference range<T>::pop_front()
    {
        reference r = front();
        block_.inc_first();
        return r;
    }
    template <typename T>
    template <typename U>
    typename zzz::range_redused<T, U>::reference range<T>::pop_front()
    {
        static_assert
        (
            std::is_same<typename zzz::size_cast::policy<T, U>::type, zzz::size_cast::policy_enum::inc_base_and_even>::value
            || std::is_same<typename zzz::size_cast::policy<T, U>::type, zzz::size_cast::policy_enum::same>::value
            || std::is_same<typename zzz::size_cast::policy<T, U>::type, zzz::size_cast::policy_enum::bit_to_byte>::value
            , "U should be not less and even than T"
        );
        typename zzz::range_redused<T, U>::reference r = front<U>();
        block_.inc_first(typename plain_traits<U>::size_type(1));
        return r;
    }
    template <typename T>
    typename range<T>::self_type range<T>::pop_front(size_type count)
    {
        self_type tmp(front(count));
        block_.set_first(tmp.block_.last());
        return tmp;
    }
    template <typename T>
    template <typename U>
    typename zzz::range_redused<T, U>::type range<T>::pop_front(typename zzz::range_redused<T, U>::size_type count)
    {
        typename zzz::range_redused<T, U>::type u_range = front<U>(count);
        pop_front(count);
        return u_range;
    }
    template <typename T>
    typename range<T>::self_type range<T>::pop_front_max(size_type count)
    {
        return pop_front(size().min(count));
    }
    template <typename T>
    template <typename U>
    typename zzz::range_redused<T, U>::type range<T>::pop_front_max(typename zzz::range_redused<T, U>::size_type count)
    {
        typename zzz::range_redused<T, U>::type u_range = front_max<U>(count);
        pop_front(u_range.size());
        return u_range;
    }
    template <typename T>
    template <typename U>
    typename zzz::range_redused<T, U>::type range<T>::pop_front_max()
    {
        typename zzz::range_redused<T, U>::type u_range = front_max<U>();
        pop_front(u_range.size());
        return u_range;
    }

    template <typename T>
    typename range<T>::reference range<T>::pop_back()
    {
        reference r = back();
        block_.dec_last();
        return r;
    }
    template <typename T>
    template <typename U>
    typename zzz::range_redused<T, U>::reference range<T>::pop_back()
    {
        static_assert
        (
            std::is_same<typename zzz::size_cast::policy<T, U>::type, zzz::size_cast::policy_enum::inc_base_and_even>::value
            || std::is_same<typename zzz::size_cast::policy<T, U>::type, zzz::size_cast::policy_enum::same>::value
            || std::is_same<typename zzz::size_cast::policy<T, U>::type, zzz::size_cast::policy_enum::bit_to_byte>::value
            , "U should be not less and even than T"
        );
        typename zzz::range_redused<T, U>::reference r = back<U>();
        block_.dec_last(typename plain_traits<U>::size_type(1));
        return r;
    }
    template <typename T>
    typename range<T>::self_type range<T>::pop_back(size_type count)
    {
        self_type tmp(back(count));
        block_.set_last(tmp.block_.first());
        return tmp;
    }
    template <typename T>
    template <typename U>
    typename zzz::range_redused<T, U>::type range<T>::pop_back(typename zzz::range_redused<T, U>::size_type count)
    {
        typename zzz::range_redused<T, U>::type u_range = back<U>(count);
        pop_back(count);
        return u_range;
    }
    template <typename T>
    typename range<T>::self_type range<T>::pop_back_max(size_type count)
    {
        return pop_back(size().min(count));
    }
    template <typename T>
    template <typename U>
    typename zzz::range_redused<T, U>::type range<T>::pop_back_max(typename zzz::range_redused<T, U>::size_type count)
    {
        typename zzz::range_redused<T, U>::type u_range = back_max<U>(count);
        pop_back(u_range.size());
        return u_range;
    }
    template <typename T>
    template <typename U>
    typename zzz::range_redused<T, U>::type range<T>::pop_back_max()
    {
        typename zzz::range_redused<T, U>::type u_range = back_max<U>();
        pop_back(u_range.size());
        return u_range;
    }

    template <typename T>
    typename range<T>::self_type range<T>::pop_from_to_front(size_type count)
    {
        self_type tmp(from_to_front(count));
        block_.set_first(tmp.block_.last());
        return tmp;
    }
    template <typename T>
    typename range<T>::self_type range<T>::pop_from_to_back(size_type count)
    {
        self_type tmp(from_to_back(count));
        block_.set_last(tmp.block_.first());
        return tmp;
    }

    template <typename T>
    typename range<T>::self_type& range<T>::crop_front(size_type count)
    {
        MSW_ASSERT(size() >= count);
        block_.set_last(block_.first() + count);
        return *this;
    }
    template <typename T>
    template <typename U>
    typename zzz::range_redused<T, U>::type range<T>::crop_front(typename zzz::range_redused<T, U>::size_type count)
    {
        crop_front(size_type(count));
        return all<U>();
    }
    template <typename T>
    typename range<T>::self_type& range<T>::crop_front_max(size_type count)
    {
        return crop_front(size().min(count));
    }
    template <typename T>
    template <typename U>
    typename zzz::range_redused<T, U>::type range<T>::crop_front_max(typename zzz::range_redused<T, U>::size_type count)
    {
        /////
    }
    template <typename T>
    template <typename U>
    typename zzz::range_redused<T, U>::type range<T>::crop_front_max()
    {
        MSW_WARY_ASSERT(check());
        auto u_first = block_.first().template to_pointer<U>();
        auto u_last = u_first.upper_adjacent_until(block_.last().template to_pointer<U>());
        block_.set_last(u_last.template to_pointer<T>());
        return all<U>();
    }

    template <typename T>
    typename range<T>::self_type& range<T>::crop_back(size_type count)
    {
        MSW_ASSERT(size() >= count);
        block_.set_first(block_.last() - count);
        return *this;
    }
    template <typename T>
    template <typename U>
    typename zzz::range_redused<T, U>::type range<T>::crop_back(typename zzz::range_redused<T, U>::size_type count)
    {
        crop_back(size_type(count));
        return all<U>();
    }
    template <typename T>
    typename range<T>::self_type& range<T>::crop_back_max(size_type count)
    {
        return crop_back(std::min(size(), count));
    }
    template <typename T>
    template <typename U>
    typename zzz::range_redused<T, U>::type range<T>::crop_back_max(typename zzz::range_redused<T, U>::size_type count)
    {
        /////
    }
    template <typename T>
    template <typename U>
    typename zzz::range_redused<T, U>::type range<T>::crop_back_max()
    {
        MSW_WARY_ASSERT(check());
        auto u_last = block_.last().template to_pointer<U>();
        auto u_first = u_last.lower_adjacent_after(block_.first().template to_pointer<U>());
        block_.set_first(u_first.template to_pointer<T>());
        return all<U>();
    }

    template <typename T>
    typename range<T>::self_type& range<T>::crop_from_to_front(size_type back)
    {
        MSW_ASSERT(size() >= back);
        block_.dec_last(back);
        return *this;
    }
    template <typename T>
    typename range<T>::self_type& range<T>::crop_from_to_back(size_type first)
    {
        MSW_ASSERT(size() >= first);
        block_.inc_first(first);
        return *this;
    }
    template <typename T>
    typename range<T>::self_type& range<T>::crop_from_to(size_type first, size_type last)
    {
        MSW_ASSERT(size() >= last);
        auto old_first = block_.first();
        block_.set_first(old_first + first);
        block_.set_last(old_first + last);
        return *this;
    }
    template <typename T>
    typename range<T>::self_type& range<T>::crop_from_count(size_type first, size_type count)
    {
        MSW_ASSERT(size() >= (first + count));
        block_.inc_first(first);
        block_.set_last(block_.first() + count);
        return *this;
    }

    template <typename T>
    void range<T>::shrink_forward()
    {
        block_.set_first(block_.last());
    }
    template <typename T>
    void range<T>::shrink_backward()
    {
        block_.set_last(block_.first());
    }

    template <typename T>
    void range<T>::clear()
    {
        *this = self_type();
    }


    template <typename T>
    template <typename Block MSW_ON_DEBUG(, typename Validator)>
    range<T>::range(Block&& blk MSW_ON_DEBUG(, Validator&& validator), zzz::construct_range_tag)
        : block_(std::forward<Block>(blk))
        MSW_ON_DEBUG
        (
            , validator_(std::forward<Validator>(validator))
        )
    {}

#ifdef MSW_DEBUG
    template <typename T>
    bool range<T>::check() const
    {
        return validator_.check();
    }
#endif

}


namespace msw {
namespace zzz {

    template <typename Block MSW_ON_DEBUG(, typename Validator)>
    range<typename std::remove_reference<Block>::type::value_type> construct_range(Block&& blk MSW_ON_DEBUG(, Validator&& validator))
    {
        return range<typename std::remove_reference<Block>::type::value_type>(std::forward<Block>(blk) MSW_ON_DEBUG(, std::forward<Validator>(validator)), construct_range_tag());
    }

}}
