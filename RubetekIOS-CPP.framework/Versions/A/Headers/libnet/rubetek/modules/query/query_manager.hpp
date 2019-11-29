#pragma once

#include <thread>

#include "basic_query.hpp"
#include "ssl_basic_query.hpp"

namespace rubetek
{
    namespace util
    {
        class query_manager
        {
        public:

            typedef std::shared_ptr<i_query> query_ptr;

            static std::list<query_ptr>::iterator push(const std::string &host, bool use_ssl);
            static std::list<query_ptr>::iterator push(const std::string &host, bool use_ssl, unsigned short port);
            static void remove(std::list<query_ptr>::iterator it);

        private:

            query_manager();
            ~query_manager();

            static query_manager& instance();
            std::list<query_ptr>::iterator push_i(const std::string &host, bool use_ssl, unsigned short port);

            boost::asio::io_service                     io_service_;
            boost::asio::ssl::context                   context_;
            std::list<query_ptr>                        queries_;
            std::thread                                 io_thrd_;

        };

        /////////////////////////////////////////////////////////////////

        inline query_manager& query_manager::instance()
        {
            static query_manager manager;
            return manager;
        }

        inline query_manager::query_manager()
            : context_(boost::asio::ssl::context::sslv23)
        {
            context_.set_default_verify_paths();
            context_.set_verify_mode(boost::asio::ssl::verify_none);
            io_thrd_ = std::thread([this]()
            {
                try
                {
                    boost::asio::io_service::work work(io_service_);
                    io_service_.run();
                } catch (...)
                //} catch (const std::exception &e)
                {
                    std::cout << "io_service exception: " << std::endl;
                    //std::cout << "io_service exception: " << e.what() << std::endl;
                }
            });
        }

        inline query_manager::~query_manager()
        {
            if (io_thrd_.joinable())
            {
                io_service_.post([this]()
                {
                    if (io_service_.stopped() == false)
                    {
                        io_service_.stop();
                    }
                });
                io_thrd_.join();
            }
        }

        inline std::list<query_manager::query_ptr>::iterator query_manager::push(const std::string &host, bool use_ssl)
        {
            unsigned port = use_ssl ? 443 : 80;
            return instance().push_i(host, use_ssl, port);
        }

        inline std::list<query_manager::query_ptr>::iterator query_manager::push(const std::string &host, bool use_ssl, unsigned short port)
        {
            return instance().push_i(host, use_ssl, port);
        }

        inline void query_manager::remove(std::list<query_ptr>::iterator it)
        {
            instance().queries_.erase(it);
        }

        inline std::list<query_manager::query_ptr>::iterator query_manager::push_i(const std::string &host, bool use_ssl, unsigned short port)
        {
            std::shared_ptr<i_query> query;

            if (use_ssl)
            {
                query.reset(new ssl_basic_query(io_service_, context_, host, port));
            } else
            {
                query.reset(new basic_query(io_service_, host, port));
            }

            queries_.push_back(query);
            auto i = --queries_.end();

            return i;
        }
    }
}
