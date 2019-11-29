#pragma once

#include <msw/plain_types.hpp>
#include <msw/include/windows.hpp>
#include <msw/percentage_relation.hpp>


namespace dawn {
namespace win  {

    struct memory_status
    {
        msw::uint64 phisical_total       () const ;
        msw::uint64 phisical_available   () const ;
        msw::uint64 phisical_load        () const ;
        int         phisical_usage       () const ;
        msw::uint64 pagin_file_total     () const ;
        msw::uint64 pagin_file_available () const ;
        msw::uint64 pagin_file_load      () const ;
        int         pagin_file_usage     () const ;
        void        update               ()       ;
                    memory_status        ()       ;
    private:
        msw::uint64 phisical_total_       ;
        msw::uint64 phisical_available_   ;
        msw::uint64 pagin_file_total_     ;
        msw::uint64 pagin_file_available_ ;
    };

}}


namespace dawn {
namespace win  {

    inline msw::uint64 memory_status::phisical_total() const
    {
        return phisical_total_;
    }
    inline msw::uint64 memory_status::phisical_available() const
    {
        return phisical_available_;
    }
    inline msw::uint64 memory_status::phisical_load() const
    {
        return phisical_total() - phisical_available();
    }
    inline int memory_status::phisical_usage() const
    {
        return msw::percentage_relation(phisical_load(), phisical_total());
    }
    inline msw::uint64 memory_status::pagin_file_total() const
    {
        return pagin_file_total_;
    }
    inline msw::uint64 memory_status::pagin_file_available() const
    {
        return pagin_file_available_;
    }
    inline msw::uint64 memory_status::pagin_file_load() const
    {
        return pagin_file_total() - pagin_file_available();
    }
    inline int memory_status::pagin_file_usage() const
    {
        return msw::percentage_relation(pagin_file_load(), pagin_file_total());
    }

    inline void memory_status::update()
    {
        MEMORYSTATUSEX status;
        status.dwLength = sizeof(status);
        ::GlobalMemoryStatusEx(&status);
        phisical_total_       = status.ullTotalPhys                           ;
        phisical_available_   = status.ullAvailPhys                           ;
        pagin_file_total_     = status.ullTotalPageFile - status.ullTotalPhys ;
        pagin_file_available_ = status.ullAvailPageFile - status.ullAvailPhys ;
    }
    inline memory_status::memory_status()
    {
        update();
    }

}}
