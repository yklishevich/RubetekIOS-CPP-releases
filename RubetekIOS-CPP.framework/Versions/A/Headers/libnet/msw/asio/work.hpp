#pragma once

#include <msw/include/boost/asio.hpp>


namespace msw
{
    void io_service_work(boost::asio::io_service&);
}


namespace msw
{
    inline void io_service_work(boost::asio::io_service& io_service)
    {
        boost::asio::io_service::work work(io_service);
        io_service.run();
    }
}
