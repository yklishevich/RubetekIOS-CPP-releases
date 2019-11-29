#pragma once

#include <string>
#include <functional>

#include <msw/plain_types.hpp>

#include <rubetek/byte_range.hpp>
#include <rubetek/module/tag_type.hpp>


namespace rubetek {
namespace module  {

    typedef std::function<void(tag_type, byte_const_range)> tag_ready       ;
    typedef std::function<void(byte_const_range)>           tag_ready_short ;

    typedef std::function<void(tag_type, std::string const&)> tag_ready_s       ;
    typedef std::function<void(std::string const&)>           tag_ready_s_short ;

    typedef std::function<void(tag_type, msw::byte const*, msw::uint)> tag_ready_ex       ;
    typedef std::function<void(msw::byte const*, msw::uint)>           tag_ready_ex_short ;

}}
