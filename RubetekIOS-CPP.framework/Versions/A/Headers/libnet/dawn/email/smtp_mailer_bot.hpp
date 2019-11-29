#pragma once

#include <string>
#include <functional>

#include <msw/std/regex.hpp>
#include <msw/noncopyable.hpp>
#include <msw/dimension/trigger.hpp>
#include <msw/throw_runtime_error.hpp>
#include <msw/proto/elastic_receiver.hpp>
#include <msw/lexical_cast/bytes_as_string.hpp>

#include <dawn/email/letter.hpp>
#include <dawn/code/base64/encode.hpp>
#include <dawn/email/extract_login.hpp>


namespace dawn
{
    struct smtp_mailer_bot
        : msw::noncopyable
    {
        typedef std::function<void(msw::range<msw::byte const>)> on_send;

             smtp_mailer_bot (
                                   on_send
                                 , std::string  const& address
                                 , std::string  const& password
                                 , email_letter const& letter
                                 , std::string  const& server
                             )                                   ;
        void on_receive      (msw::range<msw::byte const>)       ;
        bool letter_sent     ()                            const ;

    private:

        void send_command    (std::string const&) ;
        template <typename FnPtr>
        void receive_command (FnPtr)              ;

        void cmd_ehlo        () ;
        void cmd_auth_login  () ;
        void cmd_login       () ;
        void cmd_password    () ;
        void cmd_mail_from   () ;
        void cmd_rcpt_to     () ;
        void cmd_data        () ;
        void send_letter     () ;

        msw::trigger                                 letter_sent_      ;
        std::string                            const address_          ;
        std::string                            const login_            ;
        std::string                            const password_         ;
        std::string                            const server_           ;
        email_letter                           const letter_           ;
        on_send                                      send_             ;
        msw::elastic_receiver<msw::byte const>       elastic_receiver_ ;
    };
}


namespace dawn
{
    inline smtp_mailer_bot::smtp_mailer_bot
    (
          on_send             on_send
        , std::string  const& address
        , std::string  const& password
        , email_letter const& letter
        , std::string  const& server
    )
        : address_  ( address                                      )
        , login_    ( base64::encode(/*email_extract_login*/(address)) )
        , password_ ( base64::encode(password)                     )
        , server_   ( server                                       )
        , letter_   ( letter                                       )
        , send_     ( on_send                                      )
    {
        elastic_receiver_.receive
        (
            [this](msw::range<msw::byte const>)
            {
                cmd_ehlo();
            }
        );
    }

    inline void smtp_mailer_bot::on_receive(msw::range<msw::byte const> packet)
    {
        elastic_receiver_.external_on_receive(packet);
    }

    inline bool smtp_mailer_bot::letter_sent() const
    {
        return static_cast<bool>(letter_sent_);
    }

    inline void smtp_mailer_bot::send_command(std::string const& command)
    {
        send_(msw::make_range<msw::byte>(command + "\r\n"));
    }
    template <typename FnPtr>
    void smtp_mailer_bot::receive_command(FnPtr fn_ptr)
    {
        elastic_receiver_.receive(std::bind(fn_ptr, this));
    }

    inline void smtp_mailer_bot::cmd_ehlo()
    {
        send_command("ehlo " + server_);
        receive_command(&smtp_mailer_bot::cmd_auth_login);
    }
    inline void smtp_mailer_bot::cmd_auth_login()
    {
        send_command("auth login");
        receive_command(&smtp_mailer_bot::cmd_login);
    }
    inline void smtp_mailer_bot::cmd_login()
    {
        send_command(login_);
        receive_command(&smtp_mailer_bot::cmd_password);
    }
    inline void smtp_mailer_bot::cmd_password()
    {
        send_command(password_);
        receive_command(&smtp_mailer_bot::cmd_mail_from);
    }
    inline void smtp_mailer_bot::cmd_mail_from()
    {
        send_command("mail from: " + address_);
        receive_command(&smtp_mailer_bot::cmd_rcpt_to);
    }
    inline void smtp_mailer_bot::cmd_rcpt_to()
    {
        send_command("rcpt to: " + letter_.address);
        receive_command(&smtp_mailer_bot::cmd_data);
    }
    inline void smtp_mailer_bot::cmd_data()
    {
        send_command("data");
        receive_command(&smtp_mailer_bot::send_letter);
    }
    inline void smtp_mailer_bot::send_letter()
    {
        send_command("from: "       + (letter_.from.empty() ? address_ : letter_.from));
        send_command("to: "         + (letter_.to.empty() ? letter_.address : letter_.to));
        send_command("subject: "    + letter_.subject + "\n");
        send_command(letter_.text);
        send_command(".");
        elastic_receiver_.receive
        (
            [this](msw::range<msw::byte const> packet)
            {
                msw::smatch m;
                std::string const s = msw::bytes_as_string(packet);
                {
                    static msw::regex const e("ok", msw::regex_constants::icase);
                    if (msw::regex_search(s, m, e)) letter_sent_.set();
                }
                {
                    static msw::regex const e("goodbye", msw::regex_constants::icase);
                    if (msw::regex_search(s, m, e)) letter_sent_.set();
                }
                send_command("quit");
            }
        );
    }
}
