#pragma once

#include <string>
#include <memory>
#include <functional>

#include <msw/assert.hpp>
#include <msw/buffer.hpp>
#include <msw/noncopyable.hpp>
#include <msw/include/boost/asio.hpp>
#include <msw/asio/socket_options.hpp>
#include <msw/zzz/asio/error_handler.hpp>


namespace msw {
namespace zzz {

    struct serial_port
        : noncopyable
        , std::enable_shared_from_this<serial_port>
    {
        typedef std::function<void(range<byte>)> on_received ;

             serial_port (boost::asio::io_service&, std::string const& port, on_received, on_error) ;

        void start       ()                                                                         ;
        void cancel      ()                                                                         ;

        void send        (range<byte const>)                                                        ;

        void close       ()                                                                         ;

        void baud_rate   (int)                                                                      ;

    private:

        void do_receive  ()                                                                         ;

        boost::asio::serial_port serial_port_     ;
        buffer<byte>             received_packet_ ;
        on_received              on_received_     ;
        error_handler            error_handler_   ;
    };

}}


namespace msw {
namespace zzz {

    inline serial_port::serial_port(boost::asio::io_service& io_service, std::string const& port, on_received on_received, on_error on_error)
        : serial_port_     ( io_service, port )
        , received_packet_ ( msw::KB * 64     )
        , on_received_     ( on_received      )
        , error_handler_   ( on_error         )
    {
        baud_rate(1000000);
        serial_port_.set_option( boost::asio::serial_port_base::character_size(8)                                                 );
        serial_port_.set_option( boost::asio::serial_port_base::flow_control  (boost::asio::serial_port_base::flow_control::none) );
        serial_port_.set_option( boost::asio::serial_port_base::parity        (boost::asio::serial_port_base::parity::none)       );
        serial_port_.set_option( boost::asio::serial_port_base::stop_bits     (boost::asio::serial_port_base::stop_bits::one)     );
    }

    inline void serial_port::start()
    {
        do_receive();
    }
    inline void serial_port::cancel()
    {
        on_received_ = nullptr;
        close();
    }

    inline void serial_port::send(range<byte const> packet)
    {
        serial_port_.write_some(boost::asio::buffer(packet.data().native(), packet.size().count()));
    }

    inline void serial_port::close()
    {
        serial_port_.close();
    }

    inline void serial_port::baud_rate(int rate)
    {
        serial_port_.set_option( boost::asio::serial_port_base::baud_rate( rate ) );
    }

    inline void serial_port::do_receive()
    {
        auto self(shared_from_this());
        serial_port_.async_read_some
        (
            boost::asio::buffer(received_packet_.data().native(), received_packet_.size().count()),
            [this, self](boost::system::error_code ec, std::size_t bytes_received)
            {
                if(on_received_)
                {
                    if (ec) error_handler_(ec);
                    else
                    {
                        (on_received(on_received_))(received_packet_.front(bytes_received));
                        if (on_received_) do_receive();
                    }
                }
            }
        );
    }

}}
