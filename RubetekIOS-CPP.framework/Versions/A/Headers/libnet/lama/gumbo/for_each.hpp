#pragma once

#include <msw/assert.hpp>

#include <lama/gumbo/element.hpp>
#include <lama/gumbo/include/gumbo.hpp>
#include <lama/gumbo/impl/element_visitor.hpp>


namespace lama  {
namespace gumbo {

    template <typename Result = void>
    struct node_visitor
    {
        typedef Result result_type;
    };

    template <typename Visitor, typename ...Args>
    typename std::enable_if
    <
        std::is_same
        <
            typename Visitor::result_type
            , void
        >::value
    >::type
    node_apply_visitor(GumboNode* node, Visitor&& visit, Args&&... args)
    {
        MSW_ASSERT(node);
        switch (node->type)
        {
            case GUMBO_NODE_DOCUMENT:
            {
                document doc(&node->v.document);
                visit(doc, std::forward<Args>(args)...);
                break;
            }
            case GUMBO_NODE_ELEMENT:
            {
                element elem(&node->v.element);
                visit(elem, std::forward<Args>(args)...);
                break;
            }
            default: visit(node->type, text(&node->v.text), std::forward<Args>(args)...);
        }
    }
    template <typename Visitor, typename ...Args>
    typename std::enable_if
    <
        !std::is_same
        <
            typename Visitor::result_type
            , void
        >::value
        , typename Visitor::result_type
    >::type
    node_apply_visitor(GumboNode* node, Visitor&& visit, Args&&... args)
    {
        MSW_ASSERT(node);
        switch (node->type)
        {
            case GUMBO_NODE_DOCUMENT:
            {
                document doc(&node->v.document);
                return visit(doc, std::forward<Args>(args)...);
            }
            case GUMBO_NODE_ELEMENT:
            {
                element elem(&node->v.element);
                return visit(elem, std::forward<Args>(args)...);
            }
            default: return visit(node->type, text(&node->v.text), std::forward<Args>(args)...);
        }
    }

    template <typename Object, typename Fn>
    void for_each_child(Object object, Fn&& fn, unsigned fst, unsigned lst)
    {
        for (unsigned n = fst; n != lst; ++n)
            fn(object.children(n));
    }
    template <typename Object, typename Fn>
    void for_each_child(Object object, Fn&& fn)
    {
        for_each_child(object, std::forward<Fn>(fn), 0, object.children_count());
    }
    template <typename Object, typename Fn>
    bool conditional_for_each_child(Object object, Fn&& fn, unsigned fst, unsigned lst)
    {
        for (unsigned n = fst; n != lst; ++n)
            if (!fn(object.children(n))) return false;
        return true;
    }
    template <typename Object, typename Fn>
    bool conditional_for_each_child(Object object, Fn&& fn)
    {
        return conditional_for_each_child(object, std::forward<Fn>(fn), 0, object.children_count());
    }


    template <typename Visitor>
    void for_each_node_impl(GumboNode* node, Visitor&& visitor, int level, int max_depth);
    template <typename Visitor>
    bool conditional_for_each_node_impl(GumboNode* node, Visitor&& visitor, int level, int max_depth);

