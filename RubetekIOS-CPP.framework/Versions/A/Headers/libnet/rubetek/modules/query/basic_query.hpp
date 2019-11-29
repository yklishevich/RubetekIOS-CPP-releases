#pragma once

#include "query_interface.hpp"

namespace rubetek
{
    namespace util
    {

        struct basic_query : public i_query
        {
            basic_query(boost::asio::io_service &io_service, const std::string &host, unsigned short port);

            virtual ~basic_query();

            virtual void do_query(on_error_fn on_error, on_response_complete_fn on_response_complete, on_need_data_fn on_need_data = nullptr) override;

        private:

            boost::asio::ip::tcp::socket                        socket_;

            virtual void async_read() override;
            virtual void continue_query(on_need_data_fn on_need_data) override;
        };

        ///////////////////////////////////////////////////


        inline basic_query::basic_query(boost::asio::io_service &io_service, const std::string &host, unsigned short port)
            : i_query(io_service, host, port)
            , socket_(io_service)            
        {
        }

        inline basic_query::~basic_query()
        {
        }

        inline void basic_query::do_query(on_error_fn on_error, on_response_complete_fn on_response_complete, on_need_data_fn on_need_data)
        {
            using boost::asio::ip::tcp;
            using namespace boost::asio;

            on_response_complete_ = on_response_complete;
            on_error_ = on_error;
            on_need_data_ = on_need_data;

            tcp::resolver::query query(host_, std::to_string(port_));

            resolver_.async_resolve(query, [this](const boost::system::error_code& error, boost::asio::ip::tcp::resolver::iterator endpoint_iterator)
            {
                if (!error)
                {
                    socket_.async_connect(*endpoint_iterator, [this](boost::system::error_code const& error)
                    {
                        if (!error)
                        {
                            boost::asio::streambuf request;

                            build_request(request);

                            boost::asio::write(socket_, request);

                            if (on_need_data_)
                            {
                                std::vector<unsigned char> data;
                                while(on_need_data_(data))
                                {
                                    size_t bytes_sent = boost::asio::write(socket_, boost::asio::buffer(data));
                                }
                            }

                            read_response(boost::system::error_code(), 0);
                        } else
                        {
                            on_error_("async_connect: " + error.message());
                            is_complete_ = true;
                        }
                    });

                } else
                {
                    on_error_("async resolve: " + error.message());
                    is_complete_ = true;
                }
            });
        }

        inline void basic_query::async_read()
        {
            boost::asio::async_read(socket_, boost::asio::buffer(buffer_), boost::asio::transfer_at_least(1),
                                    boost::bind(&basic_query::read_response, this, boost::asio::placeholders::error,
                                                boost::asio::placeholders::bytes_transferred));
        }

        inline void basic_query::continue_query(i_query::on_need_data_fn on_need_data)
        {

        }
    }
}
