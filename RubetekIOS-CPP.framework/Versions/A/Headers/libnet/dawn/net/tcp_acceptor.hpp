#pragma once

#include <msw/noncopyable.hpp>
#include <msw/asio/tcp_acceptor.hpp>
#include <msw/lexical_cast/network/endpoint/to_string.hpp>
#include <msw/lexical_cast/network/endpoint/from_string.hpp>

#include <dawn/log/logger.hpp>


namespace dawn {
namespace net  {

    struct tcp_acceptor
        : msw::noncopyable
    {
        typedef msw::tcp_acceptor::io_service io_service ;
        typedef msw::tcp_acceptor::endpoint   endpoint   ;
        typedef msw::tcp_acceptor::on_accept  on_accept  ;
        typedef msw::on_error                 on_error   ;

        tcp_acceptor (boost::asio::io_service&, msw::wbyte port, on_accept, on_error = nullptr) ;
        tcp_acceptor (boost::asio::io_service&, endpoint       , on_accept, on_error = nullptr) ;

    private:

        logger            const logger_       ;
        msw::tcp_acceptor       tcp_acceptor_ ;
    };

}}


namespace dawn {
namespace net  {

    inline tcp_acceptor::tcp_acceptor(boost::asio::io_service& io_service, endpoint endpoint, on_accept on_accept, on_error on_error)
        : logger_ ( "tcp acceptor", msw::network_endpoint_to_string(endpoint), log::level::info)
        , tcp_acceptor_
        (
            io_service
            , endpoint
            , [this, on_accept](boost::asio::ip::tcp::socket socket)
            {
                logger_.debug("accepted new connection: ", msw::network_endpoint_to_string(socket.remote_endpoint()));
                on_accept(std::move(socket));
            }
            , [this, on_error](boost::system::error_code ec)
            {
                logger_.error(ec.message());
                if (on_error) on_error(ec);
                else throw boost::system::system_error(ec);
            }
        )
    {}
    inline tcp_acceptor::tcp_acceptor(boost::asio::io_service& io_service, msw::wbyte port, on_accept on_accept, on_error on_error)
        : tcp_acceptor (io_service, msw::tcp_endpoint_from_string("0.0.0.0", port), on_accept, on_error)
    {}

}}
