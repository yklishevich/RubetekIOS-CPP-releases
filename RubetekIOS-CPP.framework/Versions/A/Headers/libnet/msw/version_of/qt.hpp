#pragma once

#include <QtGlobal>

#include <msw/format.hpp>


namespace msw
{
    static std::string version_of_qt()
    {
        std::string const compile_version = QT_VERSION_STR ;
        std::string const runtime_version = qVersion()     ;
        if (compile_version == runtime_version) return runtime_version;
        return msw::format(compile_version, "(rt-", runtime_version, ")");
    }
}
