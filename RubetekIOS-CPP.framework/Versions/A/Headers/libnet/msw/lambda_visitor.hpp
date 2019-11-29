#pragma once

#include <boost/variant.hpp>


namespace msw
{
    /*
    // not compile on g++
    template <typename ReturnType, typename... Lambdas>
    struct lambda_visitor
        : public boost::static_visitor<ReturnType>
        , public Lambdas...
    {
        lambda_visitor(Lambdas... lambdas)
            : Lambdas(lambdas)...
        {}
    };
    */

    template <typename ReturnType, typename... Lambdas>
    struct lambda_visitor;

    template <typename ReturnType, typename Lambda1, typename... Lambdas>
    struct lambda_visitor<ReturnType, Lambda1, Lambdas...>
        : public lambda_visitor<ReturnType, Lambdas...>
        , public Lambda1
    {
        using Lambda1::operator();
        using lambda_visitor<ReturnType, Lambdas...>::operator();
        lambda_visitor(Lambda1 l1, Lambdas... lambdas)
            : lambda_visitor<ReturnType, Lambdas...>(lambdas...)
            , Lambda1(l1)
        {}
    };

    template <typename ReturnType, typename Lambda1>
    struct lambda_visitor<ReturnType, Lambda1>
        : public boost::static_visitor<ReturnType>
        , public Lambda1
    {
        using Lambda1::operator();
        lambda_visitor(Lambda1 l1)
            : boost::static_visitor<ReturnType>()
            , Lambda1(l1)
        {}
    };

    template <typename ReturnType>
    struct lambda_visitor<ReturnType>
        : public boost::static_visitor<ReturnType>
    {
        lambda_visitor()
            : boost::static_visitor<ReturnType>()
        {}
    };


    template <typename ReturnType, typename... Lambdas>
    lambda_visitor<ReturnType, Lambdas...> make_lambda_visitor(Lambdas... lambdas)
    {
        return { lambdas... };
    }

    template <typename ReturnType, typename Variant, typename... Lambdas>
    ReturnType apply_lambda_visitor(Variant&& v, Lambdas... lambdas)
    {
        auto visitor = make_lambda_visitor<ReturnType>(lambdas...);
        return boost::apply_visitor(visitor, v);
    }

    template <typename Variant, typename... Lambdas>
    void apply_lambda_void_visitor(Variant&& v, Lambdas... lambdas)
    {
        auto visitor = make_lambda_visitor<void>(lambdas...);
        boost::apply_visitor(visitor, v);
    }
}
