#pragma once

#include <dawn/salix/tree.hpp>
#include <dawn/salix/parse/read_scope.hpp>
#include <dawn/salix/parse/line_reader.hpp>


namespace dawn  {
namespace salix {

    inline tree read_stream(std::istream& s)
    {
        line_reader reader(s);
        tree t;
        read_scope(reader, scope_prcessor(0, t));
        return std::move(t);
    }

}}
