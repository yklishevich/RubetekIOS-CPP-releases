#ifndef CC_REQUEST_THREAD_HPP
#define CC_REQUEST_THREAD_HPP

namespace rubetek
{
    namespace module
    {
        struct request_threads
        {
            struct request_thread
            {
                std::shared_ptr<std::thread>    thread;
                bool                            is_completed;

                request_thread();
            };

            typedef std::list<request_thread>::iterator thread_it;

            thread_it add_thread();

            ~request_threads();
          private:

            void check_completed(bool wait_all);

            std::list<request_thread>           threads;
        };

        request_threads::request_thread::request_thread()
            : is_completed(false)
        {
        }

        request_threads::thread_it request_threads::add_thread()
        {
            check_completed(false);

            threads.push_back(request_thread());
            auto res = --threads.end();
            return res;
        }

        void request_threads::check_completed(bool wait_all)
        {
            for (thread_it it = threads.begin(); it != threads.end();)
            {
                if (it->thread)
                {
                    if (wait_all || it->is_completed)
                    {
                        if (it->thread->joinable())
                        {
                            it->thread->detach();
                        }
                        threads.erase(it++);
                    } else
                    {
                        ++it;
                    }
                }
            }
        }

        request_threads::~request_threads()
        {
            check_completed(true);
        }
    }
}

#endif //CC_REQUEST_THREAD_HPP
