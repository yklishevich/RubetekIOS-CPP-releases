#pragma once

#include <memory>

#include <msw/zzz/validator/root/fwd.hpp>


namespace msw {
namespace zzz {

    struct validator_link
    {
                        validator_link ()                            ;

                        validator_link (validator_link const&)       ;
        validator_link& operator =     (validator_link const&)       ;

                        validator_link (validator_link&&)            ;
        validator_link& operator =     (validator_link&&)            ;

        bool            check          ()                      const ;
        bool            same_root      (validator_link const&) const ;

    private:

        struct fake {};

        friend struct validator_root;
        friend validator_link make_fake_validator();

        typedef std::shared_ptr<bool const> valid_type;

        explicit        validator_link (valid_type const&)           ;
        explicit        validator_link (fake)                        ;

        valid_type valid_;
    };

}}
