#pragma once

#include <string>
#include <ostream>

#include <msw/int.hpp>


namespace danw
{
    struct tabulated_output
    {
        explicit tabulated_output(std::ostream& ostream = std::cout, msw::uint tab_size = 2, msw::uint offset = 0)
            : ostream_  ( ostream                )
            , tab_size_ ( tab_size               )
            , tab_      ( offset * tab_size, ' ' )
        {}
        template <typename T>
        tabulated_output const& operator << (T&& x) const
        {
            ostream_ << tab_ << std::forward<T>(x) << '\n';
            return *this;
        }
        void shift()
        {
            tab_.insert(tab_.end(), tab_size_, ' ');
        }
        tabulated_output branch() const
        {
            tabulated_output tab_out(*this);
            tab_out.shift();
            return tab_out;
        }
        msw::uint tab_size() const
        {
            return tab_size_;
        }
        msw::uint offset() const
        {
            return tab_.size() / tab_size_;
        }
    private:
        std::ostream&       ostream_  ;
        msw::uint     const tab_size_ ;
        std::string         tab_      ;
    };
}
