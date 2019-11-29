#include <string>

#include <msw/buffer.hpp>
#include <msw/system_error/throw.hpp>


namespace dawn {
namespace win  {

    inline std::string get_environment_variable(std::string const& name)
    {
        msw::uint result = ::GetEnvironmentVariable(name.data(), 0, 0);
        if (!result) msw::system_error::throw_exception("get environment variable '", name,"' fail");
        msw::buffer<char> s(result + 1);
        result = ::GetEnvironmentVariable(name.data(), s.data().native(), static_cast<unsigned>(s.size().count()));
        if (!result) msw::system_error::throw_exception("get environment variable '", name,"' fail");
        MSW_ASSERT(s.size().count() >= result);
        return s.data().native();
    }
    inline void set_environment_variable(std::string const& name, std::string const& value)
    {
        if (!::SetEnvironmentVariable(name.data(), value.data())) msw::system_error::throw_exception("set environment variable '", name,"' fail");
    }

}}
