#pragma once

#include <memory>

#include <msw/zzz/asio/resolver.hpp>


namespace msw
{
    template <typename Protocol>
    struct resolver
        : noncopyable
    {
        typedef typename zzz::resolver<Protocol>::iterator    iterator    ;
        typedef typename zzz::resolver<Protocol>::query       query       ;
        typedef typename zzz::resolver<Protocol>::on_resolved on_resolved ;

              resolver      (boost::asio::io_service&, query, on_resolved, on_error = nullptr)      ;
              ~resolver     ()                                                                      ;
        query resolve_query ()                                                                const ;

    private:
        std::shared_ptr<zzz::resolver<Protocol>> resolver_;
    };

    typedef resolver<boost::asio::ip::tcp> tcp_resolver ;
    typedef resolver<boost::asio::ip::udp> udp_resolver ;
}


namespace msw
{
    template <typename Protocol>
    resolver<Protocol>::resolver(boost::asio::io_service& io_service, query query, on_resolved on_resolved, on_error on_error)
        : resolver_(std::make_shared<zzz::resolver<Protocol>>(io_service, query, on_resolved, on_error))
    {
        resolver_->start();
    }
    template <typename Protocol>
    resolver<Protocol>::~resolver()
    {
        resolver_->cancel();
    }
    template <typename Protocol>
    typename resolver<Protocol>::query resolver<Protocol>::resolve_query() const
    {
        return resolver_->resolve_query();
    }
}
