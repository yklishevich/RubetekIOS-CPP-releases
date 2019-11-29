#pragma once

#include <string>

#include <msw/noncopyable.hpp>

#include <dawn/salix/tree.hpp>
#include <dawn/salix/parse/remove_tabs_and_spaces.hpp>


namespace dawn  {
namespace salix {

    tree read_stream(std::istream&);

    struct scope_prcessor
        : msw::noncopyable
    {
        scope_prcessor(std::size_t level, tree& t)
            : level_ ( level )
            , tree_  ( t     )
        {}
        scope_prcessor(scope_prcessor&& other)
            : level_ ( other.level_ )
            , tree_  ( other.tree_  )
        {}
        std::size_t level() const
        {
            return level_;
        }
        std::size_t next_level() const
        {
            return level() + 1;
        }
        void process_line(std::string const& line)
        {
            if (line.front() == '#')
            {
                static std::string const reserved_word = "include";
                if ((line.size() >= reserved_word.size()) && (line.substr(1, reserved_word.size()) == reserved_word))
                {
                    std::string const filename = remove_tabs_and_spaces(line.substr(1 + reserved_word.size()));
                    try
                    {
                        std::ifstream file(filename);
                        if (!file.good()) msw::throw_runtime_error("can't open file");
                        tree_.child.push_back(read_stream(file));
                    }
                    catch (std::exception const& e)
                    {
                        msw::throw_runtime_error("file '", filename, "': ", e.what());
                    }
                }
            }
            else
            {
                tree_.child.emplace_back(node(0, level(), line));
            }
        }
        scope_prcessor next()
        {
            MSW_ASSERT(has_items());
            return scope_prcessor(next_level(), tree_.child.back());
        }
        bool has_items() const
        {
            return !tree_.child.empty();
        }
    private:
        std::size_t const level_ ;
        tree&             tree_  ;
    };

}}
