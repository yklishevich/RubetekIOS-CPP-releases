#pragma once

#include <memory>
#include <functional>

#include <msw/config.hpp>
#include <msw/assert.hpp>
#include <msw/noncopyable.hpp>
#include <msw/include/boost/asio.hpp>
#include <msw/zzz/asio/error_handler.hpp>
#include <msw/asio/socket_options.hpp>



namespace msw {
namespace zzz {

    struct tcp_connector
        : noncopyable
        , std::enable_shared_from_this<tcp_connector>
    {
        typedef std::function<void(boost::asio::ip::tcp::socket)> on_connected;

                                        tcp_connector (boost::asio::io_service&, boost::asio::ip::tcp::endpoint, on_connected, on_error)       ;
        void                            start         ()                                                                                       ;
        void                            cancel        ()                                                                                       ;
        boost::asio::ip::tcp::endpoint  endpoint      ()                                                                                 const ;

    private:

        boost::asio::ip::tcp::endpoint const endpoint_      ;
        boost::asio::ip::tcp::socket         socket_        ;
        on_connected                         on_connected_  ;
        error_handler                        error_handler_ ;
    };

}}


namespace msw {
namespace zzz {

    inline tcp_connector::tcp_connector(boost::asio::io_service& io_service, boost::asio::ip::tcp::endpoint endpoint, on_connected on_connected, on_error on_error)
        : endpoint_      ( endpoint     )
        , socket_        ( io_service   )
        , on_connected_  ( on_connected )
        , error_handler_ ( on_error     )
    {
        set_socket_option(socket_).cloexec();
    }
    inline void tcp_connector::start()
    {
        auto self(shared_from_this());
        try
        {
            socket_.async_connect
            (
                endpoint_
                , [this, self](boost::system::error_code ec)
                {
                    if (on_connected_)
                    {
                        if (ec) error_handler_(ec);
                        else (on_connected(on_connected_))(std::move(socket_));
                    }
                }
            );
        }
        catch (boost::system::system_error e)
        {
            socket_.get_io_service().post
            (
                [this, e]()
                {
                    error_handler_(e.code());
                }
            );
        }
    }
    inline void tcp_connector::cancel()
    {
        on_connected_ = nullptr;
        socket_.close();
    }
    inline boost::asio::ip::tcp::endpoint tcp_connector::endpoint() const
    {
        return endpoint_;
    }

}}
