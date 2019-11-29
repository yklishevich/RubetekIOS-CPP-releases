#pragma once

#include <string>

#include <msw/format.hpp>
#include <msw/plain_types.hpp>
#include <msw/throw_runtime_error.hpp>


namespace dawn {
namespace win  {

    enum class processor_architecture
    {
          x86
        , arm     = 5
        , itanium = 6
        , x64     = 9
        , unknown = 0xffff
    };

    enum class processor_type
    {
          intel_386     = 386
        , intel_486     = 486
        , intel_pentium = 586
        , intel_ia64    = 2200
        , amd_x8664     = 8664
    };

    struct system_info
    {
        processor_architecture processor_architecture  ;
        processor_type         processor_type          ;
        msw::uint32            processors_count        ;
        msw::uint32            page_size               ;
        void*                  min_application_address ;
        void*                  max_application_address ;
    };


    inline system_info system_info_from_api(SYSTEM_INFO const& api_info)
    {
        system_info info;
        info.processor_architecture  = static_cast<processor_architecture>( api_info.wProcessorArchitecture)     ;
        info.processor_type          = static_cast<processor_type>(         api_info.dwProcessorType)            ;
        info.processors_count        =                                      api_info.dwNumberOfProcessors        ;
        info.page_size               =                                      api_info.dwPageSize                  ;
        info.min_application_address =                                      api_info.lpMinimumApplicationAddress ;
        info.max_application_address =                                      api_info.lpMaximumApplicationAddress ;
        return info;
    }

    inline system_info get_system_info()
    {
        SYSTEM_INFO inf;
        ::GetSystemInfo(&inf);
        return system_info_from_api(inf);
    }
    inline system_info get_native_system_info()
    {
        SYSTEM_INFO inf;
        ::GetNativeSystemInfo(&inf);
        return system_info_from_api(inf);
    }

    char const* processor_architecture_to_str(processor_architecture v)
    {
#define MSW_WIN_PROCESSOR_ARCHITECTURE_TO_STR(V)\
        if (v == processor_architecture::V)\
        {\
            static char const* const s_##V = #V;\
            return s_##V;\
        }
        MSW_WIN_PROCESSOR_ARCHITECTURE_TO_STR( x86     );
        MSW_WIN_PROCESSOR_ARCHITECTURE_TO_STR( arm     );
        MSW_WIN_PROCESSOR_ARCHITECTURE_TO_STR( itanium );
        MSW_WIN_PROCESSOR_ARCHITECTURE_TO_STR( x64     );
        MSW_WIN_PROCESSOR_ARCHITECTURE_TO_STR( unknown );
#undef MSW_WIN_PROCESSOR_ARCHITECTURE_TO_STR
        msw::throw_runtime_error("failed to convert to string: unknown processor architecture: ", static_cast<msw::uint>(v));
#include <msw/throw_hide_warning.hpp>
    }

    char const* processor_type_to_str(processor_type v)
    {
#define MSW_WIN_PROCESSOR_TYPE_TO_STR(V)\
        if (v == processor_type::V)\
        {\
            static char const* const s_##V = #V;\
            return s_##V;\
        }
        MSW_WIN_PROCESSOR_TYPE_TO_STR( intel_386     );
        MSW_WIN_PROCESSOR_TYPE_TO_STR( intel_486     );
        MSW_WIN_PROCESSOR_TYPE_TO_STR( intel_pentium );
        MSW_WIN_PROCESSOR_TYPE_TO_STR( intel_ia64    );
        MSW_WIN_PROCESSOR_TYPE_TO_STR( amd_x8664     );
#undef MSW_WIN_PROCESSOR_TYPE_TO_STR
        msw::throw_runtime_error("failed to convert to string: unknown processor type: ", static_cast<msw::uint>(v));
#include <msw/throw_hide_warning.hpp>
    }

    std::string system_info_to_str(system_info i, std::string const delimiter = "; ")
    {
        return msw::format
        (
                         "processor architecture: "  , processor_architecture_to_str(i.processor_architecture)
            , delimiter, "processor type: "          , processor_type_to_str(i.processor_type)
            , delimiter, "processors count: "        , i.processors_count
            , delimiter, "page size: "               , i.page_size
            , delimiter, "min application address: " , i.min_application_address
            , delimiter, "max application address: " , i.max_application_address
        );
    }

}}
