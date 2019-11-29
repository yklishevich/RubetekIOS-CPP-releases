#pragma once

#include <memory>

#include <msw/int.hpp>
#include <msw/range.hpp>
#include <msw/noncopyable.hpp>
#include <msw/plain_types.hpp>
#include <msw/zzz/asio/session.hpp>


namespace msw
{
    template <typename Socket>
    struct session
        : noncopyable
    {
        typedef          Socket                      socket_type   ;
        typedef          zzz::session<socket_type>   session_base  ;
        typedef typename session_base::on_received   on_received   ;
        typedef typename session_base::on_sent       on_sent       ;
        typedef typename session_base::endpoint_type endpoint_type ;

                      session             (socket_type&&, on_received, on_sent, on_error = nullptr)       ;
                      ~session            ()                                                              ;

        void          send                (range<byte const>)                                             ;

        uint          queue_packets_count ()                                                        const ;
        size<byte>    queue_packets_size  ()                                                        const ;

        endpoint_type local_endpoint      ()                                                        const ;
        endpoint_type remote_endpoint     ()                                                        const ;
        endpoint_type remote_endpoint     (boost::system::error_code&)                              const ;

        template <typename Handler>
        void          eject_socket        (Handler)                                                       ;

    private:
        std::shared_ptr<session_base> session_;
    };
}


namespace msw
{
    template <typename Socket>
    session<Socket>::session(socket_type&& socket, on_received on_received, on_sent on_sent, on_error on_error)
        : session_(std::make_shared<session_base>(std::move(socket), on_received, on_sent, on_error))
    {
        session_->start();
    }
    template <typename Socket>
    session<Socket>::~session()
    {
        session_->cancel();
    }

    template <typename Socket>
    void session<Socket>::send(range<byte const> packet)
    {
        session_->send(packet);
    }

    template <typename Socket>
    uint session<Socket>::queue_packets_count() const
    {
        return session_->queue_packets_count();
    }
    template <typename Socket>
    size<byte> session<Socket>::queue_packets_size() const
    {
        return session_->queue_packets_size();
    }

    template <typename Socket>
    typename session<Socket>::endpoint_type session<Socket>::local_endpoint() const
    {
        return session_->local_endpoint();
    }
    template <typename Socket>
    typename session<Socket>::endpoint_type session<Socket>::remote_endpoint() const
    {
        return session_->remote_endpoint();
    }
    template <typename Socket>
    typename session<Socket>::endpoint_type session<Socket>::remote_endpoint(boost::system::error_code& ec) const
    {
        return session_->remote_endpoint(ec);
    }
    template <typename Socket>
    template <typename Handler>
    void session<Socket>::eject_socket(Handler handler)
    {
        session_->eject_socket(handler);
    }
}
