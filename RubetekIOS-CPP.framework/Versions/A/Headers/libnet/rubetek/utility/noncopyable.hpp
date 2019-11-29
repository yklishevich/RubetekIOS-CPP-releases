#pragma once


namespace rubetek {
namespace utility {

	class noncopyable
	{
	public:
		noncopyable() {}
	private:
		noncopyable(noncopyable const&);
		noncopyable& operator = (noncopyable const&);
	};

}}

