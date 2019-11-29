#pragma once 

#include <boost/filesystem.hpp>

#include <msw/config.hpp>
#include <msw/file_path.hpp>
#include <msw/self/exe_name.hpp>
#include <msw/execute_process.hpp>
#include <msw/command_line/make.hpp>
#include <msw/native_bitness_64.hpp>
#include <msw/command_line/build_string.hpp>


namespace msw
{
    struct self_x64_app
    {
        self_x64_app()
        {
            std::string const self_name = self_exe_name();
            filename_ = file_path::directory(self_name) + "/" + file_path::basic(self_name) + "64" + file_path::extension(self_name);
            arguments_ = build_command_line_string(make_command_line().arguments());
        }
        bool advisable_execute() const
        {
#ifdef MSW_BITNESS_32
            return native_bitness_64();
#else
            return false;
#endif
        }
        bool execute() const
        {
            if (advisable_execute())
            {
                execute_process(filename_, arguments_);
                return true;
            }
            return false;
        }
        std::string file_name() const
        {
            return filename_;
        }
        bool file_exists() const
        {
            return boost::filesystem::exists(filename_);
        }
        void set_file_name(std::string const& name)
        {
            filename_ = name;
        }
        void set_arguments(std::string const& arguments)
        {
            arguments_ = arguments;
        }
    private:
        std::string filename_  ;
        std::string arguments_ ;
    };
}
