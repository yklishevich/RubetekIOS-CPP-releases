#pragma once

#include <array>

#include <msw/buffer.hpp>
#include <msw/include/windows.hpp>
#include <msw/system_error/throw.hpp>
#include <msw/regibits/location/population_count.hpp>


namespace dawn {
namespace win  {

    struct cpu_info
    {
        unsigned processor_count         () const ;
        unsigned core_count              () const ;
        unsigned logical_processor_count () const ;
        unsigned l1_cache_count          () const ;
        unsigned l2_cache_count          () const ;
        unsigned l3_cache_count          () const ;
                 cpu_info                ()       ;
    private:
        unsigned                processor_count_         = 0   ;
        unsigned                core_count_              = 0   ;
        unsigned                logical_processor_count_ = 0   ;
        std::array<unsigned, 3> cache_count_             = {0} ;
    };

}}


namespace dawn {
namespace win  {

    inline unsigned cpu_info::processor_count() const
    {
        return processor_count_;
    }
    inline unsigned cpu_info::core_count() const
    {
        return core_count_;
    }
    inline unsigned cpu_info::logical_processor_count() const
    {
        return logical_processor_count_;
    }
    inline unsigned cpu_info::l1_cache_count() const
    {
        return cache_count_[0];
    }
    inline unsigned cpu_info::l2_cache_count() const
    {
        return cache_count_[1];
    }
    inline unsigned cpu_info::l3_cache_count() const
    {
        return cache_count_[2];
    }

    inline cpu_info::cpu_info()
    {
        msw::buffer<msw::byte> buffer;
        bool done = false;
        while (!done)
        {
            DWORD len = static_cast<DWORD>(buffer.size().count());
            if (!::GetLogicalProcessorInformation(reinterpret_cast<PSYSTEM_LOGICAL_PROCESSOR_INFORMATION>(buffer.data().native()), &len))
            {
                if (::GetLastError() == ERROR_INSUFFICIENT_BUFFER) buffer.resize(len);
                else msw::system_error::throw_exception("get logical processor infomation fail");
            }
            else break;
        }
        msw::range<msw::byte> data = buffer.all();
        while (data.size().count() >= sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION))
        {
            PSYSTEM_LOGICAL_PROCESSOR_INFORMATION ptr = reinterpret_cast<PSYSTEM_LOGICAL_PROCESSOR_INFORMATION>(data.data().native());
            data.pop_front(sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION));
            switch (ptr->Relationship)
            {
            case RelationNumaNode:
                break;
            case RelationProcessorCore:
                core_count_++;
                logical_processor_count_ += msw::regibits::population_count(ptr->ProcessorMask);
                break;
            case RelationCache:
                if (cache_count_.size() >= ptr->Cache.Level) cache_count_[ptr->Cache.Level - 1]++;
                break;
            case RelationProcessorPackage:
                processor_count_++;
                break;
            default:
                msw::throw_runtime_error("unsupported logical processor relationship");
            }
        }
    }

}}
