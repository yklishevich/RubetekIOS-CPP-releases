#pragma once

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/bind.hpp>

#include <rubetek/module/log/log.hpp>

#include <thread>

namespace rubetek
{
    namespace module
    {
        struct i_http_query
        {
            typedef std::function<void(unsigned)>   on_response_complete_fn;
            typedef std::function<void()>           on_error_fn;

            enum query_type
            {
                get,
                post,
                put
            };

            i_http_query();
            virtual ~i_http_query();

            virtual void set_query_params(query_type type, const std::string &url, const std::unordered_map<std::string, std::string> &headers,
                                  const std::string &body);

            virtual void do_query(on_error_fn on_error, on_response_complete_fn on_response_complete) = 0;
            virtual bool is_complete() const;

            static std::string query_type_to_string(query_type qt);

        protected:

            std::string                                         query_url_;
            query_type                                          query_type_;
            std::unordered_map<std::string, std::string>        query_headers_;
            std::string                                         query_body_;
            on_error_fn                                         on_error_;
            on_response_complete_fn                             on_response_complete_;
            bool                                                is_complete_;
        };

        inline i_http_query::i_http_query()
            : is_complete_(false)
        {
        }

        i_http_query::~i_http_query()
        {

        }

        inline void i_http_query::set_query_params(query_type type, const std::string &url, const std::unordered_map<std::string, std::string> &headers,
                              const std::string &body)
        {
            query_type_ = type;
            query_url_ = url;
            query_headers_ = headers;
            query_body_ = body;
        }

        inline bool i_http_query::is_complete() const
        {
            return is_complete_;
        }

        inline std::string i_http_query::query_type_to_string(query_type qt)
        {
            static const std::map<query_type, std::string> a = {{get, "GET"}, {post, "POST"}, {put, "PUT"}};
            return a.at(qt);
        }

        struct http_query : public i_http_query
        {
            http_query(boost::asio::io_service &io_service, const std::string &host)
                : host_(host)
                , io_service_(io_service)
                , resolver_(io_service)
                , socket_(io_service)
            {
            }

            virtual ~http_query()
            {
                log::debug("~http_query");
            }

            void do_query(on_error_fn on_error, on_response_complete_fn on_response_complete)
            {
                using boost::asio::ip::tcp;
                using namespace boost::asio;

                on_response_complete_ = on_response_complete;
                on_error_ = on_error;

                tcp::resolver::query query(host_, "http");

                resolver_.async_resolve(query, [this](const boost::system::error_code& error, boost::asio::ip::tcp::resolver::iterator endpoint_iterator)
                {
                    if (!error)
                    {
                        socket_.async_connect(*endpoint_iterator, [this](boost::system::error_code const& error)
                        {
                            if (!error)
                            {
                                boost::asio::streambuf request;

                                std::ostream request_stream(&request);

                                request_stream << query_type_to_string(query_type_) << " " << query_url_.c_str() << " HTTP/1.0\r\n";

                                for (auto &header : query_headers_)
                                {
                                    request_stream << header.first << ": " << header.second << "\r\n";
                                }

                                request_stream << "\r\n";
                                if (!query_body_.empty())
                                {
                                    request_stream << query_body_;
                                }

                                boost::asio::write(socket_, request);

                                read_response(boost::system::error_code(), 0);
                            } else
                            {
                                log::debug("connection error");
                                on_error_();
                                is_complete_ = true;
                            }
                        });

                    } else
                    {
                        log::debug("resolve error: " + error.message());
                        on_error_();
                        is_complete_ = true;
                    }
                });
            }

        private:

            std::string                     host_;
            boost::asio::io_service         &io_service_;
            boost::asio::ip::tcp::resolver  resolver_;
            boost::asio::ip::tcp::socket    socket_;            

            std::array<char, 8192>          buffer_;
            std::string                     headers_;
            unsigned                        response_code_;            

            void read_response(const boost::system::error_code& error, size_t bytes_transferred)
            {
                if (!error)
                {
                    headers_ += std::string(buffer_.data(), bytes_transferred);

                    if (headers_.find("\r\n\r\n") == std::string::npos)
                    {
                        boost::asio::async_read(socket_, boost::asio::buffer(buffer_), boost::asio::transfer_at_least(1),
                                                boost::bind(&http_query::read_response, this, boost::asio::placeholders::error,
                                                            boost::asio::placeholders::bytes_transferred));
                    } else
                    {
                        bool is_ok = false;
                        size_t idx = headers_.find("\r\n");
                        if (idx != std::string::npos)
                        {
                            std::string first_str = headers_.substr(0, idx);
                            size_t space_idx = first_str.find(" ");
                            if (space_idx != std::string::npos)
                            {
                                std::string version = first_str.substr(0, space_idx);
                                first_str.erase(0, space_idx + 1);
                                space_idx = first_str.find(" ");
                                if (space_idx != std::string::npos)
                                {
                                    std::string code = first_str.substr(0, space_idx);

                                    try
                                    {
                                        response_code_ = std::stoi(code);
                                        is_ok = true;
                                    }
                                    catch(std::exception &e)
                                    {
                                        log::debug("wrong reponse code: " + std::string(e.what()));
                                    }
                                }
                            }
                        }

                        if (is_ok)
                        {
                            if (on_response_complete_)
                            {
                                on_response_complete_(response_code_);
                            }
                        } else
                        {
                            on_error_();
                            is_complete_ = true;
                        }
                    }
                } else
                {
                    log::debug("read_response error");
                    on_error_();
                    is_complete_ = true;
                }
            }
        };

