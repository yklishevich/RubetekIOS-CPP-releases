#pragma once

#include <ctime>
#include <memory>
#include <string>
#include <vector>
#include <fstream>
#include <functional>

#include <boost/format.hpp>
#include <boost/filesystem.hpp>

#include <msw/config.hpp>
#include <msw/format.hpp>
#include <msw/cwrite.hpp>
#include <msw/std/mutex.hpp>
#include <msw/std/thread.hpp>
#include <msw/dimension/trigger.hpp>
#include <msw/throw_runtime_error.hpp>
#include <msw/lexical_cast/int_to/fix_hex.hpp>

#include <lama/boost/include/date_time/posix_time/posix_time.hpp>

#ifdef MSW_LINUX
#   include <dawn/log/syslog.hpp>
#endif

#include <dawn/chronometer.hpp>


namespace dawn
{
    struct log
    {
        enum class level
        {
              debug
            , info
            , warning
            , error
            , critical
        };

        static level       level_from_string    (std::string const&);
        static std::string level_to_string      (level             );
        static std::string level_to_curt_string (level             );

        struct config
        {
            typedef std::function<void(std::string const&)>             message_handler_type       ;
            typedef std::function<void(log::level, std::string const&)> typed_message_handler_type ;

            log::level                 level                 ;
            bool                       console               ;
            std::string                syslog                ;
            int                        syslog_facility       ;
            std::string                file                  ;
            bool                       file_append           ;
            message_handler_type       message_handler       ;
            typed_message_handler_type typed_message_handler ;

            config();
            explicit config(log::level);
        };

        struct statistics
        {
            msw::uint64 debug    = 0 ;
            msw::uint64 info     = 0 ;
            msw::uint64 warning  = 0 ;
            msw::uint64 error    = 0 ;
            msw::uint64 critical = 0 ;
        };

        static void init (config const&)                                 ;
        static void init (std::string const& file, bool console = false) ;
        static void stat (level = level::info, std::string = "stat")     ;

        template <typename ...Args> static void print         ( level, Args&&...                            ) ;
        template <typename ...Args> static void pref_print    ( level, std::string const& prefix, Args&&... ) ;
        template <typename ...Args> static void debug         ( Args&&...                                   ) ;
        template <typename ...Args> static void info          ( Args&&...                                   ) ;
        template <typename ...Args> static void warning       ( Args&&...                                   ) ;
        template <typename ...Args> static void error         ( Args&&...                                   ) ;
        template <typename ...Args> static void critical      ( Args&&...                                   ) ;
        template <typename ...Args> static void pref_debug    ( std::string const& prefix, Args&&...        ) ;
        template <typename ...Args> static void pref_info     ( std::string const& prefix, Args&&...        ) ;
        template <typename ...Args> static void pref_warning  ( std::string const& prefix, Args&&...        ) ;
        template <typename ...Args> static void pref_error    ( std::string const& prefix, Args&&...        ) ;
        template <typename ...Args> static void pref_critical ( std::string const& prefix, Args&&...        ) ;

    private:
        struct kernel
        {
             std::thread::id                    const main_thread_id        ;
             chronometer                        const lifespan              ;
             log::level                               level                 ;
             bool                                     console               ;
MSW_ON_LINUX(std::unique_ptr<sys_log>                 syslog                ;)
             std::string                              file_name             ;
             std::unique_ptr<std::ofstream>           file                  ;
             config::message_handler_type             message_handler       ;
             config::typed_message_handler_type       typed_message_handler ;
             statistics                               stat                  ;
             unsigned                                 thread_id_max_width   ;
             std::vector<std::thread::id>             thread_id_index       ;

             kernel();
        };

        static kernel const& get_kernel        ()                                                                   ;
        static bool          permitted_level   (log::level const&)                                                  ;
        static std::string   now_time          ()                                                                   ;
        static std::string   thread_id         ()                                                                   ;
        static std::string   thread_id_aligned ()                                                                   ;
        static void          private_print     (level const&, std::string const& msg, std::string const& msg_short) ;

        static msw::mutex mutex_  ;
        static kernel     kernel_ ;
    };
}


