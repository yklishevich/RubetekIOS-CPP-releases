#pragma once

#include <tuple>

#include <msw/config.hpp>
#ifdef MSW_NO_RESULT_OF
#   include <boost/utility/result_of.hpp>
#endif


namespace msw         {
namespace entry_point {
namespace zzz         {

    template <typename Function, typename Tuple>
    struct function_tuple_args {};

    template <typename Function, typename Tuple>
    void call_function_tuple_args(Function&& fn, Tuple tuple)
    {
        function_tuple_args<Function, Tuple>::call(std::forward<Function>(fn), tuple);
    }


    template<int ...>
    struct seq {};

    template<int N, int ...S>
    struct gen_seq 
        : gen_seq<N - 1, N - 1, S...> 
    {};

    template<int ...S>
    struct gen_seq<0, S...>
    {
        typedef seq<S...> type;
    };


    template <typename Function, typename ...Args>
    struct function_tuple_args<Function, std::tuple<Args...>>
    {
        typedef typename
#ifdef MSW_NO_RESULT_OF
        boost
#else
        std
#endif
        ::result_of<Function(Args...)>::type result;

        bool static const no_return = std::is_same<result, void>::value;


        template<typename Fn, typename TupleArgs, bool is_procedure = no_return>
        static
        typename std::enable_if<is_procedure, void>::type
        call(Fn&& fn, TupleArgs&& fn_args)
        {
            call_impl(std::forward<Fn>(fn), std::forward<TupleArgs>(fn_args), typename gen_seq<sizeof...(Args)>::type());
        }

        template<typename Fn, typename TupleArgs, bool is_procedure = no_return>
        static
        typename std::enable_if<!is_procedure, result>::type
        call(Fn&& fn, TupleArgs&& fn_args)
        {
            return call_impl(std::forward<Fn>(fn), std::forward<TupleArgs>(fn_args), typename gen_seq<sizeof...(Args)>::type());
        }

    private:

        template<typename Fn, typename TupleArgs, bool is_procedure = no_return, int ...S>
        static
        typename std::enable_if<is_procedure, void>::type
        call_impl(Fn&& fn, TupleArgs&& fn_args, seq<S...>)
        {
            fn(std::get<S>(std::forward<TupleArgs>(fn_args))...);
        }

        template<typename Fn, typename TupleArgs, bool is_procedure = no_return, int ...S>
        static
        typename std::enable_if<!is_procedure, result>::type
        call_impl(Fn&& fn, TupleArgs&& fn_args, seq<S...>)
        {
            return fn(std::get<S>(std::forward<TupleArgs>(fn_args))...);
        }
    };

}}}
