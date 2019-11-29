#pragma once

#include <msw/asio/timer.hpp>
#include <msw/lexical_cast/network/endpoint/to_string.hpp>

#include <dawn/net/tcp_server_session.hpp>
#include <dawn/net/remote_endpoint_to_string.hpp>

#include <rubetek/proto/ihlp/timeouts.hpp>
#include <rubetek/proto/ihlp/transceiver.hpp>
#include <rubetek/net/ihlp_server_session_interface.hpp>


namespace rubetek
{
    template <typename Session, typename SizeType = msw::wbyte>
    struct ihlp_server_session
        : ihlp_server_session_interface
    {
        typedef SizeType                                   size_type     ;
        typedef msw::tcp_session::endpoint_type            endpoint_type ;
        typedef ihlp_server_session_interface::on_received on_received   ;


        template <typename ...Args>
                      ihlp_server_session (msw::tcp_session_interface_ptr, ihlp::timeouts, Args&&...)             ;
        ~             ihlp_server_session ()                                                                      ;

        bool          check_timeout       ()                                                                      ;
        bool          is_early            (msw::chrono::steady_clock::duration)                       const       ;

        std::string   name                ()                                                          const final ;
        endpoint_type remote_endpoint     (boost::system::error_code&)                                const final ;
        void          close               ()                                                                final ;

    private:
        void       send            (msw::range<msw::byte const>)       final ;
        void       set_on_received (on_received)                       final ;
        ip_address remote_ip       ()                            const final ;

        msw::tcp_session_interface_ptr    const interface_        ;
        on_received                             on_received_      ;
        ihlp::transceiver_base<size_type>       ihlp_transceiver_ ;
        Session                                 session_          ;
    };
}


namespace rubetek
{
    template <typename Session, typename SizeType>
    template <typename ...Args>
    ihlp_server_session<Session, SizeType>::ihlp_server_session(msw::tcp_session_interface_ptr interface, ihlp::timeouts timeouts, Args&&... args)
        : interface_   ( interface )
        , on_received_ ( nullptr   )
        , ihlp_transceiver_
        (
            [this](msw::range<msw::byte> payload)
            {
                if (on_received_) on_received_(payload);
            }
            , [interface](msw::range<msw::byte> ihlp_packet)
            {
                interface->send(ihlp_packet);
            }
            , timeouts
        )
        , session_ (this, std::forward<Args>(args)...)
    {
        interface->set_on_received(std::bind(&ihlp::transceiver_base<size_type>::process_tcp_payload, &ihlp_transceiver_, std::placeholders::_1));
    }
    template <typename Session, typename SizeType>
    ihlp_server_session<Session, SizeType>::~ihlp_server_session()
    {
        on_received_ = nullptr;
    }

    template <typename Session, typename SizeType>
    bool ihlp_server_session<Session, SizeType>::check_timeout()
    {
        return ihlp_transceiver_.check_timeout();
    }
    template <typename Session, typename SizeType>
    bool ihlp_server_session<Session, SizeType>::is_early(msw::chrono::steady_clock::duration duration) const
    {
        return ihlp_transceiver_.is_early(duration);
    }

    template <typename Session, typename SizeType>
    std::string ihlp_server_session<Session, SizeType>::name() const
    {
        return dawn::net::remote_endpoint_to_string(this);
    }
    template <typename Session, typename SizeType>
    typename ihlp_server_session<Session, SizeType>::endpoint_type ihlp_server_session<Session, SizeType>::remote_endpoint(boost::system::error_code& ec) const
    {
        return interface_->remote_endpoint(ec);
    }
    template <typename Session, typename SizeType>
    void ihlp_server_session<Session, SizeType>::close()
    {
        interface_->close();
    }

    template <typename Session, typename SizeType>
    void ihlp_server_session<Session, SizeType>::send(msw::range<msw::byte const> packet)
    {
        ihlp_transceiver_.send(packet);
    }
    template <typename Session, typename SizeType>
    void ihlp_server_session<Session, SizeType>::set_on_received(on_received on_received)
    {
        on_received_ = on_received;
    }
    template <typename Session, typename SizeType>
    typename ihlp_server_session<Session, SizeType>::ip_address ihlp_server_session<Session, SizeType>::remote_ip() const
    {
        return interface_->remote_endpoint().address();
    }
}
