#pragma once

#include <string>
#include <sstream>
#include <functional>

#include <msw/buffer.hpp>
#include <msw/std/memory.hpp>
#include <msw/noncopyable.hpp>
#include <msw/lexical_cast/bytes_as_string.hpp>

#include <rubetek/log.hpp>
#include <rubetek/net/ssl_hybrid_client.hpp>


namespace rubetek
{
    struct http_request_performer
    {
        typedef ssl_hybrid_client::query  query        ;
        typedef std::function<void(bool)> on_completed ;

        http_request_performer
        (
              boost::asio::io_service& io_service
            , std::string              server
            , std::string              path
            , on_completed             on_completed
            , bool                     use_ssl        = true
        )
        : logger_("http request performer", this)
        , tcp_client_
        (
            msw::make_unique<ssl_hybrid_client>
            (
                io_service
                , ssl_hybrid_client::query(server, use_ssl ? "https" : "http")
                , [this, on_completed](msw::range<msw::byte const> response)
                {
                    tcp_client_.reset();
                    MSW_ASIO_SAFE_CALLBACK(on_completed, process_responce(response));
                }
                , [this, server, path]()
                {
                    //std::string const content = path;//"%D0%90%D1%80%D1%85%D0%B0%D0%BD%D0%B3%D0%B5%D0%BB%D1%8C%D1%81%D0%BA%D0%BE%D0%B5%2036%2D1%F0%9F%8C%B2%F0%9F%8C%B3";
                    //std::string s = std::string("POST ")
                    //        + "/"
                    //        // + path
                    //        //+ "/push/set_client_id/565c073daf2e/678d2798632ab5eb44aee5ed12512b684d515b784a77efe793192563a20bb44f/ru/ios/"
                    //        + " HTTP/1.0\r\n"
                    //    + "Host: " + server + "\r\n"
                    //    + "Accept: */*\r\n"
                    //    + "Content-Length: " + std::to_string(content.size()) + "\r\n"
                    //    + "Content-Type: application/x-www-form-urlencoded" + "\r\n"
                    //    + "Connection: close\r\n\r\n"
                    //    + content;
                    std::string s = "POST " + path + " HTTP/1.0\r\n" + "Host: " + server + "\r\n" + "Accept: */*\r\n" + "Connection: close\r\n\r\n";
                    //logger_.debug("send: ", s);
                    msw::buffer<msw::byte> request;
                    request.push_back(s);
                    tcp_client_->send(request.all());
                }
                , [this, on_completed]()
                {
                    tcp_client_.reset();
                    MSW_ASIO_SAFE_CALLBACK(on_completed, false);
                }
                , [this, on_completed]()
                {
                    tcp_client_.reset();
                    MSW_ASIO_SAFE_CALLBACK(on_completed, false);
                }
                , use_ssl
            )
        )
        {}

    private:

        bool process_responce(msw::range<msw::byte const> response)
        {
            try
            {
                std::stringstream response_stream(msw::bytes_as_string(response));
                //logger_.debug("recv: ", msw::bytes_as_string(response));
                std::string http_version;
                response_stream >> http_version;
                unsigned status_code;
                response_stream >> status_code;
                std::string status_message;
                std::getline(response_stream, status_message);
                if (!response_stream || http_version.substr(0, 5) != "HTTP/")
                {
                    logger_.warning("invalid response");
                    return false;
                }
                logger_.warning("response returned with status code: ", status_code);
                if (status_code != 200) return false;
            }
            catch (std::exception const& e)
            {
                logger_.error("process response fail: ", e.what());
                return false;
            }
            return true;
        }

        logger                             const logger_     ;
        std::unique_ptr<ssl_hybrid_client>       tcp_client_ ;
    };
}
