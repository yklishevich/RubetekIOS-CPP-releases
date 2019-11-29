#pragma once

#include <string>
#include <functional>

#include <msw/asio/ssl_client.hpp>
#include <msw/asio/safe_callback.hpp>
#include <msw/lexical_cast/bytes_as_string.hpp>
#include <msw/lexical_cast/network/endpoint/to_string.hpp>
#include <msw/asio/ssl_extract_verifying_subject_name.hpp>

#include <dawn/email/smtp_mailer_bot.hpp>
#include <dawn/email/default_smtp_server.hpp>
#include <dawn/net/mail/log_message_type.hpp>


namespace dawn
{
    struct smtps_mailer
    {
        typedef std::function<void(mail_log_message_type, std::string)> on_log        ;
        typedef std::function<void(std::string const&)>                 on_completion ;

             smtps_mailer (
                                boost::asio::io_service&
                              , on_completion
                              , std::string  const& address
                              , std::string  const& password
                              , email_letter const& letter
                              , std::string  const& server
                              , msw::wbyte               port      = 465
                          ) ;
             smtps_mailer (
                              boost::asio::io_service&
                              , on_completion
                              , std::string  const& address
                              , std::string  const& password
                              , email_letter const& letter
                              , msw::wbyte          port      = 465
                          ) ;

        void set_log      (on_log)                                                        ;

    private:

        void send_completed (std::string const& error)                       ;
        void log_message    (mail_log_message_type type, std::string const&) ;

        on_log                           on_log_        ;
        on_completion                    on_completion_ ;
        smtp_mailer_bot                  mailer_bot_    ;
        std::unique_ptr<msw::ssl_client> ssl_client_    ;
    };
}


namespace dawn
{
    inline smtps_mailer::smtps_mailer
    (
          boost::asio::io_service& io_service
        , on_completion            on_completion
        , std::string  const&      address
        , std::string  const&      password
        , email_letter const&      letter
        , std::string  const&      server
        , msw::wbyte               port
    )
        : on_completion_(on_completion)
        , mailer_bot_
        (
            [this](msw::range<msw::byte const> packet)
            {
                MSW_ASSERT(ssl_client_);
                log_message(mail_log_message_type::to_server, bytes_as_string(packet));
                ssl_client_->send(packet);
            }
            , address
            , password
            , letter
            , server
        )
        , ssl_client_
        (
            new msw::ssl_client
            (
                io_service
                , msw::ssl_client::query(server, std::to_string(port))
                , [this](msw::tcp_session::endpoint_type endpoint)
                {
                    log_message(mail_log_message_type::connection, "connecting to: " + msw::network_endpoint_to_string(endpoint));
                }
                , [this]()
                {
                    log_message(mail_log_message_type::connection, "connecting timeout");
                }
                , [this](boost::system::error_code ec)
                {
                    if (ec) send_completed("can't connect: " + ec.message());
                    else log_message(mail_log_message_type::connection, "successfully connected");
                    // warning: after calling the send_completed should not have code, you can delete the object
                }
                , msw::ssl_extract_verifying_subject_name
                (
                    static_cast<msw::ssl_extract_verifying_subject_name::handler>
                    (
                        [this](std::string subject_name)
                        {
                            log_message(mail_log_message_type::connection, "verifying: " + subject_name);
                        }
                    )
                )
                , [this]()
                {
                    log_message(mail_log_message_type::connection, "handshake timeout");
                }
                , [this](boost::system::error_code ec)
                {
                    if (ec) send_completed("handshake fail: " + ec.message());
                    else log_message(mail_log_message_type::connection, "handshake successfully completed");
                    // warning: after calling the send_completed should not have code, you can delete the object
                }
                , [this](msw::range<msw::byte const> packet)
                {
                    log_message(mail_log_message_type::from_server, bytes_as_string(packet));
                    mailer_bot_.on_receive(packet);
                }
                , nullptr
                , [this](boost::system::error_code ec)
                {
                    if (mailer_bot_.letter_sent()) send_completed("");
                    else send_completed("disconnected: " + ec.message());
                    // warning: after calling the send_completed should not have code, you can delete the object
                }
            )
        )
    {}
    inline smtps_mailer::smtps_mailer
    (
          boost::asio::io_service& io_service
        , on_completion            on_completion
        , std::string  const&      address
        , std::string  const&      password
        , email_letter const&      letter
        , msw::wbyte               port
    )
        : smtps_mailer
        (
              io_service
            , on_completion
            , address
            , password
            , letter
            , default_smtp_server(address)
            , port
        )
    {}

    inline void smtps_mailer::set_log(on_log on_log)
    {
        on_log_ = on_log;
    }

    inline void smtps_mailer::send_completed(std::string const& error)
    {
        ssl_client_.reset();
        MSW_ASIO_SAFE_CALLBACK(on_completion_, error);
        // warning: after calling the callback should not have code, you can delete the object
    }

    inline void smtps_mailer::log_message(mail_log_message_type type, std::string const& text)
    {
        if (on_log_) MSW_ASIO_SAFE_CALLBACK(on_log_, type, text);
    }
}
