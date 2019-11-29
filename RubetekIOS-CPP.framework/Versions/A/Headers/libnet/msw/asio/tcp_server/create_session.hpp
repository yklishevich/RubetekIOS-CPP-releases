#pragma once

#include <list>
#include <memory>

#include <msw/assert.hpp>
#include <msw/noncopyable.hpp>
#include <msw/asio/tcp_socket.hpp>
#include <msw/zzz/asio/tcp_server/session.hpp>


namespace msw
{
    template <typename Session>
    struct create_session
        : noncopyable
    {
        typedef zzz::tcp_server_impl::session<Session> session_type ;
        typedef std::shared_ptr<session_type>          session_ptr  ;
        typedef std::list<session_ptr>                 session_list ;
        typedef tcp_socket::endpoint_type              endpoint     ;

        create_session(tcp_socket&& socket, session_list& list)
            : socket_ ( std::move(socket) )
            , list_   ( list              )
        {}
        create_session(create_session&& other)
            : socket_ ( std::move(other.socket_) )
            , list_   ( other.list_              )
        {}
        template <typename ...Args>
        void operator () (Args&&... args)
        {
            list_.push_back(std::make_shared<session_type>(std::move(socket_), std::forward<Args>(args)...));
            auto iter = --list_.end();
            session_list& list = list_;
            (*iter)->set_erase
            (
                [&list, iter]()
                {
                    MSW_ASSERT(check_iterator(iter, list));
                    list.erase(iter);
                }
            );
        }
        endpoint local_endpoint() const
        {
            MSW_ASSERT(socket_.is_open());
            return socket_.local_endpoint();
        }
        endpoint remote_endpoint() const
        {
            MSW_ASSERT(socket_.is_open());
            return socket_.remote_endpoint();
        }
    private:
        static bool check_iterator(typename session_list::iterator i, session_list const& lst)
        {
            for (auto j = lst.begin(); j != lst.end(); ++j)
                if (j == i) return true;
            return false;
        }
        tcp_socket    socket_ ;
        session_list& list_   ;
    };
}
