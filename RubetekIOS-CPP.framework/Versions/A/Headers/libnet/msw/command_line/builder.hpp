#pragma once

#include <list>
#include <string>

#include <msw/buffer.hpp>


namespace msw
{
    struct command_line_builder
    {
        explicit command_line_builder(std::string const& command)
        {
            add_argument(command);
        }
        void add_argument(std::string const& argument)
        {
            if (!argument.empty())
            {
                arguments_.push_back(argument);
                line_.clear();
            }
        }

        buffer<byte>&& line()
        {
            if (line_.empty()) build();
            return std::move(line_);
        }
        buffer<byte> const& line() const
        {
            if (line_.empty()) build();
            return line_;
        }

        int argc() const
        {
            return static_cast<int>(arguments_.size());
        }

        char const* const* argv() const
        {
            return reinterpret_cast<char const* const*>(line().data().native());
        }

    private:

        void build() const
        {
            size<byte> ptrs_size = (arguments_.size() + 1) * sizeof(char*);
            size<byte> args_size = 0;
            for (std::string const& arg : arguments_)
                args_size += (arg.size() + 1);
            line_.reserve(ptrs_size + args_size);
            line_.resize(ptrs_size);
            range<uint> ptrs = line_.all<uint>();
            ptrs.pop_back() = 0;
            for (std::string const& arg : arguments_)
            {
                ptrs.pop_front() = reinterpret_cast<uint>(line_.end().to_pointer().native());
                line_.push_back_silent(make_range<byte>(arg));
                line_.push_back_silent(byte(0));
            }
        }

        std::list<std::string>         arguments_ ;
        buffer<byte>           mutable line_      ;
    };
}
