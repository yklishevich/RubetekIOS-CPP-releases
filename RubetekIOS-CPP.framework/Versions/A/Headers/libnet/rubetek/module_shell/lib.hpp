#pragma once

#include <array>
#include <string>

#include <msw/dll.hpp>
#include <msw/assert.hpp>
#include <msw/throw_runtime_error.hpp>

#include <rubetek/module/version.hpp>
#include <rubetek/module/profile.hpp>
#include <rubetek/module/interface.hpp>


namespace rubetek      {
namespace module_shell {

    struct lib
    {
        explicit lib(std::string const& path)
            : dll_            ( path    )
            , module_init_    ( nullptr )
            , module_version_ ( nullptr )
            , module_free_    ( nullptr )
        {
            dll_.link_function(module_init_    , "module_init"    );
            dll_.link_function(module_version_ , "module_version" );
            dll_.link_function(module_free_    , "module_free"    );
        }

        module::controller_module_link init(rubetek::module::profile const& profile) const
        {
            MSW_ASSERT(module_init_);
            std::array<char, 1024> err_msg;
            module::controller_module_link const link = module_init_(profile, err_msg.data(), static_cast<int>(err_msg.size()));
            err_msg.back() = 0;
            if (!link) msw::throw_runtime_error("init fail: ", std::string(err_msg.data()));
            return link;
        }
        module::version_type version() const
        {
            MSW_ASSERT(module_version_);
            return module_version_();
        }

        bool free() const
        {
            MSW_ASSERT(module_free_);
            bool const res = module_free_();
            return res;
        }

    private:

        typedef module::controller_module_link (*module_init_t)    (rubetek::module::profile const&, char* error, int error_len) ;
        typedef module::version_type           (*module_version_t) ()                                                            ;
        typedef bool                           (*module_free_t)    ()                                                            ;

        msw::dll::module const dll_            ;
        module_init_t          module_init_    ;
        module_version_t       module_version_ ;
        module_free_t          module_free_    ;
    };

}}
