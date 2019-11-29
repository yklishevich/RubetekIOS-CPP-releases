#pragma once

#include "http_query.hpp"

namespace rubetek
{
namespace module
{
    class query_manager
    {
    public:

        query_manager();
        ~query_manager();

        std::shared_ptr<i_http_query> push(const std::string &host, bool use_ssl);

    private:

        boost::asio::io_service                     io_service_;
        boost::asio::ssl::context                   context_;
        std::list<std::shared_ptr<i_http_query>>    queries_;

        std::thread                                 io_thrd_;

    };

    inline query_manager::query_manager()
        : context_(boost::asio::ssl::context::sslv23)
    {
        io_thrd_ = std::thread([this]()
        {
            try
            {
                boost::asio::io_service::work work(io_service_);
                io_service_.run();
            } catch (std::exception const & e)
            {
                log::debug("io_thread exception: " + std::string(e.what()));
            }
            log::debug("io_service.run() exit");
        });
    }

    inline query_manager::~query_manager()
    {
        if (io_thrd_.joinable())
        {
            log::debug("io_thrd_.joinable()");
            io_service_.post([this]()
            {
                if (io_service_.stopped() == false)
                {
                    io_service_.stop();
                    log::debug("io_service_.stop()");
                }
            });
            log::debug("io_thrd_.join()");
            io_thrd_.join();
        }
    }

    inline std::shared_ptr<i_http_query> query_manager::push(const std::string &host, bool use_ssl)
    {
        for (auto it = queries_.begin(); it != queries_.end();)
        {
            if ((*it)->is_complete())
            {
                queries_.erase(it++);
            } else
            {
                ++it;
            }
        }

        std::shared_ptr<i_http_query> query;

        if (use_ssl)
        {
            query.reset(new https_query(io_service_, context_, host));
        } else
        {
            query.reset(new http_query(io_service_, host));
        }

        queries_.push_back(query);
        return queries_.back();
    }
}
}