    template <typename Visitor>
    struct for_each_node_visitor
        : node_visitor<>
        , msw::noncopyable
    {
        explicit for_each_node_visitor(Visitor& visitor)
            : visitor_(visitor)
        {}
        template <typename Object>
        void operator () (Object o, int level, int max_depth) const
        {
            visitor_(o, level);
            if (level == max_depth) return;
            level++;
            for_each_child
            (
                o
                , [this, level, max_depth](GumboNode* node)
                {
                    for_each_node_impl(node, visitor_, level, max_depth);
                }
            );
        }
        void operator () (GumboNodeType node_type, text txt, int level, int) const
        {
            visitor_(node_type, txt, level);
        }
    private:
        Visitor& visitor_;
    };
    template <typename Visitor>
    struct conditional_for_each_node_visitor
        : node_visitor<bool>
        , msw::noncopyable
    {
        explicit conditional_for_each_node_visitor(Visitor& visitor)
            : visitor_(visitor)
        {}
        template <typename Object>
        bool operator () (Object o, int level, int max_depth) const
        {
            if (!visitor_(o, level)) return false;
            if (level == max_depth) return true;
            level++;
            return conditional_for_each_child
            (
                o
                , [this, level, max_depth](GumboNode* node)
                {
                    return conditional_for_each_node_impl(node, visitor_, level, max_depth);
                }
            );
        }
        bool operator () (GumboNodeType node_type, text txt, int level, int) const
        {
            return visitor_(node_type, txt, level);
        }
    private:
        Visitor& visitor_;
    };
    template <typename Visitor>
    void for_each_node_impl(GumboNode* node, Visitor&& visitor, int level, int max_depth)
    {
        MSW_ASSERT(node);
        node_apply_visitor
        (
            node
            , for_each_node_visitor<Visitor>(visitor)
            , level
            , max_depth
        );
    }
    template <typename Object, typename Visitor>
    void for_each_node_impl(Object obj, Visitor&& visitor, int level, int max_depth)
    {
        visitor(obj);
        if (max_depth == level) return;
        level++;
        for_each_child
        (
            obj
            , [&](GumboNode* node)
            {
                for_each_node_impl(node, visitor, 1, max_depth);
            }
        );
    }
    template <typename Visitor>
    bool conditional_for_each_node_impl(GumboNode* node, Visitor&& visitor, int level, int max_depth)
    {
        MSW_ASSERT(node);
        return node_apply_visitor
        (
            node
            , conditional_for_each_node_visitor<Visitor>(visitor)
            , level
            , max_depth
        );
    }

    template <typename Visitor>
    void for_each_node(GumboNode* node, Visitor&& visitor, int max_depth = -1)
    {
        for_each_node_impl(node, visitor, 0, max_depth);
    }
    template <typename Object, typename Visitor>
    void for_each_node(Object obj, Visitor&& visitor, int max_depth = -1)
    {
        for_each_node_impl(obj, visitor, 0, max_depth);
    }

    template <typename Visitor>
    bool conditional_for_each_node(GumboNode* node, Visitor&& visitor, int max_depth = -1)
    {
        MSW_ASSERT(node);
        return conditional_for_each_node_impl(node, visitor, 0, max_depth);
    }


    template <typename Object>
    struct object_visitor
    {
        typedef std::function<void(Object, int)> object_ready;

        object_visitor(object_ready object_ready)
            : object_ready_(object_ready)
        {}

        void operator () (Object o, int level) const
        {
            object_ready_(o, level);
        }
        template <typename Other>
        void operator () (Other, int) const
        {}
        void operator () (GumboNodeType, text, int) const
        {}

    private:
        object_ready object_ready_;
    };

    template <typename Object, typename Fn>
    void for_each_object(GumboNode* node, Fn&& fn, int max_depth = -1)
    {
        for_each_node(node, object_visitor<Object>(fn), max_depth);
    }
    template <typename Object, typename Fn>
    void for_each_object(Object o, Fn&& fn, int max_depth = -1)
    {
        fn(o, 0);
        if (max_depth != 0)
        {
            for_each_child
            (
                o
                , [fn, max_depth](GumboNode* node)
                {
                    for_each_node_impl(node, object_visitor<Object>(fn), 1, max_depth);
                }
            );
        }
    }
    template <typename Fn>
    void for_each_element(GumboNode* node, Fn&& fn, int max_depth = -1)
    {
        MSW_ASSERT(node);
        for_each_object<element>(node, fn, max_depth);
    }
    template <typename Fn>
    void for_each_element(element e, Fn&& fn, int max_depth = -1)
    {
        for_each_object<element>(e, fn, max_depth);
    }


    template <typename Object>
    struct conditional_object_visitor
    {
        typedef std::function<bool(Object, int)> object_ready;

        conditional_object_visitor(object_ready object_ready)
            : object_ready_(object_ready)
        {}

        bool operator () (Object o, int level) const
        {
            return object_ready_(o, level);
        }
        template <typename Other>
        bool operator () (Other, int) const
        {
            return true;
        }
        bool operator () (GumboNodeType, text, int) const
        {
            return true;
        }

    private:
        object_ready object_ready_;
    };

