#pragma once

#include <list>
#include <string>

#include <rubetek/essence/unique_id/controller.hpp>


namespace rubetek {
namespace client  {

    struct account_project_info
    {
        unique_controller_id::type id   ;
        std::string                name ;
    };
    typedef std::list<account_project_info> account_projects;

}}
