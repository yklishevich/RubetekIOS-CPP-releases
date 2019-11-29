#pragma once

#include <array>
#include <string>
#include <functional>

#include <boost/asio/ssl.hpp>


namespace msw
{
    struct ssl_extract_verifying_subject_name
    {
        typedef std::function<void(std::string)>        handler;
        typedef std::function<bool(bool, std::string)>  handler_preverified;

        explicit ssl_extract_verifying_subject_name (handler)                                                   ;
        explicit ssl_extract_verifying_subject_name (handler_preverified)                                       ;

        bool     operator ()                        (bool preverified, boost::asio::ssl::verify_context&) const ;

    private:
        handler                 handler_;
        handler_preverified     handler_preverified_;
    };
}


namespace msw
{
    inline ssl_extract_verifying_subject_name::ssl_extract_verifying_subject_name(handler_preverified handler_preverified)
        : handler_preverified_(handler_preverified)
    {}

    inline ssl_extract_verifying_subject_name::ssl_extract_verifying_subject_name(handler handler)
        : handler_(handler)
    {}

    inline bool ssl_extract_verifying_subject_name::operator () (bool preverified, boost::asio::ssl::verify_context& ctx) const
    {
        std::array<char, 256> subject_name;
        X509* cert = X509_STORE_CTX_get_current_cert(ctx.native_handle());
        X509_NAME_oneline(X509_get_subject_name(cert), subject_name.data(), static_cast<int>(subject_name.size()));
        if (handler_preverified_)
        {
            return handler_preverified_(preverified, subject_name.data());
        } else
        {
            return preverified;
        }
    }
}
