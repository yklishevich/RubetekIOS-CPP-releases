#pragma once

#include <string>
#include <unordered_map>
#include <functional>

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/algorithm/string.hpp>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>


namespace rubetek
{
    namespace util
    {
        struct i_query
        {
            struct response_result
            {
                unsigned                                        code;
                std::string                                     protocol;
                std::string                                     body;
                std::unordered_map<std::string, std::string>    headers;
            };

            typedef std::function<void(response_result)>                        on_response_complete_fn;
            typedef std::function<void(const std::string&)>                     on_error_fn;
            typedef std::function<bool (std::vector<unsigned char> &data)>      on_need_data_fn;

            i_query(boost::asio::io_service &io_service, const std::string &host, unsigned short port);
            virtual ~i_query();

            virtual void set_query_params(const std::string &method, const std::string &protocol,
                                          const std::string &version, const std::string &url,
                                          const std::unordered_map<std::string, std::string> &headers,
                                          const std::string &body);

            virtual void do_query(on_error_fn on_error, on_response_complete_fn on_response_complete, on_need_data_fn on_need_data = nullptr) = 0;
            virtual void continue_query(on_need_data_fn on_need_data) = 0;
            virtual bool is_complete() const;

        protected:

            std::string                                         query_url_;
            std::string                                         query_method_;
            std::string                                         query_protocol_;
            std::string                                         query_version_;
            std::unordered_map<std::string, std::string>        query_headers_;
            std::string                                         query_body_;
            on_error_fn                                         on_error_;
            on_need_data_fn                                     on_need_data_;
            on_response_complete_fn                             on_response_complete_;
            bool                                                is_complete_;
            bool                                                is_connected_;

            std::string                                         host_;
            unsigned short                                      port_;
            boost::asio::io_service                             &io_service_;
            boost::asio::ip::tcp::resolver                      resolver_;
            std::array<char, 8192>                              buffer_;
            std::vector<unsigned char>                          data_;
            size_t                                              bytes_elapsed_;
            std::string                                         headers_;
            bool                                                is_headers_parsed_;
            response_result                                     response_;
            unsigned long long                                  expected_size_;

            virtual void async_read() = 0;

            virtual void read_response(const boost::system::error_code& error, size_t bytes_transferred);
            virtual void build_request(boost::asio::streambuf &request);

        private:
            // функция обработки HTTP-ответа с параметром Transfer-Encoding: chunked
            bool handle_http_chunked();
        };

        //////////////////////////////////////////////////

        inline i_query::i_query(boost::asio::io_service &io_service, const std::string &host, unsigned short port)
            : is_complete_(false)
            , is_headers_parsed_(false)
            , io_service_(io_service)
            , resolver_(io_service)
            , host_(host)
            , port_(port)
            , is_connected_(false)
            , expected_size_(0)
            , bytes_elapsed_(0)
        {
        }

        inline i_query::~i_query()
        {
        }

        inline void i_query::set_query_params(const std::string &method, const std::string &protocol,
                                              const std::string &version, const std::string &url,
                                              const std::unordered_map<std::string, std::string> &headers,
                                              const std::string &body)
        {
            query_method_ =     method;
            query_protocol_ =   protocol;
            query_version_ =    version;
            query_url_ =        url;
            query_headers_ =    headers;
            query_body_ =       body;
        }

        inline bool i_query::is_complete() const
        {
            return is_complete_;
        }

