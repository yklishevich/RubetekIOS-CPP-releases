#pragma once

#include <list>
#include <memory>
#include <functional>

#include <msw/asio/tcp_acceptor.hpp>
#include <msw/zzz/asio/tcp_server/session.hpp>
#include <msw/asio/tcp_server/create_session.hpp>
#include <msw/asio/tcp_server/default_on_accept.hpp>


namespace msw
{
    template <typename Session, typename Acceptor = tcp_acceptor>
    struct tcp_server
    {
        typedef          Session                              session_type      ;
        typedef          Acceptor                             acceptor_type     ;
        typedef          msw::create_session<session_type>    create_session    ;
        typedef          std::function<void(create_session)>  on_accept         ;
        typedef          msw::default_on_accept<session_type> default_on_accept ;
        typedef typename acceptor_type::endpoint              endpoint          ;

                                            tcp_server     (boost::asio::io_service&, wbyte port, on_accept = default_on_accept(), on_error = nullptr)       ;
                                            tcp_server     (boost::asio::io_service&, endpoint  , on_accept = default_on_accept(), on_error = nullptr)       ;
                                            ~tcp_server    ()                                                                                                ;

        template <typename Fn>  void        for_each       (Fn&&)                                                                                            ;
        template <typename Fn>  void        for_each       (Fn&&)                                                                                      const ;

                                std::size_t sessions_count ()                                                                                          const ;

                                void        disconnect     ()                                                                                                ;
    private:
        typedef zzz::tcp_server_impl::session<session_type> tcp_session_type ;
        typedef std::shared_ptr<tcp_session_type>           tcp_session_ptr  ;

        acceptor_type              acceptor_ ;
        std::list<tcp_session_ptr> sessions_ ;
    };
}


namespace msw
{
    template <typename Session, typename Acceptor>
    tcp_server<Session, Acceptor>::tcp_server(boost::asio::io_service& io_service, wbyte port, on_accept on_accept, on_error on_error)
        : acceptor_
        (
              io_service
            , port
            , [this, on_accept](tcp_socket&& socket)
            {
                create_session create(std::move(socket), sessions_);
                on_accept(std::move(create));
            }
            , on_error
        )
    {}
    template <typename Session, typename Acceptor>
    tcp_server<Session, Acceptor>::tcp_server(boost::asio::io_service& io_service, endpoint endpoint, on_accept on_accept, on_error on_error)
        : acceptor_
        (
              io_service
            , endpoint
            , [this, on_accept](tcp_socket&& socket)
            {
                create_session create(std::move(socket), sessions_);
                on_accept(std::move(create));
            }
            , on_error
        )
    {}
    template <typename Session, typename Acceptor>
    tcp_server<Session, Acceptor>::~tcp_server()
    {
        disconnect();
    }
    template <typename Session, typename Acceptor>
    template <typename Fn>
    void tcp_server<Session, Acceptor>::for_each(Fn&& fn)
    {
        for (auto i = sessions_.begin(); i != sessions_.end();)
        {
            auto j = i++; // for support close (erase) in for_each
            fn((*j)->custom());
        }
    }
    template <typename Session, typename Acceptor>
    template <typename Fn>
    void tcp_server<Session, Acceptor>::for_each(Fn&& fn) const
    {
        for (auto const& s : sessions_) fn(s->custom());
    }
    template <typename Session, typename Acceptor>
    std::size_t tcp_server<Session, Acceptor>::sessions_count() const
    {
        return sessions_.size();
    }
    template <typename Session, typename Acceptor>
    void tcp_server<Session, Acceptor>::disconnect()
    {
        while (!sessions_.empty()) sessions_.pop_front();
    }
}
