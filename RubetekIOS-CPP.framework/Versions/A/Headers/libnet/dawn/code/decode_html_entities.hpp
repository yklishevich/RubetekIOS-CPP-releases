#pragma once

#include <string>

#include <msw/throw_runtime_error.hpp>

#include <lama/gumbo/output.hpp>
#include <lama/gumbo/get_whole_text.hpp>
#include <lama/gumbo/node_as_element.hpp>


namespace dawn
{
    inline std::string decode_html_entities(std::string const& text)
    {
        lama::gumbo::output out(text);
        auto e = lama::gumbo::node_as_element(out.value->root);
        if (e)
        {
            if (e->children_count())
            {
                auto elem = lama::gumbo::node_as_element(e->back_child());
                if (elem) return lama::gumbo::get_whole_text(*elem);
            }
        }
        msw::throw_runtime_error("decode html entities fail");
#include <msw/throw_hide_warning.hpp>
    }
}
