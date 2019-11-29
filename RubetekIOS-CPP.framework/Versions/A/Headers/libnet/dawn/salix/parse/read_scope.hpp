#pragma once

#include <string>

#include <boost/optional.hpp>

#include <msw/throw_runtime_error.hpp>

#include <dawn/salix/parse/line_reader.hpp>
#include <dawn/salix/parse/remove_tabs.hpp>
#include <dawn/salix/parse/scope_prcessor.hpp>


namespace dawn  {
namespace salix {

    inline void throw_invalid_placement_scope(std::size_t deep, std::size_t level)
    {
        msw::throw_runtime_error("invalid placement scope with depth ", deep, ", which should equal no more than ", level);
    }
    inline boost::optional<std::pair<std::size_t, std::string>> read_scope(line_reader& reader, scope_prcessor scope)
    {
        while (reader.has_lines())
        {
            std::string line = reader.get();
            std::size_t deep;
            line = remove_tabs(line, deep);
            if (!line.empty())
            {
                try
                {
                    if (deep == scope.level()) scope.process_line(line);
                    else if (deep > scope.level())
                    {
                        if (deep == scope.next_level())
                        {
                            if (!scope.has_items()) throw_invalid_placement_scope(deep, scope.level());
                            scope_prcessor next_scope = scope.next();
                            next_scope.process_line(line);
                            auto result = read_scope(reader, std::move(next_scope));
                            if (result)
                            {
                                MSW_ASSERT(result->first <= scope.level());
                                if (result->first == scope.level()) scope.process_line(result->second);
                                else return result;
                            }
                        }
                        else throw_invalid_placement_scope(deep, scope.next_level());
                    }
                    else return boost::optional<std::pair<std::size_t, std::string>>({deep, line});
                }
                catch (std::exception const& e)
                {
                    msw::throw_runtime_error("line ", reader.line_count(),": ", e.what());
                }
            }
        }
        return boost::none;
    }

}}
