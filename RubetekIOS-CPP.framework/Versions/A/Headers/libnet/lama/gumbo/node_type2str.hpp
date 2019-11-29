#pragma once

#include <string>

#include <msw/config.hpp>
#include <msw/throw_runtime_error.hpp>

#include <lama/gumbo/include/gumbo.hpp>


namespace lama  {
namespace gumbo {

    inline std::string node_type2str(GumboNodeType type)
    {
        switch (type)
        {
            case GUMBO_NODE_DOCUMENT   : return "document"   ; break;
            case GUMBO_NODE_ELEMENT    : return "element"    ; break;
            case GUMBO_NODE_TEXT       : return "text"       ; break;
            case GUMBO_NODE_CDATA      : return "cdata"      ; break;
            case GUMBO_NODE_COMMENT    : return "comment"    ; break;
            case GUMBO_NODE_WHITESPACE : return "whitespace" ; break;
            case GUMBO_NODE_TEMPLATE   : return "template"   ; break;
            default: msw::throw_runtime_error("unknown gumbo node type: ", static_cast<int>(type));
        }
#ifdef MSW_MSVC
#   ifdef MSW_DEBUG
        return "";
#   endif
#else
        return "";
#endif
    }

}}
