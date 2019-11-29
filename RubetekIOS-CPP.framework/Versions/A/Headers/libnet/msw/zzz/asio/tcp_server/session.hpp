#pragma once

#include <memory>
#include <functional>

#include <msw/config.hpp>
#include <msw/assert.hpp>
#include <msw/std/memory.hpp>
#include <msw/asio/tcp_socket.hpp>
#include <msw/asio/tcp_session.hpp>
#include <msw/throw_runtime_error.hpp>
#include <msw/asio/tcp_server/session_interface.hpp>


namespace msw             {
namespace zzz             {
namespace tcp_server_impl {

    template <typename CustomSession>
    struct session
        : tcp_session_interface
        , std::enable_shared_from_this<session<CustomSession>>
    {
        typedef CustomSession              custom_session ;
        typedef tcp_session::endpoint_type endpoint_type  ;
        typedef std::function<void()>      erase          ;

        template <typename ...Args>
        explicit              session   (tcp_socket&&, Args&&...)       ;
        ~                     session   ()                              ;

        void                  set_erase (erase)                         ;

        custom_session&       custom    ()                              ;
        custom_session const& custom    ()                        const ;

    private:

        void          send            (range<byte const>)                final ;
        void          close           ()                                 final ;
        endpoint_type local_endpoint  ()                           const final ;
        endpoint_type remote_endpoint ()                           const final ;
        endpoint_type remote_endpoint (boost::system::error_code&) const final ;
        void          set_on_received (on_received)                      final ;
        void          set_on_sent     (on_sent)                          final ;
        void          set_on_error    (on_error)                         final ;

        erase                             erase_          ;
        on_received                       on_received_    ;
        on_sent                           on_sent_        ;
        on_error                          on_error_       ;
        std::unique_ptr<msw::tcp_session> tcp_session_    ;
        custom_session                    custom_session_ ;
    };

}}}


namespace msw             {
namespace zzz             {
namespace tcp_server_impl {

    template <typename CustomSession>
    template <typename ...Args>
    session<CustomSession>::session(tcp_socket&& socket, Args&&... args)
        : tcp_session_
        (
            msw::make_unique<tcp_session>
            (
                std::move(socket)
                , [this](range<byte> packet)
                {
                    if (on_received_)
                    {
                        auto self(MSW_SHARED_FROM_THIS);
                        on_received_(packet);
                    }
                }
                , [this](range<byte> packet)
                {
                    if (on_sent_)
                    {
                        auto self(MSW_SHARED_FROM_THIS);
                        on_sent_(packet);
                    }
                }
                , [this](boost::system::error_code ec)
                {
                    if (on_error_)
                    {
                        auto self(MSW_SHARED_FROM_THIS);
                        on_error_(ec);
                    }
                    if (tcp_session_) close();
                }
            )
        )
        , custom_session_
        (
              this
            , std::forward<Args>(args)...
        )
    {}
    template <typename CustomSession>
    session<CustomSession>::~session()
    {
        on_received_ = nullptr ;
        on_sent_     = nullptr ;
        on_error_    = nullptr ;
    }

    template <typename CustomSession>
    void session<CustomSession>::set_erase(erase erase)
    {
        MSW_ASSERT(!erase_);
        erase_ = erase;
        if (!tcp_session_) erase_();
    }

    template <typename CustomSession>
    typename session<CustomSession>::custom_session& session<CustomSession>::custom()
    {
        return custom_session_;
    }
    template <typename CustomSession>
    typename session<CustomSession>::custom_session const& session<CustomSession>::custom() const
    {
        return custom_session_;
    }

    template <typename CustomSession>
    void session<CustomSession>::send(range<byte const> packet)
    {
        if (!tcp_session_) msw::throw_runtime_error("tcp server session: send fail, because tcp session closed");
        tcp_session_->send(packet);
    }
    template <typename CustomSession>
    void session<CustomSession>::close()
    {
        tcp_session_.reset();
        if (erase_) erase_();
    }
    template <typename CustomSession>
    tcp_session::endpoint_type session<CustomSession>::local_endpoint() const
    {
        if (!tcp_session_) msw::throw_runtime_error("tcp server session: local endpoint inaccessible, because tcp session closed");
        return tcp_session_->local_endpoint();
    }
    template <typename CustomSession>
    tcp_session::endpoint_type session<CustomSession>::remote_endpoint() const
    {
        if (!tcp_session_) msw::throw_runtime_error("tcp server session: remote endpoint inaccessible, because tcp session closed");
        return tcp_session_->remote_endpoint();
    }
    template <typename CustomSession>
    tcp_session::endpoint_type session<CustomSession>::remote_endpoint(boost::system::error_code& ec) const
    {
        if (!tcp_session_) msw::throw_runtime_error("tcp server session: remote endpoint inaccessible, because tcp session closed");
        return tcp_session_->remote_endpoint(ec);
    }
    template <typename CustomSession>
    void session<CustomSession>::set_on_received(on_received on_received)
    {
        on_received_ = on_received;
    }
    template <typename CustomSession>
    void session<CustomSession>::set_on_sent(on_sent on_sent)
    {
        on_sent_ = on_sent;
    }
    template <typename CustomSession>
    void session<CustomSession>::set_on_error(on_error on_error)
    {
        on_error_ = on_error;
    }

}}}
