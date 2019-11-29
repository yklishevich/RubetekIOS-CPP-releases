#pragma once

#include <boost/fusion/container/set.hpp>
#include <boost/fusion/include/set.hpp>
#include <boost/fusion/container/set/set_fwd.hpp>
#include <boost/fusion/include/set_fwd.hpp>
#include <boost/fusion/sequence/intrinsic/at_key.hpp>
#include <boost/fusion/algorithm/iteration/for_each.hpp>
#include <boost/fusion/include/for_each.hpp>

#include <msw/config.hpp>
#include <msw/assert.hpp>
#include <msw/noncopyable.hpp>


namespace rubetek
{
    template <typename ...Interfaces>
    struct interface_binder
        : msw::noncopyable
    {
        interface_binder()
        {
            boost::fusion::for_each(s_, initializer());
        }

        template <typename Interface>
        void bind(Interface* i)
        {
            get_ref<Interface>() = i;
        }
        template <typename Interface>
        void unbind()
        {
            bind<Interface>(nullptr);
        }
        template <typename Interface>
        bool has_bind() const
        {
            return get_ref<Interface>() != nullptr;
        }
        template <typename Interface>
        Interface* get() const
        {
            MSW_ASSERT(has_bind<Interface>());
            return get_ref<Interface>();
        }

    private:

        template <typename Interface>
        Interface*& get_ref()
        {
            return boost::fusion::at_key<Interface*>(s_);
        }
        template <typename Interface>
        Interface* const& get_ref() const
        {
            return boost::fusion::at_key<Interface*>(s_);
        }

        struct initializer
        {
            template<typename T>
            void operator()(T& t) const
            {
                t = nullptr;
            }
        };

        boost::fusion::set<Interfaces*...> s_;
    };

    template <typename Binder, typename Interface>
    struct self_interface_binder
        : msw::noncopyable
    {
        self_interface_binder(Binder& binder, Interface* i)
            : binder_(binder)
        {
            binder_.template bind<Interface>(i);
        }
        ~self_interface_binder()
        {
            binder_.template unbind<Interface>();
        }

        Binder& binder() const
        {
            return binder_;
        }

    private:
        Binder& binder_;
    };
}
