#pragma once

#include <functional>

#include <lama/gumbo/text.hpp>
#include <lama/gumbo/element.hpp>
#include <lama/gumbo/document.hpp>


namespace lama  {
namespace gumbo {
namespace impl  {

    template <typename Result = void>
    struct element_visitor
    {
        typedef std::function<Result(int, element)> element_ready;
        explicit element_visitor(element_ready element_ready, int max_depth)
            : element_ready_ (element_ready)
            , max_depth_     (max_depth)
        {}
        bool operator () (int level, document const&) const
        {
            if ((max_depth_ != -1) && (level > max_depth_)) return false;
            return true;
        }
        bool operator () (int level, element const& e) const
        {
            if ((max_depth_ != -1) && (level > max_depth_)) return false;
            element_ready_(level, e);
            return true;
        }
        bool operator () (int, GumboNodeType, text const&) const
        {
            return true;
        }
    private:
        element_ready element_ready_ ;
        int           max_depth_     ;
    };
    template<>
    inline bool element_visitor<bool>::operator () (int level, element const& e) const
    {
        if ((max_depth_ != -1) && (level > max_depth_)) return false;
        return element_ready_(level, e);
    }

}}}