        inline void i_query::read_response(const boost::system::error_code& error, size_t bytes_transferred)
        {
            if (!error)
            {
                headers_ += std::string(buffer_.data(), bytes_transferred);

                size_t header_indx = headers_.find("\r\n\r\n");

                if (header_indx == std::string::npos)
                {
                    async_read();

                } else
                {
                    if (!is_headers_parsed_)
                    {
                        is_headers_parsed_ = true;

                        response_.body = headers_.substr(header_indx + 4, headers_.size() - header_indx - 4);

                        std::vector<std::string> lines;
                        std::string all_headers = headers_.substr(0, header_indx);
                        boost::split(lines, all_headers, boost::is_any_of("\r\n"));

                        if (lines.size() > 0)
                        {
                            std::vector<std::string> v;
                            boost::split(v, lines[0], boost::is_any_of(" "));
                            //if (v.size() == 3)
                            {
                                try
                                {
                                    response_.code = std::stoi(v[1]);
                                    response_.protocol = v[0];
                                }
                                catch (const std::exception &e)
                                {
                                }
                            }
                        }

                        for (size_t i = 1; i < lines.size(); i++)
                        {
                            size_t idx = lines[i].find(":");
                            if (idx != std::string::npos)
                            {
                                std::string key, value;
                                key = lines[i].substr(0, idx);
                                value = lines[i].substr(idx + 1, lines[i].size() - idx - 1);
                                response_.headers[key] = value;
                            }
                        }

                        // 204 - ответ сервера "нет содержимого"
                        if (response_.code == 204)
                        {
                            is_complete_ = true;
                            on_response_complete_(response_);
                            return;
                        }

                        if (response_.headers.count("Content-Length") > 0)
                        {
                            expected_size_ = std::stoull(response_.headers["Content-Length"]);
                            if (expected_size_ == response_.body.size())
                            {
                                is_complete_ = true;
                                on_response_complete_(response_);
                                return;
                            }
                        }
                        else if (response_.headers.count("Transfer-Encoding") > 0)
                        {
                            if (handle_http_chunked())
                            {
                                is_complete_ = true;
                                on_response_complete_(response_);
                                return;
                            }
                        }
                        else
                        {
                            // пустой ответ с заголовками, но без Content-Length
                            // или Transfer-Encoding (на всякий случай)
                            is_complete_ = true;
                            on_response_complete_(response_);
                            return;
                        }

                        if (response_.headers.empty())
                        {
                            //is_complete_ = true;
                            on_response_complete_(response_);
                            return;
                        }

                    }
                    else
                    {
                        response_.body += std::string(buffer_.data(), bytes_transferred);

                        if ((expected_size_ != 0) && (response_.body.size() == expected_size_))
                        {
                            is_complete_ = true;
                            on_response_complete_(response_);
                        }
                        else
                        {
                            if (handle_http_chunked())
                            {
                                is_complete_ = true;
                                on_response_complete_(response_);
                            }
                        }
                    }

                    if (!is_complete_)
                    {
                        async_read();
                    }
                }
            } else
            {
                on_error_("read_response: " + error.message());
                is_complete_ = true;
            }
        }

        inline bool i_query::handle_http_chunked()
        {
            bool RESULT = false;

            // Ответ от сервера Google бывает не содержит поле Content-Length в заголовке HTTP
            // Поэтому необходимо обрабатывать Transfer-Encoding: chunked (используются возможности HTTP 1.1)

            //          ПРИМЕР chunked


            //          HTTP/1.1 200 OK
            //          Content-Type: text/html
            //          Transfer-Encoding: chunked
            //
            //          23
            //          This is the data in the first chunk
            //          1A
            //          and this is the second one
            //          0
            //

            if (!response_.headers.count("Transfer-Encoding"))
                return RESULT;
            std::string ss = response_.headers["Transfer-Encoding"];
            if (ss.find("chunked") == std::string::npos)
                return RESULT;

            std::string str_chunked_len = "";
            std::string response_body = "";
            size_t chunked_len = 0;
            size_t pos_find; // = std::string::npos;
            size_t offsset = 0;

            //do
            while (true)
            {
                pos_find = response_.body.find("\r\n", offsset);
                if (pos_find == std::string::npos)
                    break;

                str_chunked_len = response_.body.substr(offsset, pos_find - offsset);
                offsset = pos_find + 2;

                try
                {
                    chunked_len = (size_t)std::stoull(str_chunked_len, 0, 16);
                }
                catch (std::exception const& e)
                {
                    chunked_len = 0;
                }
                if (chunked_len == 0)
                {
                    RESULT = true;
                    break;
                }

                if (offsset + chunked_len > response_.body.length())
                {
                    // неполный фрагмент (значит нужно еще ждать async_read )
                    response_body.clear();
                    break;
                }
                response_body += response_.body.substr(offsset, chunked_len);
                offsset += chunked_len + 2;
            } //while (pos_find != std::string::npos);


            if (response_body.length() > 0)
            {
                RESULT = true;
                response_.body = response_body;
            }
            return RESULT;
        }

        inline void i_query::build_request(boost::asio::streambuf& request)
        {
            std::ostream request_stream(&request);

            // Очищаем буффер, так как request стал глобальным в классе ssl_basic_query
            // метод ssl_basic_query::do_query ==> on_connect_ = [this]()...
            request_stream.clear();

            request_stream << query_method_.c_str() << " " << query_url_.c_str() << " "
                           << query_protocol_.c_str() << "/" << query_version_.c_str() << "\r\n";

            for (auto &header : query_headers_)
            {
                request_stream << header.first << ": " << header.second << "\r\n";
            }

            request_stream << "\r\n";

            if (!query_body_.empty())
            {
                request_stream << query_body_;
            }
        }
    }
}
