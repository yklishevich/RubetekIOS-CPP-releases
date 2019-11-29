#pragma once

#include <boost/optional.hpp>

#include <msw/range.hpp>
#include <msw/assert.hpp>

#include <lama/gumbo/npos.hpp>
#include <lama/gumbo/attribute.hpp>
#include <lama/gumbo/include/gumbo.hpp>
#include <lama/gumbo/impl/make_range_string.hpp>


namespace lama  {
namespace gumbo {

    struct element
    {
        explicit element(GumboElement* e)
            : e_(e)
        {}

        GumboTag tag() const
        {
            return e_->tag;
        }

        bool has_original_tag() const
        {
            return e_->original_tag.length && (e_->original_tag.data != nullptr);
        }
        msw::range<char const> original_tag() const
        {
            return impl::make_range_string(e_->original_tag);
        }

        unsigned children_count() const
        {
            return e_->children.length;
        }
        GumboNode* children(unsigned index) const
        {
            MSW_ASSERT(children_count());
            MSW_ASSERT(index < children_count());
            return static_cast<GumboNode*>(e_->children.data[index]);
        }
        GumboNode* front_child() const
        {
            MSW_ASSERT(children_count());
            return children(0);
        }
        GumboNode* back_child() const
        {
            MSW_ASSERT(children_count());
            return children(children_count() - 1);
        }
        unsigned children_position(element const& e, unsigned offset = 0) const
        {
            for (unsigned pos = offset; pos != children_count(); ++pos)
            {
                GumboNode* node = children(pos);
                if ((node->type == GUMBO_NODE_ELEMENT) && (&node->v.element == e.e_)) return pos;
            }
            return npos;
        }
        unsigned children_position(GumboNodeType node_type, unsigned offset = 0) const
        {
            for (unsigned pos = offset; pos != children_count(); ++pos)
            {
                GumboNode* node = children(pos);
                if (node->type == node_type) return pos;
            }
            return npos;
        }

        unsigned attribute_count() const
        {
            return e_->attributes.length;
        }
        attribute get_attribute(unsigned index) const
        {
            MSW_ASSERT(attribute_count());
            MSW_ASSERT(attribute_count() > index);
            return attribute(static_cast<GumboAttribute*>(e_->attributes.data[index]));
        }
        boost::optional<attribute> get_attribute(char const* name) const
        {
            GumboAttribute* a = ::gumbo_get_attribute(&e_->attributes, name);
            return a ? boost::optional<attribute>(attribute(a)) : boost::none;
        }

    private:
        GumboElement* e_;
    };

}}
