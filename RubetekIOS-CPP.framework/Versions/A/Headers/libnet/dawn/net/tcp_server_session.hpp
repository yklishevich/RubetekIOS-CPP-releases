#pragma once

#include <msw/asio/tcp_server.hpp>
#include <msw/asio/tcp_server/session_interface.hpp>
#include <msw/lexical_cast/network/endpoint/to_string.hpp>

#include <dawn/log/logger_io.hpp>
#include <dawn/log/log_io_tags.hpp>
#include <dawn/net/tcp_acceptor.hpp>


namespace dawn {
namespace net  {

    template <typename Session>
    struct tcp_server_session
        : msw::tcp_session_interface
    {
        typedef msw::on_error on_error       ;
        typedef Session       custom_session ;

        template <typename ...Args>
        explicit              tcp_server_session (msw::tcp_session_interface_ptr, Args&&...)       ;
        ~                     tcp_server_session ()                                                ;

        custom_session&       custom             ()                                                ;
        custom_session const& custom             ()                                          const ;

    private:
        void                            send            (msw::range<msw::byte const>)       final ;
        void                            close           ()                                  final ;
        msw::tcp_session::endpoint_type local_endpoint  ()                            const final ;
        msw::tcp_session::endpoint_type remote_endpoint ()                            const final ;
        msw::tcp_session::endpoint_type remote_endpoint (boost::system::error_code&)  const final ;
        void                            set_on_received (on_received)                       final ;
        void                            set_on_sent     (on_sent)                           final ;
        void                            set_on_error    (on_error)                          final ;

        dawn::logger                      const logger_      ;
        dawn::logger_io<log_io_tags::tcp> const logger_io_   ;
        msw::tcp_session_interface_ptr    const interface_   ;
        on_received                             on_received_ ;
        on_sent                                 on_sent_     ;
        on_error                                on_error_    ;
        custom_session                          session_     ;
    };

}}


namespace dawn {
namespace net  {

    template <typename Session>
    template <typename ...Args>
    tcp_server_session<Session>::tcp_server_session(msw::tcp_session_interface_ptr interface, Args&&... args)
        : logger_      ( "tcp session", msw::network_endpoint_pair_to_string(interface->remote_endpoint(), interface->local_endpoint()) )
        , logger_io_   ( logger_                                                                                                        )
        , interface_   ( interface                                                                                                      )
        , on_received_ ( nullptr                                                                                                        )
        , on_sent_     ( nullptr                                                                                                        )
        , on_error_    ( nullptr                                                                                                        )
        , session_     ( this, std::forward<Args>(args)...                                                                              )
    {
        interface->set_on_received
        (
            [this, interface](msw::range<msw::byte> packet)
            {
                logger_io_.received(packet);
                if (on_received_) on_received_(packet);
            }
        );
        interface->set_on_sent
        (
            [this](msw::range<msw::byte> packet)
            {
                logger_io_.sent(packet);
                if (on_sent_) on_sent_(packet);
            }
        );
        interface->set_on_error
        (
            [this](boost::system::error_code ec)
            {
                logger_.debug("disconnected: ", ec.message());
                if (on_error_) on_error_(ec);
            }
        );
    }
    template <typename Session>
    tcp_server_session<Session>::~tcp_server_session()
    {
        on_received_ = nullptr ;
        on_sent_     = nullptr ;
        on_error_    = nullptr ;
    }

    template <typename Session>
    typename tcp_server_session<Session>::custom_session& tcp_server_session<Session>::custom()
    {
        return session_;
    }
    template <typename Session>
    typename tcp_server_session<Session>::custom_session const& tcp_server_session<Session>::custom() const
    {
        return session_;
    }

    template <typename Session>
    void tcp_server_session<Session>::send(msw::range<msw::byte const> packet)
    {
        logger_io_.send(packet);
        interface_->send(packet);
    }
    template <typename Session>
    void tcp_server_session<Session>::close()
    {
        logger_.info("close");
        interface_->close();
    }
    template <typename Session>
    msw::tcp_session::endpoint_type tcp_server_session<Session>::local_endpoint() const
    {
        return interface_->local_endpoint();
    }
    template <typename Session>
    msw::tcp_session::endpoint_type tcp_server_session<Session>::remote_endpoint() const
    {
        return interface_->remote_endpoint();
    }
    template <typename Session>
    msw::tcp_session::endpoint_type tcp_server_session<Session>::remote_endpoint(boost::system::error_code& ec) const
    {
        return interface_->remote_endpoint(ec);
    }
    template <typename Session>
    void tcp_server_session<Session>::set_on_received(on_received on_received)
    {
        logger_.debug("set on received");
        on_received_ = on_received;
    }
    template <typename Session>
    void tcp_server_session<Session>::set_on_sent(on_sent on_sent)
    {
        logger_.debug("set on sent");
        on_sent_ = on_sent;
    }
    template <typename Session>
    void tcp_server_session<Session>::set_on_error(on_error on_error)
    {
        logger_.debug("set on error");
        on_error_ = on_error;
    }

}}