namespace dawn
{
    inline log::level log::level_from_string(std::string const& s)
    {
#define DAWN_LOG_LEVEL_FROM_STRING(L) if (s == #L) return level::L;
        DAWN_LOG_LEVEL_FROM_STRING( debug    )
        DAWN_LOG_LEVEL_FROM_STRING( info     )
        DAWN_LOG_LEVEL_FROM_STRING( warning  )
        DAWN_LOG_LEVEL_FROM_STRING( error    )
        DAWN_LOG_LEVEL_FROM_STRING( critical )
#undef  DAWN_LOG_LEVEL_FROM_STRING
        msw::throw_runtime_error("unknown log level '", s, "'");
#include <msw/throw_hide_warning.hpp>
    }
    inline std::string log::level_to_string(log::level lv)
    {
#define DAWN_LOG_LEVEL_TO_STRING(L) if (lv == level::L) return #L;
        DAWN_LOG_LEVEL_TO_STRING( debug    )
        DAWN_LOG_LEVEL_TO_STRING( info     )
        DAWN_LOG_LEVEL_TO_STRING( warning  )
        DAWN_LOG_LEVEL_TO_STRING( error    )
        DAWN_LOG_LEVEL_TO_STRING( critical )
#undef  DAWN_LOG_LEVEL_TO_STRING
        MSW_ASSERT(0);
        return "";
    }
    inline std::string log::level_to_curt_string(log::level lv)
    {
#define DAWN_LOG_LEVEL_TO_STRING(L, N) if (lv == level::L) return N;
        DAWN_LOG_LEVEL_TO_STRING( debug    , "dbg" )
        DAWN_LOG_LEVEL_TO_STRING( info     , "inf" )
        DAWN_LOG_LEVEL_TO_STRING( warning  , "wrn" )
        DAWN_LOG_LEVEL_TO_STRING( error    , "err" )
        DAWN_LOG_LEVEL_TO_STRING( critical , "crt" )
#undef  DAWN_LOG_LEVEL_TO_STRING
        MSW_ASSERT(0);
        return "";
    }

    inline void log::init(config const& conf)
    {
        msw::unique_lock<msw::mutex> lock(mutex_);
        log::kernel_.level   = conf.level   ;
        log::kernel_.console = conf.console ;
        MSW_ON_LINUX
        (
            if (!conf.syslog.empty()) log::kernel_.syslog.reset(new sys_log(conf.syslog, conf.syslog_facility));
        )
        if (log::kernel_.file_name != conf.file)
        {
            if (conf.file.empty()) log::kernel_.file.reset();
            else
            {
                boost::filesystem::path path(conf.file);
                if (path.has_parent_path() && !boost::filesystem::exists(path.parent_path()))
                    boost::filesystem::create_directories(path.parent_path());
                std::ios_base::openmode mode =  std::ios_base::out;
                if (conf.file_append)   mode |= std::ios_base::app;
                log::kernel_.file.reset(new std::ofstream(conf.file, mode));
            }
            log::kernel_.file_name = conf.file;
        }
        log::kernel_.message_handler       = conf.message_handler       ;
        log::kernel_.typed_message_handler = conf.typed_message_handler ;
    }
    inline void log::init(std::string const& file, bool console)
    {
        config conf;
        conf.file    = file    ;
        conf.console = console ;
        init(conf);
    }
    inline void log::stat(log::level lv, std::string m)
    {
        statistics const s = log::kernel_.stat;
#define DAWN_LOG_STAT_INFO(L) , s.L ? msw::format(" ", #L, "[", s.L, "]") : std::string()
        std::string const msg = msw::format
        (
            m, ": time[", log::kernel_.lifespan.duration_string(), "]"
            , " threads[", log::kernel_.thread_id_index.size() + 1, "]"
            DAWN_LOG_STAT_INFO( debug    )
            DAWN_LOG_STAT_INFO( info     )
            DAWN_LOG_STAT_INFO( warning  )
            DAWN_LOG_STAT_INFO( error    )
            DAWN_LOG_STAT_INFO( critical )
        );
#undef DAWN_LOG_STAT_INFO
        print(lv, msg);
    }