    template <typename Object, typename Fn>
    bool conditional_for_each_object(GumboNode* node, Fn&& fn, int max_depth = -1)
    {
        MSW_ASSERT(node);
        return conditional_for_each_node(node, conditional_object_visitor<Object>(fn), max_depth);
    }
    template <typename Object, typename Fn>
    bool conditional_for_each_object(Object o, Fn&& fn, int max_depth = -1)
    {
        if (fn(o, 0))
        {
            return max_depth == 0 ? true : conditional_for_each_child
            (
                o
                , [fn, max_depth](GumboNode* node)
                {
                    return conditional_for_each_node_impl(node, conditional_object_visitor<Object>(fn), 1, max_depth);
                }
            );
        }
        return false;
    }
    template <typename Fn>
    bool conditional_for_each_element(GumboNode* node, Fn&& fn, int max_depth = -1)
    {
        MSW_ASSERT(node);
        return conditional_for_each_object<element>(node, fn, max_depth);
    }
    template <typename Fn>
    bool conditional_for_each_element(element e, Fn&& fn, int max_depth = -1)
    {
        return conditional_for_each_object<element>(e, fn, max_depth);
    }


    struct for_each_child_text_visitor
        : node_visitor<>
    {
        typedef std::function<void(GumboNodeType, text)> text_ready;

        explicit for_each_child_text_visitor(text_ready text_ready)
            : text_ready_(text_ready)
        {}

        template <typename Other>
        void operator () (Other) const
        {}
        void operator () (GumboNodeType type, text txt) const
        {
            text_ready_(type, txt);
        }

    private:
        text_ready text_ready_;
    };

    template <typename Object, typename Fn>
    void for_each_child_text(Object object, Fn&& fn)
    {
        for_each_child
        (
            object
            , [&](GumboNode* node)
            {
                node_apply_visitor(node, for_each_child_text_visitor(fn));
            }
        );
    }

    struct conditional_for_each_child_text_visitor
        : node_visitor<bool>
    {
        typedef std::function<bool(GumboNodeType, text)> text_ready;

        explicit conditional_for_each_child_text_visitor(text_ready text_ready)
            : text_ready_(text_ready)
        {}

        template <typename Other>
        bool operator () (Other) const
        {
            return true;
        }
        bool operator () (GumboNodeType type, text txt) const
        {
            return text_ready_(type, txt);
        }

    private:
        text_ready text_ready_;
    };

    template <typename Object, typename Fn>
    bool conditional_for_each_child_text(Object object, Fn&& fn)
    {
        return conditional_for_each_child
        (
            object
            , [&](GumboNode* node)
            {
                return node_apply_visitor(node, conditional_for_each_child_text_visitor(fn));
            }
        );
    }


    struct for_each_child_text_tag_visitor
        : node_visitor<>
    {
        typedef std::function<void(char const*)> text_ready ;
        typedef std::function<void(GumboTag)>    tag_ready  ;

        explicit for_each_child_text_tag_visitor(text_ready text_ready, tag_ready tag_ready)
            : text_ready_(text_ready)
            , tag_ready_(tag_ready)
        {}

        template <typename ...Args>
        void operator () (document const&, Args&&...) const
        {}
        template <typename ...Args>
        void operator () (element const& e, Args&&...) const
        {
            //cwriteln(e.original_tag());
            tag_ready_(e.tag());
        }
        template <typename ...Args>
        void operator () (GumboNodeType, text txt, Args&&...) const
        {
            text_ready_(txt.value());
        }

    private:
        text_ready text_ready_;
        tag_ready tag_ready_;
    };

    template <typename Object, typename FnTxt, typename FnTag>
    void for_each_child_text_tag(Object object, FnTxt&& fn_txt, FnTag&& fn_tag, unsigned fst, unsigned lst)
    {
        for_each_child
        (
            object
            , [&](GumboNode* node)
            {
                node_apply_visitor(node, for_each_child_text_tag_visitor(fn_txt, fn_tag));
            }
            , fst
            , lst
        );
    }
    template <typename Object, typename FnTxt, typename FnTag>
    void for_each_child_text_tag(Object object, FnTxt&& fn_txt, FnTag&& fn_tag)
    {
        for_each_child_text_tag(object, fn_txt, fn_tag, 0, object.children_count());
    }

    template <typename Object, typename FnTxt, typename FnTag>
    void for_each_text_tag(Object object, FnTxt&& fn_txt, FnTag&& fn_tag)
    {
        for_each_child_text_tag_visitor visitor(fn_txt, fn_tag);
        for_each_node
        (
            object
            , visitor
        );
    }

}}
