#pragma once

#include <msw/assert.hpp>

#include <lama/gumbo/include/gumbo.hpp>


namespace lama  {
namespace gumbo {

    struct document
    {
        explicit document(GumboDocument* d)
            : d_(d)
        {
            MSW_ASSERT(d_);
        }

        bool has_doctype() const
        {
            return d_->has_doctype;
        }

        char const* name() const
        {
            return d_->name;
        }
        char const* public_identifier() const
        {
            return d_->public_identifier;
        }
        char const* system_identifier() const
        {
            return d_->system_identifier;
        }

        GumboQuirksModeEnum doc_type_quirks() const
        {
            return d_->doc_type_quirks_mode;
        }

        unsigned children_count() const
        {
            return d_->children.length;
        }
        GumboNode* children(unsigned index) const
        {
            MSW_ASSERT(children_count());
            MSW_ASSERT(index < children_count());
            return static_cast<GumboNode*>(d_->children.data[index]);
        }

    private:
        GumboDocument* d_;
    };

}}
