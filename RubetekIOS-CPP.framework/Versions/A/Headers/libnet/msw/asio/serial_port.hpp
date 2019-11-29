#pragma once

#include <memory>

#include <msw/noncopyable.hpp>
#include <msw/zzz/asio/serial_port.hpp>


namespace msw
{
    struct serial_port
        : noncopyable
    {
        typedef zzz::serial_port::on_received on_received ;

             serial_port  (boost::asio::io_service&, std::string const& port, on_received, on_error = nullptr) ;
             ~serial_port ()                                                                                   ;

        void send         (range<byte const>)                                                                  ;

        void baud_rate    (int)                                                                                ;

    private:

        std::shared_ptr<zzz::serial_port> serial_port_;
    };
}


namespace msw
{
    inline serial_port::serial_port(boost::asio::io_service& io_service, std::string const& port, on_received on_received, on_error on_error)
        : serial_port_(std::make_shared<zzz::serial_port>(io_service, port, on_received, on_error))
    {
        serial_port_->start();
    }
    inline serial_port::~serial_port()
    {
        serial_port_->cancel();
    }

    inline void serial_port::send(range<byte const> packet)
    {
        serial_port_->send(packet);
    }

    inline void serial_port::baud_rate(int rate)
    {
        serial_port_->baud_rate(rate);
    }
}
