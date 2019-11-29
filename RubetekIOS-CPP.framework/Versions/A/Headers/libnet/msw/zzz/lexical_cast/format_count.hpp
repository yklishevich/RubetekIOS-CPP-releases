#pragma once 

#include <string>
#include <sstream>

#include <msw/config.hpp>
#include <msw/assert.hpp>


namespace msw {
namespace zzz {

    template <typename ValueType, ValueType Order = 1000>
    struct format_count
    {
        typedef ValueType   value_type          ;
        typedef long double floating_point_type ;

        std::string operator () (value_type value, bool fixed_float, int precision, std::string const& measure_unit) const
        {
            MSW_ASSERT(value >= 0);
            std::string prefix;
            std::ostringstream ss;
            ss.precision(precision);
            ss << std::fixed << get_short_value(value, prefix);
            std::string s(ss.str());
            if (!fixed_float && (s.size() > 1))
            {
#ifndef MSW_NO_COMPLETE_STRING
                while (s.back() == '0') s.pop_back();
                if (s.back() == '.') s.pop_back();
#else
                int n = 0;
                std::size_t len = s.size();
                while (s[len - n - 1] == '0') ++n;
                if (s[len - n - 1] == '.') ++n;
                s = s.substr(0, len - n);
#endif
            }
            std::string const postfix = prefix + measure_unit;
            if (postfix.empty()) return s;
            return s + " " + postfix;
        }

    private:
        static const value_type order = Order;

        floating_point_type get_short_value(value_type value, std::string& prefix) const
        {
            value_type current_order = order;
            if (value < current_order)
            {
                return static_cast<unsigned>(value);
            }
#define MSW_FORMAT_COUNT_BLOCK(PREFIX)\
            if ((current_order * order == 0) || (value < (current_order * order)))\
            {\
                prefix = #PREFIX;\
                return static_cast<floating_point_type>(value) / current_order;\
            }\
            current_order *= order;
            MSW_FORMAT_COUNT_BLOCK(K)
            MSW_FORMAT_COUNT_BLOCK(M)
            MSW_FORMAT_COUNT_BLOCK(G)
            MSW_FORMAT_COUNT_BLOCK(T)
            MSW_FORMAT_COUNT_BLOCK(P)
#undef MSW_FORMAT_COUNT_BLOCK
            prefix = "P";
            if (current_order == 0) throw std::runtime_error("format count fail. current_order is null");
            return current_order ? (static_cast<floating_point_type>(value) / current_order) : 0;
        }

    };

}}
