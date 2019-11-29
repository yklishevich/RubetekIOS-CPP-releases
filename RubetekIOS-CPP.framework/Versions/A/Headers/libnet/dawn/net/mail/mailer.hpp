#pragma once

#include <string>
#include <memory>

#include <dawn/net/mail/smtp_mailer.hpp>
#include <dawn/net/mail/smtps_mailer.hpp>


namespace dawn
{
    struct mailer
    {
        typedef smtp_mailer::on_log        on_log        ;
        typedef smtp_mailer::on_completion on_completion ;

             mailer  (
                           boost::asio::io_service&
                         , on_completion
                         , std::string              const& address
                         , std::string              const& password
                         , email_letter             const& letter
                         , std::string              const& server
                         , msw::wbyte                      port
                         , bool                            use_ssl
                     ) ;
             mailer  (
                           boost::asio::io_service&
                         , on_completion
                         , std::string              const& address
                         , std::string              const& password
                         , email_letter             const& letter
                         , msw::wbyte                      port
                         , bool                            use_ssl
                     ) ;

        void set_log (on_log) ;

    private:

        std::unique_ptr<smtp_mailer>  smtp_mailer_  ;
        std::unique_ptr<smtps_mailer> smtps_mailer_ ;
    };
}


namespace dawn
{
    inline mailer::mailer
    (
          boost::asio::io_service&        io_service
        , on_completion                   on_completion
        , std::string              const& address
        , std::string              const& password
        , email_letter             const& letter
        , std::string              const& server
        , msw::wbyte                      port
        , bool                            use_ssl
    )
    {
        if (use_ssl)
        {
            smtps_mailer_ = std::make_unique<smtps_mailer>
            (
                  io_service
                , on_completion
                , address
                , password
                , letter
                , server
                , port
            );
        }
        else
        {
            smtp_mailer_ = std::make_unique<smtp_mailer>
            (
                  io_service
                , on_completion
                , address
                , password
                , letter
                , server
                , port
            );
        }
    }
    inline mailer::mailer
    (
          boost::asio::io_service&        io_service
        , on_completion                   on_completion
        , std::string              const& address
        , std::string              const& password
        , email_letter             const& letter
        , msw::wbyte                      port
        , bool                            use_ssl
    )
        : mailer
        (
              io_service
            , on_completion
            , address
            , password
            , letter
            , default_smtp_server(address)
            , port
            , use_ssl
        )
    {}

    inline void mailer::set_log(on_log on_log)
    {
        if (smtp_mailer_)  smtp_mailer_  ->set_log(on_log);
        if (smtps_mailer_) smtps_mailer_ ->set_log(on_log);
    }
}
