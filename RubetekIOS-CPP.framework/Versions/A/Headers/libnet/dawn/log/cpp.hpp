#pragma once

#include <dawn/log/log.hpp>


namespace dawn
{
    log::kernel log::kernel_ ;
    msw::mutex  log::mutex_  ;
}
