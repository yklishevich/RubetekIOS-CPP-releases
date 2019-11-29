#pragma once

#include <memory>
#include <functional>

#include <msw/std/chrono.hpp>
#include <msw/asio/timer.hpp>
#include <msw/std/memory.hpp>
#include <msw/noncopyable.hpp>
#include <msw/dimension/trigger.hpp>
#include <msw/lexical_cast/network/endpoint/to_string.hpp>

#include <dawn/net/make_ip_list.hpp>

#include <rubetek/log.hpp>
#include <rubetek/net/udp_point.hpp>
#include <rubetek/essence/beacon_magic_number.hpp>
#include <rubetek/essence/default_port/beacon.hpp>


namespace rubetek {
namespace client  {

    struct beacon
        : msw::noncopyable
    {
        typedef std::function<void(boost::asio::ip::tcp::endpoint)> endpoint_ready;

        explicit beacon   (
                                boost::asio::io_service&
                              , endpoint_ready
                              , msw::wbyte                          broadcast_port    = default_beacon_port
                              , msw::chrono::steady_clock::duration active_interval   = msw::chrono::seconds ( 1  )
                              , msw::chrono::steady_clock::duration inactive_interval = msw::chrono::seconds ( 30 )
                          )  ;

        void     active   () ;
        void     inactive () ;

        boost::asio::io_service& get_io_service () const ;

    private:

        void start              (msw::chrono::steady_clock::duration) ;
        void send_requests_safe ()                                    ;
        void send_requests      ()                                    ;
        void send_request       (boost::asio::ip::udp::endpoint)      ;

        logger                              const logger_            ;
        msw::wbyte                          const broadcast_port_    ;
        endpoint_ready                      const endpoint_ready_    ;
        udp_point                                 udp_point_         ;
        msw::chrono::steady_clock::duration       active_interval_   ;
        msw::chrono::steady_clock::duration       inactive_interval_ ;
        std::unique_ptr<msw::timer>               timer_             ;
    };

}}


namespace rubetek {
namespace client  {

    inline beacon::beacon
    (
          boost::asio::io_service&            io_service
        , endpoint_ready                      endpoint_ready
        , msw::wbyte                          broadcast_port
        , msw::chrono::steady_clock::duration active_interval
        , msw::chrono::steady_clock::duration inactive_interval
    )
        : logger_         ( "beacon", log::level::info )
        , broadcast_port_ ( broadcast_port             )
        , endpoint_ready_ ( endpoint_ready             )
        , udp_point_
        (
            io_service
            , 0
            , [this](boost::asio::ip::udp::endpoint endpoint, msw::range<msw::byte const> packet)
            {
                if (packet.size() == msw::size<msw::wbyte>(1) + msw::size<beacon_magic_number_type>(1))
                {
                    beacon_magic_number_type const magic_number = packet.pop_front<beacon_magic_number_type>();
                    if (magic_number == default_beacon_magic_number) endpoint_ready_(boost::asio::ip::tcp::endpoint(endpoint.address(), packet.as<msw::wbyte>()));
                    else logger_.warning("bad packet content");
                }
                else logger_.warning("bad packet size");
            }
        )
        , active_interval_   ( active_interval   )
        , inactive_interval_ ( inactive_interval )
    {
#ifdef RUBETEK_CLIENT_ONLY_REMOTE
        logger_.warning("only remote mode activated");
#endif
        udp_point_.enable_broadcast();
        active();
#ifdef MSW_ANDROID
        logger_.debug("is android");
#endif
#ifdef MSW_APPLE
        logger_.debug("is ios");
#endif
    }

    inline void beacon::active()
    {
        //send_requests_safe();
        start(active_interval_);
    }
    inline void beacon::inactive()
    {
        start(inactive_interval_);
    }

    inline boost::asio::io_service& beacon::get_io_service() const
    {
        return udp_point_.get_io_service();
    }

    inline void beacon::start(msw::chrono::steady_clock::duration interval)
    {
        timer_.reset();
        timer_ = msw::make_unique<msw::timer>
        (
            udp_point_.get_io_service()
            , interval
            , [this]()
            {
                send_requests_safe();
            }
        );
    }
    inline void beacon::send_requests_safe()
    {
        try
        {
            send_requests();
        }
        catch (std::exception const& e)
        {
            logger_.error("send requests fail: ", e.what());
        }
        catch (...)
        {
            logger_.error("send requests fail");
        }
    }
    inline void beacon::send_requests()
    {
        using namespace boost::asio::ip;
        //auto ip_list = dawn::make_ip_list();
        std::list<std::pair<std::string, std::string>> ip_list;
        /*
        logger_.debug("broadcast: v4: ", boost::asio::ip::address_v4::broadcast().to_string());
        if (ip_list.empty())
        {
            logger_.debug("broadcast: empty");
        }
        else
        {
            for (auto const& item : ip_list)
                logger_.debug("broadcast: ", item.first, "/", item.second);
        }
        */
        if (ip_list.empty()) send_request(udp::endpoint(boost::asio::ip::address_v4::broadcast(), broadcast_port_));
        else
        {
            for (auto const& item : ip_list)
            {
                try
                {
                    send_request(udp::endpoint(address_v4::broadcast(address_v4::from_string(item.first), address_v4::from_string(item.second)), broadcast_port_));
                }
                catch (std::exception const& e)
                {
                    logger_.error("send request to ", item.first, "/", item.second, " fail: ", e.what());
                }
                catch (...)
                {
                    logger_.error("send request to ", item.first, "/", item.second, " fail");
                }
            }
        }
    }
    inline void beacon::send_request
    (
        boost::asio::ip::udp::endpoint
#ifndef RUBETEK_CLIENT_ONLY_REMOTE
            endpoint
#endif
    )
    {
#ifndef RUBETEK_CLIENT_ONLY_REMOTE
        udp_point_.send_to(endpoint, msw::make_range<msw::byte>(default_beacon_magic_number));
#endif
    }

}}
