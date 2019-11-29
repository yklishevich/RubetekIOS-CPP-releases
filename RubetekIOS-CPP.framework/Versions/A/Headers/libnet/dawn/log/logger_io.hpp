#pragma once

#include <msw/range.hpp>
#include <msw/config.hpp>
#include <msw/noncopyable.hpp>
#include <msw/include/boost/asio.hpp>
#include <msw/lexical_cast/format_size.hpp>
#include <msw/lexical_cast/network/endpoint/to_string.hpp>

#include <dawn/log/logger.hpp>
#include <dawn/log/log_io_mode.hpp>


namespace dawn
{
    template <typename T>
    struct logger_io
        : msw::noncopyable
    {
            explicit logger_io (logger const&)                                                                              ;

    static  void     set_mode  (log_io_mode)                                                                                ;

            void     send      (msw::range<msw::byte const> packet, char const* name = 0)                             const ;
            void     sent      (msw::range<msw::byte const> packet, char const* name = 0)                             const ;
            void     received  (msw::range<msw::byte const> packet, char const* name = 0)                             const ;
            void     event     (char const* description, msw::range<msw::byte const> packet, char const* name = 0)    const ;

    template <typename Protocol>
            void     send      (msw::range<msw::byte const> packet, boost::asio::ip::basic_endpoint<Protocol> const&) const ;
    template <typename Protocol>
            void     sent      (msw::range<msw::byte const> packet, boost::asio::ip::basic_endpoint<Protocol> const&) const ;
    template <typename Protocol>
            void     received  (msw::range<msw::byte const> packet, boost::asio::ip::basic_endpoint<Protocol> const&) const ;

    private:

            void     io_event  (char const* direction, char const* name, msw::range<msw::byte const> packet)          const ;

        logger      const& logger_ ;
        log_io_mode static mode_   ;
    };

    template <typename T> log_io_mode logger_io<T>::mode_ = log_io_mode::disable;
}


namespace dawn
{
    template <typename T>
    logger_io<T>::logger_io(logger const& logger)
        : logger_(logger)
    {}

    template <typename T>
    void logger_io<T>::set_mode(log_io_mode mode)
    {
        mode_ = mode;
    }

    template <typename T>
    void logger_io<T>::send(msw::range<msw::byte const> packet, char const* name) const
    {
        io_event("send", name, packet);
    }
    template <typename T>
    void logger_io<T>::sent(msw::range<msw::byte const> packet, char const* name) const
    {
        io_event("sent", name, packet);
    }
    template <typename T>
    void logger_io<T>::received(msw::range<msw::byte const> packet, char const* name) const
    {
        io_event("received", name, packet);
    }

    template <typename T>
    template <typename Protocol>
    void logger_io<T>::send(msw::range<msw::byte const> packet, boost::asio::ip::basic_endpoint<Protocol> const& endpoint) const
    {
        io_event("send", msw::network_endpoint_to_string(endpoint).c_str(), packet);
    }
    template <typename T>
    template <typename Protocol>
    void logger_io<T>::sent(msw::range<msw::byte const> packet, boost::asio::ip::basic_endpoint<Protocol> const& endpoint) const
    {
        io_event("sent", msw::network_endpoint_to_string(endpoint).c_str(), packet);
    }
    template <typename T>
    template <typename Protocol>
    void logger_io<T>::received(msw::range<msw::byte const> packet, boost::asio::ip::basic_endpoint<Protocol> const& endpoint) const
    {
        io_event("received", msw::network_endpoint_to_string(endpoint).c_str(), packet);
    }
    template <typename T>
    void logger_io<T>::event(char const* description, msw::range<msw::byte const> packet, char const* name) const
    {
        io_event(description, name, packet);
    }

    template <typename T>
    void logger_io<T>::io_event(char const* direction, char const* name, msw::range<msw::byte const> packet) const
    {
        if (mode_ == log_io_mode::disable) return;
        else if (mode_ == log_io_mode::only_size) logger_.debug(direction, (name ? " [" : " "), (name ? name : ""), (name ? "] " : ""), msw::format_size(packet.size(), "B"));
        else
        {
            msw::size<msw::byte> max_size(0xffff);
#ifdef MSW_GCC
#   pragma GCC diagnostic push
#   pragma GCC diagnostic ignored "-Wswitch"
#endif
            switch (mode_)
            {
                case log_io_mode::cut16  : max_size = 16  ; break ;
                case log_io_mode::cut32  : max_size = 32  ; break ;
                case log_io_mode::cut64  : max_size = 64  ; break ;
                case log_io_mode::cut128 : max_size = 128 ; break ;
            };
#ifdef MSW_GCC
#   pragma GCC diagnostic pop
#endif
            bool const big_packet = packet.size() > max_size;
            logger_.debug(direction, (name ? " [" : " "), (name ? name : ""), (name ? "] " : ""), msw::format_size(packet.size(), "B"), (packet.has_items() ? " - " : "")
                , (big_packet ? packet.front(max_size) : packet), (big_packet ? " ..." : ""));
        }
    }
}
