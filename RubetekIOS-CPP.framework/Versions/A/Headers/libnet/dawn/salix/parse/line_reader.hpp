#pragma once

#include <string>
#include <istream>

#include <msw/noncopyable.hpp>
#include <msw/dimension/counter.hpp>


namespace dawn  {
namespace salix {

    struct line_reader
        : msw::noncopyable
    {
        explicit line_reader(std::istream& s)
            : s_(s)
        {}
        std::string get()
        {
            std::string line;
            std::getline(s_, line);
            ++count_;
            return std::move(line);
        }
        bool has_lines() const
        {
            return !s_.eof();
        }
        msw::counter<>::value_type line_count() const
        {
            return count_.value();
        }
    private:
        msw::counter<> count_ ;
        std::istream&  s_     ;
    };

}}
