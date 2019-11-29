#pragma once

#include <memory>
#include <functional>

#include <msw/config.hpp>
#include <msw/assert.hpp>
#include <msw/std/memory.hpp>
#include <msw/noncopyable.hpp>
#include <msw/include/boost/asio.hpp>
#include <msw/zzz/asio/error_handler.hpp>


namespace msw {
namespace zzz {

    template <typename Protocol>
    struct resolver
        : noncopyable
        , std::enable_shared_from_this<resolver<Protocol>>
    {
        typedef          boost::asio::ip::basic_resolver<Protocol> address_resolver ;
        typedef typename address_resolver::iterator                iterator         ;
        typedef typename address_resolver::query                   query            ;
        typedef          std::function<void(iterator)>             on_resolved      ;

              resolver      (boost::asio::io_service&, query, on_resolved, on_error)       ;

        void  start         ()                                                             ;
        void  cancel        ()                                                             ;
        query resolve_query ()                                                       const ;

    private:
        query            const query_         ;
        address_resolver       resolver_      ;
        on_resolved            on_resolved_   ;
        error_handler          error_handler_ ;
    };

}}


namespace msw {
namespace zzz {

    template <typename Protocol>
    resolver<Protocol>::resolver(boost::asio::io_service& io_service, query query, on_resolved on_resolved, on_error on_error)
        : query_         ( query       )
        , resolver_      ( io_service  )
        , on_resolved_   ( on_resolved )
        , error_handler_ ( on_error    )
    {}
    template <typename Protocol>
    void resolver<Protocol>::start()
    {
        auto self(MSW_SHARED_FROM_THIS);
        resolver_.async_resolve
        (
            query_
            , [this](boost::system::error_code const& ec, boost::asio::ip::tcp::resolver::iterator iterator)
            {
                if (on_resolved_)
                {
                    if (ec) error_handler_(ec);
                    else (on_resolved(on_resolved_))(iterator);
                }
            }
        );
    }
    template <typename Protocol>
    void resolver<Protocol>::cancel()
    {
        on_resolved_ = nullptr;
        resolver_.cancel();
    }
    template <typename Protocol>
    typename resolver<Protocol>::query resolver<Protocol>::resolve_query() const
    {
        return query_;
    }

}}
