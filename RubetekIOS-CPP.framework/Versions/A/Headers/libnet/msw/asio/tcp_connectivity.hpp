#pragma once

#include <memory>
#include <functional>

#include <msw/std/memory.hpp>
#include <msw/noncopyable.hpp>
#include <msw/asio/resolver.hpp>
#include <msw/asio/tcp_connector.hpp>


namespace msw
{
    struct tcp_connectivity
        : noncopyable
    {
        typedef tcp_resolver::query       query           ;
        typedef std::function<void(bool)> on_result_check ;

        tcp_connectivity(boost::asio::io_service&, query, on_result_check);
    private:
        std::unique_ptr<tcp_connector> connector_ ;
        std::unique_ptr<tcp_resolver>  resolver_  ;
    };
}


namespace msw
{
    inline tcp_connectivity::tcp_connectivity
    (
          boost::asio::io_service& io_service
        , query                    query
        , on_result_check          on_result_check
    )
        : resolver_
        (
            msw::make_unique<tcp_resolver>
            (
                io_service
                , query
                , [this, &io_service, on_result_check](tcp_resolver::iterator i)
                {
                    resolver_.reset();
                    connector_ = msw::make_unique<msw::tcp_connector>
                    (
                        io_service
                        , i->endpoint()
                        , [this, on_result_check](tcp_socket socket)
                        {
                            connector_.reset();
                            on_result_check(true);
                        }
                        , [on_result_check](boost::system::error_code)
                        {
                            on_result_check(false);
                        }
                    );
                }
                , [on_result_check](boost::system::error_code)
                {
                    on_result_check(false);
                }
            )
        )
    {}
}
