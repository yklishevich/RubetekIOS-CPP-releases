#pragma once

#include <boost/optional.hpp>

#include <msw/config.hpp>
#include <msw/asio/timer.hpp>
#include <msw/lexical_cast/network/endpoint/to_string.hpp>

#include <dawn/net/tcp_server.hpp>

#include <rubetek/log.hpp>
#include <rubetek/proto/ihlp/timeouts.hpp>
#include <rubetek/net/ihlp_server_session.hpp>


namespace rubetek
{
#ifdef MSW_GCC
#   define RUBETEK_IHLP_SERVER_ARGS_SPEC
#else
#   define RUBETEK_IHLP_SERVER_ARGS_SPEC ...
#endif

    template <typename Session, typename SizeType = msw::wbyte, template <typename, typename> class IhlpSession = ihlp_server_session>
    struct ihlp_server
    {
        typedef Session                             session           ;
        typedef SizeType                            size_type         ;
        typedef IhlpSession<session, size_type>     ihlp_session      ;
        typedef msw::chrono::steady_clock::duration duration          ;
        typedef boost::optional<duration>           optional_duration ;

        template <typename EndpointPort, typename RUBETEK_IHLP_SERVER_ARGS_SPEC Args>
        ihlp_server
        (
             boost::asio::io_service&
           , EndpointPort
           , ihlp::timeouts
           , optional_duration close_duplicate_ip
           , Args&& RUBETEK_IHLP_SERVER_ARGS_SPEC
        ) ;
    private:
        void close_early (msw::chrono::steady_clock::duration, boost::asio::ip::address) ;

        typedef dawn::net::tcp_server<ihlp_session> tcp_server;

        dawn::logger      const logger_             ;
        optional_duration const close_duplicate_ip_ ;
        tcp_server              tcp_server_         ;
        msw::timer              timer_              ;
    };
}


namespace rubetek
{
    template <typename Session, typename SizeType, template <typename, typename> class IhlpSession>
    template <typename EndpointPort, typename RUBETEK_IHLP_SERVER_ARGS_SPEC Args>
    ihlp_server<Session, SizeType, IhlpSession>::ihlp_server
    (
          boost::asio::io_service& io_service
        , EndpointPort             endpoint_port
        , ihlp::timeouts           timeouts
        , optional_duration        close_duplicate_ip
        , Args&& RUBETEK_IHLP_SERVER_ARGS_SPEC args
    )
        : logger_             ( "ihlp server", msw::network_endpoint_to_string(endpoint_port), log::level::debug )
        , close_duplicate_ip_ ( close_duplicate_ip                                                               )
        , tcp_server_
        (
            io_service
            , endpoint_port
            , [this, timeouts, args RUBETEK_IHLP_SERVER_ARGS_SPEC](typename tcp_server::create_session create)
            {
                if (close_duplicate_ip_) close_early(*close_duplicate_ip_, create.remote_endpoint().address());
                create(timeouts, args RUBETEK_IHLP_SERVER_ARGS_SPEC);
            }
        )
        , timer_
        (
            io_service
            , msw::chrono::milliseconds(200)
            , [this]()
            {
                tcp_server_.for_each
                (
                    [this](ihlp_session& s)
                    {
                        if (!s.check_timeout())
                        {
                            logger_.debug("close session [", s.name(), "] by timeout");
                            s.close();
                        }
                    }
                );
            }
        )
    {}

    template <typename Session, typename SizeType, template <typename, typename> class IhlpSession>
    void ihlp_server<Session, SizeType, IhlpSession>::close_early(msw::chrono::steady_clock::duration early_duration, boost::asio::ip::address ip)
    {
        logger_.info("closing early sessions with a lifetime of more ", msw::chrono::duration_cast<msw::chrono::seconds>(early_duration).count(), " s for ip: ", ip.to_string());
        tcp_server_.for_each
        (
            [this, early_duration](ihlp_session& s)
            {
                boost::system::error_code ec;
                //typename ihlp_session::endpoint_type const endpoint = s.remote_endpoint(ec);
                if (ec) logger_.error("session missing remote endpoint");
                else
                {
                    if (s.is_early(early_duration))
                    {
                        logger_.debug("close early session: ", s.name());
                        s.close();
                    }
                }
            }
        );
    }
}