    template <typename ...Args>
    void log::print(level lv, Args&& ...args)
    {
        if (!permitted_level(lv)) return;
        std::string const time     = now_time();
        std::string const lev      = level_to_curt_string(lv);
        std::string const pure_msg = msw::format(std::forward<Args>(args)...);
        msw::unique_lock<msw::mutex> lock(mutex_);
        std::string const msg = msw::format(time, " [", thread_id_aligned(), "|", lev, "] ", pure_msg);
        if (kernel_.console || (lv == level::critical)) msw::cwriteln(msg);
        switch (lv)
        {
            case level::debug    : kernel_.stat.debug    ++ ; break ;
            case level::info     : kernel_.stat.info     ++ ; break ;
            case level::warning  : kernel_.stat.warning  ++ ; break ;
            case level::error    : kernel_.stat.error    ++ ; break ;
            case level::critical : kernel_.stat.critical ++ ; break ;
        }
#ifdef MSW_LINUX
        int priority = 0;
        switch (lv)
        {
            case level::debug    : priority = LOG_DEBUG   ; break ;
            case level::info     : priority = LOG_INFO    ; break ;
            case level::warning  : priority = LOG_WARNING ; break ;
            case level::error    : priority = LOG_ERR     ; break ;
            case level::critical : priority = LOG_CRIT    ; break ;
        }
        if (kernel_.syslog) kernel_.syslog->push(priority, pure_msg);
#endif
        if (kernel_.file                  ) *kernel_.file << msg << std::endl;
        if (kernel_.message_handler       ) kernel_.message_handler(msg);
        if (kernel_.typed_message_handler ) kernel_.typed_message_handler(lv, msg);
    }
    template <typename ...Args>
    void log::pref_print(level lv, std::string const& prefix, Args&& ...args)
    {
        print(lv, "<" + prefix + ">: ", std::forward<Args>(args)...);
    }

#define DAWN_LOG_PRINT(L)\
    template <typename ...Args>\
    void log::L(Args&& ...args)\
    {\
        print(level::L, std::forward<Args>(args)...);\
    }
    DAWN_LOG_PRINT( debug    )
    DAWN_LOG_PRINT( info     )
    DAWN_LOG_PRINT( warning  )
    DAWN_LOG_PRINT( error    )
    DAWN_LOG_PRINT( critical )
#undef DAWN_LOG_PRINT
#define DAWN_LOG_PRINT_PREF(L)\
    template <typename ...Args>\
    void log::pref_##L(std::string const& prefix, Args&& ...args)\
    {\
        pref_print(level::L, prefix, std::forward<Args>(args)...);\
    }
    DAWN_LOG_PRINT_PREF( debug    )
    DAWN_LOG_PRINT_PREF( info     )
    DAWN_LOG_PRINT_PREF( warning  )
    DAWN_LOG_PRINT_PREF( error    )
    DAWN_LOG_PRINT_PREF( critical )
#undef DAWN_LOG_PRINT_PREF

    inline log::kernel const& log::get_kernel()
    {
        return kernel_;
    }
    inline bool log::permitted_level(log::level const& lv)
    {
        level const lv_ = get_kernel().level;
        return (lv >= lv_) || (lv == level::critical);
    }
    inline std::string log::now_time()
    {
        boost::posix_time::ptime const now = boost::posix_time::microsec_clock::local_time();
        std::string s = boost::posix_time::to_simple_string(now);
        if (s.size() > 24) s = s.substr(0, 24);
        return s;
    }
    inline std::string log::thread_id()
    {
        msw::thread::id const id = msw::this_thread::get_id();
        if (id == kernel_.main_thread_id) return "M";
        unsigned n = 0;
        for (; n != kernel_.thread_id_index.size(); n++)
            if (kernel_.thread_id_index[n] == id) break;
        if (n == kernel_.thread_id_index.size()) kernel_.thread_id_index.push_back(id);
        return std::to_string(n + 2);
    }
    inline std::string log::thread_id_aligned()
    {
        std::string s = thread_id();
        unsigned const len = static_cast<unsigned>(s.size());
        if (kernel_.thread_id_max_width != len)
        {
            if (kernel_.thread_id_max_width > len)
            {
                bool const is_text = s == "M";
                unsigned n = kernel_.thread_id_max_width - len;
                for (; n; --n) s = (is_text ? " " : "0") + s;
            }
            else kernel_.thread_id_max_width = len;
        }
        return s;
    }

    inline log::config::config(log::level lev)
             : level           ( lev      )
             , console         ( false    )
MSW_ON_LINUX(, syslog_facility ( LOG_USER ))
             , file_append     ( false    )
    {}
    inline log::config::config()
        : config(log::level::debug)
    {}

    inline log::kernel::kernel()
        : main_thread_id      ( msw::this_thread::get_id() )
        , level               ( log::level::critical       )
        , console             ( false                      )
        , thread_id_max_width ( 1                          )
    {}
}
