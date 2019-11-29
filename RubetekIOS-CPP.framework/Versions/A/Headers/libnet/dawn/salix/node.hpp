#pragma once

#include <string>

#include <msw/plain_types.hpp>

#include <dawn/salix/parse/parse_key_value.hpp>


namespace dawn  {
namespace salix {

    struct node
    {
        node()
        {}
        node(msw::uint number, msw::uint offset, std::string const& line)
            : number_ ( number )
            , offset_ ( offset )
            , line_   ( line   )
        {
            auto key_value = parse_key_value(line_);
            key_ = key_value.first;
            value_ = key_value.second;
        }
        node(node&& other)
            : number_ ( other.number_           )
            , offset_ ( other.offset_           )
            , line_   ( std::move(other.line_)  )
            , key_    ( std::move(other.key_)   )
            , value_  ( std::move(other.value_) )
        {}
        std::string const& key() const
        {
            return key_;
        }
        std::string const& value() const
        {
            return value_;
        }
        std::string const& line() const
        {
            return line_;
        }
        msw::uint line_number() const
        {
            return number_;
        }
        msw::uint line_offset() const
        {
            return offset_;
        }
        bool has_value() const
        {
            return !value_.empty();
        }

    private:
        msw::uint        number_ ;
        msw::uint        offset_ ;
        std::string line_   ;
        std::string key_    ;
        std::string value_  ;
    };

}}
