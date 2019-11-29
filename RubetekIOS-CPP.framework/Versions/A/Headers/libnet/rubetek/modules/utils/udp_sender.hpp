#ifndef CC_UDP_SENDER_HPP
#define CC_UDP_SENDER_HPP

#include <boost/asio.hpp>
#include <boost/bind.hpp>

#include <vector>
#include <string>
#include <functional>

namespace rubetek
{
    namespace module
    {
        using namespace boost::asio;

        class udp_sender
        {
          public:

            struct packet_info
            {
                std::string                 ip;
                unsigned short              port;
                std::vector<unsigned char>  data;

                packet_info(const std::string &_ip, unsigned short _port, const std::vector<unsigned char> &_data);
            };

            typedef std::vector<unsigned char> t_packet;
            typedef std::function<void(const packet_info&)> on_packet_receive_fn;

            udp_sender(boost::asio::io_service &io_service, const std::string &source_address, unsigned short source_port, on_packet_receive_fn on_packet_receive);

            void send(const std::string& to_address, unsigned short to_port, const t_packet &packet);

          private:

            void do_receive();
            void read_data(const boost::system::error_code &ec, std::size_t bytes_received);

            io_service                      &io_service_;
            on_packet_receive_fn            on_packet_receive_;
            ip::udp::endpoint               local_endpoint_;
            ip::udp::socket                 socket_;
            std::array<unsigned char, 8192> receive_buffer_;
            ip::udp::endpoint               receive_endpoint_;
        };

        udp_sender::udp_sender(boost::asio::io_service &io_service, const std::string &source_address, unsigned short source_port, on_packet_receive_fn on_packet_receive)
            : io_service_(io_service)
            , on_packet_receive_(on_packet_receive)
            , local_endpoint_( ip::address::from_string(source_address), source_port)
            , socket_(io_service_, local_endpoint_.protocol())
        {
            socket_.set_option(socket_base::reuse_address(true));
            socket_.bind(local_endpoint_);

            do_receive();
        }

        void udp_sender::do_receive()
        {
            socket_.async_receive_from(boost::asio::buffer(receive_buffer_), receive_endpoint_
                , boost::bind(&udp_sender::read_data, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
        }

        void udp_sender::read_data(const boost::system::error_code &ec, std::size_t bytes_received)
        {
            if (!ec)
            {
                if (bytes_received != 0 )
                {
                    if (on_packet_receive_)
                    {
                        on_packet_receive_
                            (
                                packet_info
                                    (
                                        receive_endpoint_.address().to_string(), receive_endpoint_.port()
                                        , std::vector<unsigned char>(receive_buffer_.begin(), receive_buffer_.begin() + bytes_received)
                                    )
                            );
                    }
                }
                do_receive();
            }
        }

        void udp_sender::send(const std::string& to_address, unsigned short to_port, const t_packet &packet)
        {
            io_service_.post([this, packet, to_address, to_port]()
                             {
                                 boost::system::error_code ec;
                                 boost::asio::ip::udp::endpoint dest_endpoint(ip::address_v4::from_string(to_address), to_port);
                                 socket_.send_to(boost::asio::buffer(packet), dest_endpoint, 0, ec);
                             });
        }

        udp_sender::packet_info::packet_info(const std::string &_ip, unsigned short _port, const udp_sender::t_packet &_data)
            : ip(_ip)
            , port(_port)
            , data(_data)
        {
        }
    }
}

#endif //CC_UDP_SENDER_HPP
