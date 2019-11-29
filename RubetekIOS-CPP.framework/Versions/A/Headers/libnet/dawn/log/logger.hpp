#pragma once

#include <string>
#include <type_traits>

#include <boost/optional.hpp>

#include <msw/format.hpp>
#include <msw/noncopyable.hpp>

#include <dawn/log/log.hpp>


namespace dawn
{
    struct logger
        : msw::noncopyable
    {
                                    explicit logger   (std::string const& prefix                                                            ) ;
                                             logger   (std::string const& prefix, log::level lifetime_level                                 ) ;
                                             logger   (std::string const& prefix, std::string const& specificity                            ) ;
                                             logger   (std::string const& prefix, std::string const& specificity, log::level lifetime_level ) ;
        template <typename T>                logger   (std::string const& prefix, T const&                                                  ) ;
        template <typename T>                logger   (std::string const& prefix, T const&                      , log::level lifetime_level ) ;
                                             ~logger  ()                                                                                      ;

        template <typename ...Args> void     debug    (Args&& ...) const ;
        template <typename ...Args> void     info     (Args&& ...) const ;
        template <typename ...Args> void     warning  (Args&& ...) const ;
        template <typename ...Args> void     error    (Args&& ...) const ;
        template <typename ...Args> void     critical (Args&& ...) const ;

    private:

        template <typename T> typename std::enable_if<std::is_pointer<T>::value, std::string>::type  format_value(T const&);
        template <typename T> typename std::enable_if<!std::is_pointer<T>::value, std::string>::type format_value(T const&);

        std::string                 const prefix_         ;
        boost::optional<log::level> const lifetime_level_ ;
    };
}


namespace dawn
{
    inline logger::logger(std::string const& prefix)
        : prefix_( prefix )
    {}
    inline logger::logger(std::string const& prefix, log::level lifetime_level)
        : prefix_         ( prefix         )
        , lifetime_level_ ( lifetime_level )
    {
        if ( *lifetime_level_ >  log::level::info  ) throw std::runtime_error("lifetime level must be no more level::info");
        if ( *lifetime_level_ == log::level::debug ) debug ("+") ;
        if ( *lifetime_level_ == log::level::info  ) info  ("+") ;
    }
    inline logger::logger(std::string const& prefix, std::string const& specificity)
        : logger(prefix + "[" + specificity + "]")
    {}
    inline logger::logger(std::string const& prefix, std::string const& specificity, log::level lifetime_level)
        : logger(prefix + "[" + specificity + "]", lifetime_level)
    {}
    template <typename T>
    logger::logger(std::string const& prefix, T const& v)
        : logger(prefix, format_value(v))
    {}
    template <typename T>
    logger::logger(std::string const& prefix, T const& v, log::level lifetime_level)
        : logger(prefix, format_value(v), lifetime_level)
    {}

    inline logger::~logger()
    {
        if (lifetime_level_)
        {
            if ( *lifetime_level_ == log::level::debug ) debug ("~") ;
            if ( *lifetime_level_ == log::level::info  ) info  ("~") ;
        }
    }

#define DAWN_LOG_PRINT(L)\
    template <typename ...Args>\
    void logger::L(Args&& ...args) const\
    {\
        log::pref_##L(prefix_, std::forward<Args>(args)...);\
    }
    DAWN_LOG_PRINT( debug    )
    DAWN_LOG_PRINT( info     )
    DAWN_LOG_PRINT( warning  )
    DAWN_LOG_PRINT( error    )
    DAWN_LOG_PRINT( critical )
#undef DAWN_LOG_PRINT

    template <typename T>
    typename std::enable_if<std::is_pointer<T>::value, std::string>::type logger::format_value(T const& v)
    {
        return msw::format(static_cast<void const*>(v));
    }
    template <typename T>
    typename std::enable_if<!std::is_pointer<T>::value, std::string>::type logger::format_value(T const& v)
    {
        return msw::format(v);
    }
}
