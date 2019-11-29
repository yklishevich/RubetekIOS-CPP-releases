#pragma once

#include <msw/plain_types.hpp> // uint64


namespace msw
{
    template <typename Val = uint64>
    class odometer
    {
    public:
        typedef Val value_type;

                   odometer ()                       ;

        void       add      (value_type value)       ;
        void       sub      (value_type value)       ;
        void       set      (value_type value)       ;
        void       reset    ()                       ;
        bool       empty    ()                 const ;
        value_type value    ()                 const ;

    private:
        value_type value_;
    };
}


namespace msw
{
    template <typename Val>
    odometer<Val>::odometer()
        : value_(0)
    {}

    template <typename Val>
    void odometer<Val>::add(value_type value)
    {
        value_ += value;
    }
    template <typename Val>
    void odometer<Val>::sub(value_type value)
    {
        value_ -= value;
    }
    template <typename Val>
    void odometer<Val>::set(value_type value)
    {
        value_ = value;
    }
    template <typename Val>
    void odometer<Val>::reset()
    {
        value_ = 0;
    }
    template <typename Val>
    bool odometer<Val>::empty() const
    {
        return value_ == 0;
    }
    template <typename Val>
    typename odometer<Val>::value_type odometer<Val>::value() const
    {
        return value_;
    }
}
