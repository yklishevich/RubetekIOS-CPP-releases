#pragma once

#include "query_interface.hpp"
#include <boost/asio/ssl.hpp>
#include <iostream>

namespace rubetek
{
    namespace util
    {
        struct ssl_basic_query : public i_query
        {
            ssl_basic_query(boost::asio::io_service &io_service, boost::asio::ssl::context &context,
                            const std::string &host, unsigned short port);

            virtual ~ssl_basic_query();

            virtual void do_query(on_error_fn on_error, on_response_complete_fn on_response_complete, on_need_data_fn on_need_data = nullptr) override;
            virtual void continue_query(on_need_data_fn on_need_data) override;

        private:

            boost::asio::ssl::stream<boost::asio::ip::tcp::socket>  socket_;

            typedef std::function<void()> on_connect_fn;
            typedef std::function<void (const boost::system::error_code& error, std::size_t bytes_transferred)> on_write_fn;

            on_connect_fn on_connect_;
            on_write_fn   on_write_;

            boost::asio::streambuf request;

            virtual void async_read() override;
            void write_request(const boost::system::error_code& error, std::size_t bytes_transferred);
            void connect();
        };

        /////////////////////////////////////////////////////////////////

        inline ssl_basic_query::ssl_basic_query(boost::asio::io_service &io_service, boost::asio::ssl::context &context,
                               const std::string &host, unsigned short port)
            : i_query(io_service, host, port)
            , socket_(io_service, context)
        {
        }

        inline ssl_basic_query::~ssl_basic_query()
        {
            //socket_.close();
            socket_.lowest_layer().close();
        }

        inline void ssl_basic_query::connect()
        {
            using boost::asio::ip::tcp;
            using namespace boost::asio;

            tcp::resolver::query query(host_, std::to_string(port_));

            resolver_.async_resolve(query, [this](const boost::system::error_code& error, boost::asio::ip::tcp::resolver::iterator endpoint_iterator)
            {
                if (!error)
                {
                    socket_.lowest_layer().async_connect( *endpoint_iterator, [this](boost::system::error_code const& error)
                    {
                        if (!error)
                        {
                            socket_.async_handshake(boost::asio::ssl::stream_base::client,
                                                    [this](boost::system::error_code const& error)
                                                    {
                                                        if (!error)
                                                        {
                                                            on_connect_();
                                                            is_connected_ = true;

                                                        } else
                                                        {
                                                            on_error_("async handshake: " + error.message());
                                                            is_complete_ = true;
                                                        }
                                                    });
                        } else
                        {
                            on_error_("async_connect: " + error.message());
                            is_complete_ = true;
                        }
                    });

                } else
                {
                    on_error_("async_resolve: " + error.message());
                    is_complete_ = true;
                }
            });

        }

        inline void ssl_basic_query::async_read()
        {
            boost::asio::async_read(socket_, boost::asio::buffer(buffer_), boost::asio::transfer_at_least(1),
                                    boost::bind(&ssl_basic_query::read_response, this, boost::asio::placeholders::error,
                                                boost::asio::placeholders::bytes_transferred));
        }

        inline void ssl_basic_query::continue_query(i_query::on_need_data_fn on_need_data)
        {
            std::vector<unsigned char> data;
            on_need_data_ = on_need_data;

            write_request(boost::system::error_code(), 0);
        }

        inline void ssl_basic_query::do_query(i_query::on_error_fn on_error,
                                       i_query::on_response_complete_fn on_response_complete, i_query::on_need_data_fn on_need_data)
        {
            on_response_complete_ = on_response_complete;
            on_error_ = on_error;
            on_need_data_ = on_need_data;

            on_write_ = [this](const boost::system::error_code& error, std::size_t bytes_transferred)
            {
                //body
                if (on_need_data_)
                {
                    write_request(boost::system::error_code(), 0);
                } else
                {
                    read_response(boost::system::error_code(), 0);
                }
            };

            on_connect_ = [this]()
            {
                //boost::asio::streambuf request;
                build_request(request);
                boost::asio::async_write(socket_, request, on_write_);
            };

            if (!is_connected_)
            {
                connect();
            } else
            {
                on_connect_();
            }
        }

        inline void ssl_basic_query::write_request(const boost::system::error_code &error, std::size_t bytes_transferred)
        {
            if (!error)
            {

                bytes_elapsed_ -= bytes_transferred;

                if ((bytes_transferred == 0) || (bytes_elapsed_ == 0))
                {
                    if (on_need_data_(data_))
                    {
                        bytes_elapsed_ = data_.size();

                        boost::asio::async_write(socket_, boost::asio::buffer(data_),
                                                 boost::bind(&ssl_basic_query::write_request, this, boost::asio::placeholders::error,
                                                             boost::asio::placeholders::bytes_transferred));
                    } else
                    {
                        is_headers_parsed_ = false;
                        is_complete_ = false;
                        headers_ = "";
                        response_.code = 0;
                        response_.body = "";
                        response_.headers.clear();
                        response_.protocol = "";

                        read_response(boost::system::error_code(), 0);
                    }
                } else
                {
                    data_.erase(data_.begin(), data_.begin() + bytes_transferred);
                    boost::asio::async_write(socket_, boost::asio::buffer(data_.data(), data_.size()),
                                             boost::bind(&ssl_basic_query::write_request, this, boost::asio::placeholders::error,
                                                         boost::asio::placeholders::bytes_transferred));
                }
            } else
            {
                on_error_("write_request: " + error.message());
            }
        }
    }
}
