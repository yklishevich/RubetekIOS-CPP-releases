#pragma once 

#include <msw/noncopyable.hpp>
#include <msw/std/mutex.hpp>
#include <msw/std/condition_variable.hpp>


namespace msw          {
namespace cross_thread {

    struct event
        : noncopyable
    {
             event  () ;

        void notify () ;
        void wait   () ;

    private:
        bool                    v_  ;
        msw::condition_variable cv_ ;
        msw::mutex              m_  ;
    };

}}



namespace msw          {
namespace cross_thread {

    inline event::event()
        : v_(false)
    {}

    inline void event::notify()
    {
        msw::unique_lock<msw::mutex> lock(m_);
        v_ = true;
        cv_.notify_one();
    }
    inline void event::wait()
    {
        msw::unique_lock<msw::mutex> lock(m_);
        cv_.wait
        (
            lock
            , [this](){ return v_; }
        );
        v_ = false;
    }

}}
