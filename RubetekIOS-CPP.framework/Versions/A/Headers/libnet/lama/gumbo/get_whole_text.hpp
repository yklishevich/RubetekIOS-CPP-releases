#pragma once

#include <string>

#include <msw/assert.hpp>

#include <lama/gumbo/for_each.hpp>


namespace lama  {
namespace gumbo {

    inline std::string decode_text_tag(GumboTag tag)
    {
        std::string s;
        if (tag == GUMBO_TAG_BR) s.push_back('\n');
        //else if (tag == GUMBO_TAG_P) s.push_back('\n');
        else
        {
            //cwriteln("bad tag: ", int(tag));
            //MSW_ASSERT(0);
        }
        return s;
    }

#define LAMA_GUMBO_GET_WHILE_TEXT(X)\
    template <typename Object>\
    std::string get_whole##X##text(Object object)\
    {\
        std::string txt;\
        for_each##X##text_tag\
        (\
            object\
            , [&](char const* s)\
            {\
                txt += s;\
            }\
            , [&](GumboTag tag)\
            {\
                txt.append(decode_text_tag(tag));\
            }\
        );\
        return txt;\
    }
LAMA_GUMBO_GET_WHILE_TEXT(_)
LAMA_GUMBO_GET_WHILE_TEXT(_child_)
#undef LAMA_GUMBO_GET_WHILE_TEXT

    template <typename Object>
    std::string get_whole_child_text(Object object, unsigned fst, unsigned lst)
    {
        std::string txt;
        for_each_child_text_tag
        (
            object
            , [&](char const* s)
            {
                txt += s;
            }
            , [&](GumboTag tag)
            {
                txt.append(decode_text_tag(tag));
            }
            , fst
            , lst
        );
        return txt;
    }

}}
