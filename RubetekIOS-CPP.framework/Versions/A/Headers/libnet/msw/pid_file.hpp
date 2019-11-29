#pragma once 

#include <string>
#include <fstream>

#include <boost/filesystem.hpp>

#include <msw/self/pid.hpp>
#include <msw/noncopyable.hpp>
#include <msw/throw_runtime_error.hpp>


namespace msw
{
    struct pid_file
        : noncopyable
    {
        explicit pid_file(std::string const& filename)
            : filename_(filename)
        {
            std::ofstream pid_file(filename);
            if (!pid_file.good()) throw_runtime_error("can't create pid file: ", filename);
            pid_file << self_pid();
        }
        ~pid_file()
        {
            boost::filesystem::remove(filename_);
        }
        std::string const& filename() const
        {
            return filename_;
        }
    private:
        std::string const filename_;
    };
}
