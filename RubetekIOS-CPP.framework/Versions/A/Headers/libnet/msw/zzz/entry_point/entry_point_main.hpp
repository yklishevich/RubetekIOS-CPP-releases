#pragma once

#include <msw/command_line/make.hpp>
#include <msw/command_line/win_make.hpp>
#include <msw/entry_point/type.hpp>
#include <msw/entry_point/exception_handler/default.hpp>
#include <msw/zzz/entry_point/call_procedure_as_function.hpp>
#include <msw/zzz/entry_point/call_function_tuple_args.hpp>


namespace msw         {
namespace entry_point {
namespace zzz         {

    template <typename MainFn, typename ...Args>
    int call_main(MainFn&& main_fn, Args&&... args)
    {
        return call_procedure_as_function(0, main_fn, std::forward<Args>(args)...);
    }

    template <type Type = type::classic>
    struct main_impl
    {
        template <typename MainFn, typename ...Args>
        static int call(MainFn&& main_fn, Args&&... args)
        {
            return call_main(main_fn, std::forward<Args>(args)...);
        }
    };

    template <>
    struct main_impl<type::plain>
    {
        template <typename MainFn, typename ...Args>
        static int call(MainFn&& main_fn, Args&&...)
        {
            return call_main(main_fn);
        }
    };

    template <>
    struct main_impl<type::advanced>
    {
        template <typename MainFn, typename ...Args>
        static int call(MainFn&& main_fn, Args&&... args)
        {
            return call_main(main_fn, make_command_line(std::forward<Args>(args)...));
        }
    };


    template
    <
          type Type
        , typename MainFn
        , typename ExceptionHandler
        , typename UnknownExceptionHandler
        , typename ...Args
    >
    int entry_point_call
    (
          MainFn&& main_fn
        , ExceptionHandler&& exception_handler
        , UnknownExceptionHandler&& unknown_exception_handler
        , Args&&... args
    )
    {
        try
        {
            return main_impl<Type>::call(main_fn, std::forward<Args>(args)...);
        }
        catch (std::exception const& e)
        {
            return call_procedure_as_function(-1, std::forward<ExceptionHandler>(exception_handler), e);
        }
        catch (...)
        {
            return call_procedure_as_function(-1, std::forward<UnknownExceptionHandler>(unknown_exception_handler));
        }
    }


    template <typename TupleNativeArgs>
    struct entry_point_impl{};

    template <typename ...Args>
    struct entry_point_impl<std::tuple<Args...>>
    {
        template
        <
              type Type
            , typename MainFn
            , typename ExceptionHandler
            , typename UnknownExceptionHandler
            , typename NativeArgs
        >
        static int call
        (
              MainFn&& main_fn
            , ExceptionHandler&& exception_handler
            , UnknownExceptionHandler&& unknown_exception_handler
            , NativeArgs&& native_arguments
        )
        {
            return call_impl<Type>
            (
                  std::forward<MainFn>(main_fn)
                , std::forward<ExceptionHandler>(exception_handler)
                , std::forward<UnknownExceptionHandler>(unknown_exception_handler)
                , std::forward<NativeArgs>(native_arguments)
                , typename gen_seq<sizeof...(Args)>::type()
            );
        }

    private:

        template
        <
              type Type
            , typename MainFn
            , typename ExceptionHandler
            , typename UnknownExceptionHandler
            , typename NativeArgs
            , int ...S
        >
        static int call_impl
        (
              MainFn&& main_fn
            , ExceptionHandler&& exception_handler
            , UnknownExceptionHandler&& unknown_exception_handler
            , NativeArgs&& native_arguments
            , seq<S...>
        )
        {
            return entry_point_call<Type>
            (
                  std::forward<MainFn>(main_fn)
                , std::forward<ExceptionHandler>(exception_handler)
                , std::forward<UnknownExceptionHandler>(unknown_exception_handler)
                , std::get<S>(std::forward<NativeArgs>(native_arguments))...
            );
        }
    };


    template
    <
          type Type
        , typename NativeArguments
        , typename MainFn
        , typename ExceptionHandler = decltype(default_exception_handler)&
        , typename UnknownExceptionHandler = decltype(default_unknown_exception_handler)&
    >
    int entry_point_main
    (
          NativeArguments&& native_arguments
        , MainFn&& main_fn
        , ExceptionHandler&& exception_handler = default_exception_handler
        , UnknownExceptionHandler&& unknown_exception_handler = default_unknown_exception_handler
    )
    {
        return entry_point_impl<NativeArguments>::template call<Type>
        (
              std::forward<MainFn>(main_fn)
            , std::forward<ExceptionHandler>(exception_handler)
            , std::forward<UnknownExceptionHandler>(unknown_exception_handler)
            , std::forward<NativeArguments>(native_arguments)
        );
    }


    template <typename ...Args>
    int call(Args...);

}}}
