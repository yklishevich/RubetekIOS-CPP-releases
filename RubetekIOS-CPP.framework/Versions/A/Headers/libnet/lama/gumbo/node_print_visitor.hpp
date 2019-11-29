#pragma once

#include <string>
#include <ostream>

#include <msw/format.hpp>
#include <msw/lexical_cast/bytes_as_string.hpp>

#include <lama/gumbo/text.hpp>
#include <lama/gumbo/element.hpp>
#include <lama/gumbo/document.hpp>
#include <lama/gumbo/node_type2str.hpp>


namespace lama  {
namespace gumbo {

    struct node_print_visitor
        : node_visitor<>
    {
        explicit node_print_visitor(std::ostream& s = std::cout, int tab_width = 2)
            : s_         ( s         )
            , tab_width_ ( tab_width )
        {}

        void operator () (document const& d, int level) const
        {
            msw::format_s
            (
                  s_
                , make_offset(level)
                , "document: name: ", d.name()
                , "; public_identifier: ", d.public_identifier()
                , "; system_identifier: ", d.system_identifier()
                , msw::nl
            );
        }
        void operator () (element const& e, int level) const
        {
            std::string tag = "N:" + std::to_string(e.tag());
            if (e.has_original_tag())
            {
                tag += ": ";
                tag += msw::bytes_as_string(e.original_tag());
            }
            msw::format_s(s_, make_offset(level), "element: tag: ", tag, ";", msw::nl);
            for (unsigned i = 0; i != e.attribute_count(); ++i)
            {
                attribute attr = e.get_attribute(i);
                msw::format_s(s_, make_offset(level + 1), "attr: ", attr.name(), "; value: ", attr.value(), msw::nl);
            }
        }
        void operator () (GumboNodeType node_type, text const& txt, int level) const
        {
            msw::format_s(s_, make_offset(level), node_type2str(node_type), ": ", txt.value(), msw::nl);
        }
    private:

        node_print_visitor& operator = (node_print_visitor const&) = delete;

        std::string make_offset(int level) const
        {
            int const count = level * tab_width_;
            std::string offset;
            for (int i = 0; i != count; ++i) offset.push_back(' ');
            return std::move(offset);
        }

        std::ostream&       s_         ;
        int           const tab_width_ ;
    };

}}
