#pragma once


namespace rubetek {
namespace module  {

    class noncopyable
    {
    public:
        noncopyable() {}
    private:
        noncopyable(noncopyable const&);
        noncopyable& operator = (noncopyable const&);
    };

}}
