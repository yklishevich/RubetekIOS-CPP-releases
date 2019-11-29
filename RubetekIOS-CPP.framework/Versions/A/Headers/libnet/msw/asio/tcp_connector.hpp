#pragma once

#include <memory>

#include <msw/asio/tcp_socket.hpp>
#include <msw/zzz/asio/tcp_connector.hpp>


namespace msw
{
    struct tcp_connector
        : noncopyable
    {
        typedef zzz::tcp_connector::on_connected on_connected;

                                       tcp_connector  (boost::asio::io_service&, boost::asio::ip::tcp::endpoint, on_connected, on_error = nullptr)       ;
                                       ~tcp_connector ()                                                                                                 ;
        boost::asio::ip::tcp::endpoint endpoint       ()                                                                                           const ;

    private:
        std::shared_ptr<zzz::tcp_connector> connector_;
    };
}


namespace msw
{
    inline tcp_connector::tcp_connector(boost::asio::io_service& io_service, boost::asio::ip::tcp::endpoint endpoint, on_connected on_connected, on_error on_error)
        : connector_(std::make_shared<zzz::tcp_connector>(io_service, endpoint, on_connected, on_error))
    {
        connector_->start();
    }
    inline tcp_connector::~tcp_connector()
    {
        connector_->cancel();
    }
    inline boost::asio::ip::tcp::endpoint tcp_connector::endpoint() const
    {
        return connector_->endpoint();
    }
}
