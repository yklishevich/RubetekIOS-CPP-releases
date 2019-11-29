#pragma once

#include <memory>

#include <msw/noncopyable.hpp>
#include <msw/plain_types.hpp>
#include <msw/asio/tcp_socket.hpp>
#include <msw/zzz/asio/tcp_acceptor.hpp>


namespace msw
{
    struct tcp_acceptor
        : noncopyable
    {
        typedef zzz::tcp_acceptor::on_accept  on_accept  ;
        typedef zzz::tcp_acceptor::io_service io_service ;
        typedef zzz::tcp_acceptor::endpoint   endpoint   ;

        tcp_acceptor  (io_service&, endpoint  , on_accept, on_error = nullptr) ;
        tcp_acceptor  (io_service&, wbyte port, on_accept, on_error = nullptr) ;
        ~tcp_acceptor ()                                                       ;
    private:
        std::shared_ptr<zzz::tcp_acceptor> acceptor_;
    };
}


namespace msw
{
    inline tcp_acceptor::tcp_acceptor(boost::asio::io_service& io_service, boost::asio::ip::tcp::endpoint endpoint, on_accept on_accept, on_error on_error)
        : acceptor_(std::make_shared<zzz::tcp_acceptor>(io_service, endpoint, on_accept, on_error))
    {
        acceptor_->start();
    }
    inline tcp_acceptor::tcp_acceptor(boost::asio::io_service& io_service, wbyte port, on_accept on_accept, on_error on_error)
        : tcp_acceptor(io_service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port), on_accept, on_error)
    {}
    inline tcp_acceptor::~tcp_acceptor()
    {
        acceptor_->cancel();
    }
}
