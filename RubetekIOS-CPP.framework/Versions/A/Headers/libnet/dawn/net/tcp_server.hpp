#pragma once

#include <msw/asio/tcp_server.hpp>

#include <dawn/log/logger_io.hpp>
#include <dawn/log/log_io_tags.hpp>
#include <dawn/net/tcp_acceptor.hpp>
#include <dawn/net/tcp_server_session.hpp>


namespace dawn {
namespace net  {

    template <typename Session>
    struct tcp_server
        : msw::tcp_server<dawn::net::tcp_server_session<Session>, dawn::net::tcp_acceptor>
    {
        typedef msw::tcp_server<dawn::net::tcp_server_session<Session>, dawn::net::tcp_acceptor> tcp_server_base   ;
        typedef typename tcp_server_base::on_accept                                              on_accept         ;
        typedef typename tcp_server_base::default_on_accept                                      default_on_accept ;
        typedef typename tcp_server_base::endpoint                                               endpoint          ;

                                    tcp_server (boost::asio::io_service&, msw::wbyte port, on_accept = default_on_accept())       ;
                                    tcp_server (boost::asio::io_service&, endpoint       , on_accept = default_on_accept())       ;

        template <typename Fn> void for_each   (Fn&&)                                                                             ;
        template <typename Fn> void for_each   (Fn&&)                                                                       const ;
    };

}}


namespace dawn {
namespace net  {

    template <typename Session>
    tcp_server<Session>::tcp_server(boost::asio::io_service& io_service, msw::wbyte port, on_accept on_accept)
        : tcp_server_base
        (
              io_service
            , port
            , on_accept
        )
    {}
    template <typename Session>
    tcp_server<Session>::tcp_server(boost::asio::io_service& io_service, endpoint endpoint, on_accept on_accept)
        : tcp_server_base
        (
              io_service
            , endpoint
            , on_accept
        )
    {}

    template <typename Session>
    template <typename Fn>
    void tcp_server<Session>::for_each(Fn&& fn)
    {
        tcp_server_base::for_each
        (
            [&fn](dawn::net::tcp_server_session<Session>& s)
            {
                fn(s.custom());
            }
        );
    }
    template <typename Session>
    template <typename Fn>
    void tcp_server<Session>::for_each(Fn&& fn) const
    {
        tcp_server_base::for_each
        (
            [&fn](dawn::net::tcp_server_session<Session> const& s)
            {
                fn(s.custom());
            }
        );
    }

}}
