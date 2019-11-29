#pragma once

#include <msw/plain_types.hpp>
#include <msw/include/windows.hpp>
#include <msw/percentage_relation.hpp>


namespace dawn {
namespace win  {

    struct cpu_usage
    {
        typedef msw::uint64 time_type;

        static_assert(sizeof(FILETIME) == sizeof(time_type), "size difference of the variables");

        cpu_usage()
        {
            get_system_times(idle_, kernel_, user_);
        }

        void get(int& total_usage)
        {
            int kernel_usage;
            get(total_usage, kernel_usage);
        }
        void get(int& total_usage, int& kernel_usage)
        {
            time_type const prev_idle   = idle_   ;
            time_type const prev_kernel = kernel_ ;
            time_type const prev_user   = user_   ;
            get_system_times(idle_, kernel_, user_);
            time_type const total = (kernel_ - prev_kernel) + (user_ - prev_user);
            time_type const idle = idle_ - prev_idle;
            total_usage  = msw::percentage_relation(total - idle, total);
            kernel_usage = msw::percentage_relation((kernel_ - prev_kernel) - idle, total);
        }

    private:
        void get_system_times(time_type& idle, time_type& kernel, time_type& user)
        {
            ::GetSystemTimes(reinterpret_cast<PFILETIME>(&idle), reinterpret_cast<PFILETIME>(&kernel), reinterpret_cast<PFILETIME>(&user));
        }

        time_type idle_   ;
        time_type kernel_ ;
        time_type user_   ;
    };

}}
