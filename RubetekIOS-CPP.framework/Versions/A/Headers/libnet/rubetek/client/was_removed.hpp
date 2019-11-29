#pragma once

#include <ctime>

#include <boost/optional.hpp>

#include <rubetek/essence/unique_id/client.hpp>


namespace rubetek {
namespace client  {

    struct was_removed
    {
        unique_client_id::type client_id ;
        std::time_t            time      ;
    };

    typedef boost::optional<was_removed> option_was_removed;

}}
