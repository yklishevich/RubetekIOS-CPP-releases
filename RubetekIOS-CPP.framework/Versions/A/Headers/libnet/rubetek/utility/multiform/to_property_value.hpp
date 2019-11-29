#pragma once

#include <rubetek/essence/property.hpp>
#include <rubetek/utility/noncopyable.hpp>
#include <rubetek/utility/multiform/multiform.hpp>
#include <rubetek/utility/multiform/apply_visitor.hpp>


namespace rubetek
{
    struct multiform_to_property_value_visitor
        : utility::noncopyable
    {
        static_assert(std::is_same<property::value_type, boost::variant<bool, int, float, std::string>>::value, "can't apply visitor");

        multiform_to_property_value_visitor(property::value_type& value)
            : value_(value)
        {}

        template <typename T>
        void operator () (T const&)
        {
            throw std::runtime_error("can't find type in property::value_type");
        }

        void operator () (bool v)
        {
            value_ = v;
        }
        void operator () (int v)
        {
            value_ = v;
        }
        void operator () (float v)
        {
            value_ = v;
        }
        void operator () (std::string const& v)
        {
            value_ = v;
        }

    private:
        property::value_type& value_;
    };

    property::value_type to_property_value(multiform const& mf)
    {
        property::value_type value;
        multiform_to_property_value_visitor visitor(value);
        apply_multiform_visitor(mf, visitor);
        return value;
    }
}
