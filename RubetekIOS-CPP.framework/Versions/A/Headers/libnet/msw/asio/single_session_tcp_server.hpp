#pragma once

#include <memory>
#include <functional>

#include <msw/std/memory.hpp>
#include <msw/asio/tcp_session.hpp>
#include <msw/asio/tcp_acceptor.hpp>
#include <msw/zzz/asio/tcp_server/session.hpp>
#include <msw/asio/tcp_server/create_session.hpp>
#include <msw/asio/tcp_server/default_on_accept.hpp>


namespace msw
{
    struct single_session_tcp_server
    {
        typedef on_error                   on_accepted     ;
        typedef tcp_session::on_received   on_received     ;
        typedef tcp_session::on_sent       on_sent         ;
        typedef on_error                   on_disconnected ;
        typedef tcp_session::endpoint_type endpoint_type   ;

                      single_session_tcp_server (
                                                      boost::asio::io_service&
                                                    , wbyte port
                                                    , on_accepted
                                                    , on_received
                                                    , on_sent         = nullptr
                                                    , on_disconnected = nullptr
                                                )                         ;
        bool          accepted                  ()                  const ;

        void          send                      (range<byte const>)       ;

        endpoint_type local_endpoint            ()                  const ;
        endpoint_type remote_endpoint           ()                  const ;
    private:
        typedef tcp_session session_type ;

        void          listen                    ()                        ;

        boost::asio::io_service&            io_service_      ;
        wbyte                         const port_            ;
        on_accepted                   const on_accepted_     ;
        on_received                   const on_received_     ;
        on_sent                       const on_sent_         ;
        on_disconnected               const on_disconnected_ ;
        std::unique_ptr<tcp_acceptor>       acceptor_        ;
        std::unique_ptr<session_type>       session_         ;
    };
}


namespace msw
{
    inline single_session_tcp_server::single_session_tcp_server
    (
          boost::asio::io_service& io_service
        , wbyte                    port
        , on_accepted              on_accepted
        , on_received              on_received
        , on_sent                  on_sent
        , on_disconnected          on_disconnected
    )
        : io_service_      ( io_service      )
        , port_            ( port            )
        , on_accepted_     ( on_accepted     )
        , on_received_     ( on_received     )
        , on_sent_         ( on_sent         )
        , on_disconnected_ ( on_disconnected )
    {
        listen();
    }
    inline bool single_session_tcp_server::accepted() const
    {
        return static_cast<bool>(session_);
    }
    inline void single_session_tcp_server::send(range<byte const> packet)
    {
        MSW_ASSERT(accepted());
        session_->send(packet);
    }

    inline single_session_tcp_server::endpoint_type single_session_tcp_server::local_endpoint() const
    {
        MSW_ASSERT(accepted());
        return session_->local_endpoint();
    }
    inline single_session_tcp_server::endpoint_type single_session_tcp_server::remote_endpoint() const
    {
        MSW_ASSERT(accepted());
        return session_->remote_endpoint();
    }

    inline void single_session_tcp_server::listen()
    {
        MSW_ASSERT(!acceptor_);
        acceptor_ = msw::make_unique<tcp_acceptor>
        (
              io_service_
            , port_
            , [this](tcp_socket&& socket)
            {
                session_ = msw::make_unique<tcp_session>
                (
                      std::move(socket)
                    , [this](range<byte> packet)
                    {
                        if (on_received_) on_received_(packet);
                    }
                    , [this](range<byte> packet)
                    {
                        if (on_sent_) on_sent_(packet);
                    }
                    , [this](boost::system::error_code ec)
                    {
                        session_.reset();
                        if (on_disconnected_) on_disconnected_(ec);
                        listen();
                    }
                );
                on_accepted_(boost::system::error_code());
                acceptor_.reset();
            }
            , on_accepted_
        );
    }
}