        struct https_query : public i_http_query
        {
            https_query(boost::asio::io_service &io_service, boost::asio::ssl::context &context, const std::string &host)
                : host_(host)
                , socket_(io_service, context)
                , io_service_(io_service)
                , resolver_(io_service)
            {
                context.set_default_verify_paths();
                socket_.set_verify_mode(boost::asio::ssl::verify_none);
                //socket_.set_verify_callback(boost::bind(&https_query::verify_certificate, this, _1, _2));
            }

            virtual ~https_query()
            {
                log::debug("~https_query");
            }

            virtual void do_query(on_error_fn on_error, on_response_complete_fn on_response_complete)
            {
                using boost::asio::ip::tcp;
                using namespace boost::asio;

                on_response_complete_ = on_response_complete;
                on_error_ = on_error;

                tcp::resolver::query query(host_, "443");

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
                                        log::debug("async handshake");

                                        boost::asio::streambuf request;

                                        std::ostream request_stream(&request);

                                        request_stream << query_type_to_string(query_type_) << " " << query_url_.c_str() << " HTTP/1.0\r\n";

                                        for (auto &header : query_headers_)
                                        {
                                            request_stream << header.first << ": " << header.second << "\r\n";
                                        }

                                        request_stream << "\r\n";
                                        if (!query_body_.empty())
                                        {
                                            request_stream << query_body_;
                                        }

                                        boost::asio::write(socket_, request);

                                        read_response(boost::system::error_code(), 0);
                                    } else
                                    {
                                        log::debug("handshake error: " + error.message());
                                        on_error_();
                                        is_complete_ = true;
                                    }
                                });
                            } else
                            {
                                log::debug("connection error: " + error.message());
                                on_error_();
                                is_complete_ = true;
                            }
                        });

                    } else
                    {
                        log::debug("resolve error: " + error.message());
                        on_error_();
                        is_complete_ = true;
                    }
                });
            }

        private:

            std::string                                             host_;
            boost::asio::ssl::stream<boost::asio::ip::tcp::socket>  socket_;
            boost::asio::io_service                                 &io_service_;
            boost::asio::ip::tcp::resolver                          resolver_;            

            std::array<char, 8192>                                  buffer_;
            std::string                                             headers_;
            unsigned                                                response_code_;            

            void read_response(const boost::system::error_code& error, size_t bytes_transferred)
            {
                if (!error)
                {
                    headers_ += std::string(buffer_.data(), bytes_transferred);

                    if (headers_.find("\r\n\r\n") == std::string::npos)
                    {
                        boost::asio::async_read(socket_, boost::asio::buffer(buffer_), boost::asio::transfer_at_least(1),
                                                boost::bind(&https_query::read_response, this, boost::asio::placeholders::error,
                                                            boost::asio::placeholders::bytes_transferred));
                    } else
                    {
                        bool is_ok = false;
                        size_t idx = headers_.find("\r\n");
                        if (idx != std::string::npos)
                        {
                            std::string first_str = headers_.substr(0, idx);
                            size_t space_idx = first_str.find(" ");
                            if (space_idx != std::string::npos)
                            {
                                std::string version = first_str.substr(0, space_idx);
                                first_str.erase(0, space_idx + 1);
                                space_idx = first_str.find(" ");
                                if (space_idx != std::string::npos)
                                {
                                    std::string code = first_str.substr(0, space_idx);

                                    try
                                    {
                                        response_code_ = std::stoi(code);
                                        is_ok = true;
                                    }
                                    catch(std::exception &e)
                                    {
                                        log::debug("wrong reponse code: " + std::string(e.what()));
                                    }
                                }
                            }
                        }

                        if (is_ok)
                        {
                            if (on_response_complete_)
                            {
                                on_response_complete_(response_code_);
                                is_complete_ = true;
                            }
                        } else
                        {
                            on_error_();
                            is_complete_ = true;
                        }
                    }
                } else
                {
                    log::debug("read_response error");
                    on_error_();
                    is_complete_ = true;
                }
            }
        };
    }
}

