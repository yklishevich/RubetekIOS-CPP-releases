#pragma once

#include <memory>

#include <msw/noncopyable.hpp>
#include <msw/zzz/validator/link/fwd.hpp>


namespace msw {
namespace zzz {

    struct validator_root
        : noncopyable
    {
        struct init {};

                        validator_root  ()                          ;
        explicit        validator_root  (init)                      ;
                        ~validator_root ()                          ;

                        validator_root  (validator_root&&)          ;
        validator_root& operator =      (validator_root&&)          ;

        void            change          ()                          ;

        validator_link  make_link       ()                  const   ;

        void            swap            (validator_root&)           ;

    private:
        std::shared_ptr<bool> valid_;
    };

}}
