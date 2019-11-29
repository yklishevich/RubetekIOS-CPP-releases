#ifndef SVL_DETAIL_CONFIG_HPP
#define SVL_DETAIL_CONFIG_HPP


#define SVL_VER 100

// для использования собственных функций по преобразованию UNICODE <-> ANSI необходимо определить макрос
// #define SVL_USER_STRING_CONVERSION 1

// для использования дополнительной функциональности библиотеки (при поддержке со стороны библиотеки boost)
// необходимо определить макрос #define SVL_USE_BOOST 1

// для использования ANSI версии библиотеки необходимо определить макрос #define SVL_UNICODE 0

#ifndef SVL_UNICODE
  #define SVL_UNICODE 1
#endif

#ifndef SVL_USER_STRING_CONVERSION
  #define SVL_USER_STRING_CONVERSION 0
#endif

// SVL_DEBUG_LEVEL: 0(отсутствует), 1(базовая), 2(расширенная)
#ifndef SVL_DEBUG_LEVEL
  #if defined(_DEBUG) || defined(DEBUG)
    #define SVL_DEBUG_LEVEL 1
  #else
    #define SVL_DEBUG_LEVEL 0
  #endif
#endif

#ifndef SVL_CPP0X
  #if 1600 <= _MSC_VER
    #define SVL_CPP0X 1
  #else
    #define SVL_CPP0X 0
  #endif
#else
  #define SVL_CPP0X 0
#endif

/*----------------------------------------------------------------------*/

#ifndef SVL_USE_BOOST
  #define SVL_USE_BOOST 0
#endif

#ifndef SVL_USE_BOOST_FORMAT
  #define SVL_USE_BOOST_FORMAT SVL_USE_BOOST
#endif

#ifndef SVL_USE_BOOST_LEXICAL_CAST
  #define SVL_USE_BOOST_LEXICAL_CAST SVL_USE_BOOST
#endif

#ifndef SVL_USE_BOOST_NONCOPYABLE
  #define SVL_USE_BOOST_NONCOPYABLE SVL_USE_BOOST
#endif

#ifndef SVL_USE_BOOST_RANGE
  #define SVL_USE_BOOST_RANGE SVL_USE_BOOST
#endif

#ifndef SVL_USE_BOOST_TRIBOOL
  #define SVL_USE_BOOST_TRIBOOL SVL_USE_BOOST
#endif

/*----------------------------------------------------------------------*/

#ifndef WINVER
  #define WINVER 0x0501
#endif

#ifndef _WIN32_IE
  #define _WIN32_IE 0x0500
#endif

#include <windows.h>
#include <commctrl.h>
#include <shlobj.h>

#ifndef PBM_SETMARQUEE
  #define PBM_SETMARQUEE (WM_USER+10)
#endif

#ifndef PBS_MARQUEE
  #define PBS_MARQUEE 0x08
#endif

/*----------------------------------------------------------------------*/

#include <map>
#include <vector>
#include <list>
#include <string>
#include <stdexcept>
#include <memory>
#include <algorithm>
#include <functional>
#include <cstddef>

#ifdef _MSC_VER
namespace std
{
	typedef ::uintptr_t uintptr_t;
}
#endif

#if SVL_DEBUG_LEVEL != 0
  #define SVL_TO_STRING2(a) #a
  #define SVL_TO_STRING(a) SVL_TO_STRING2(a)
  #define SVL_CAST(type, object) dynamic_cast<type>(object)
  #define SVL_CAST_TYPE(object) protected: virtual ~object() {}
  #define SVL_VERIFY(expr, cond) if (!(expr cond)) { ::MessageBoxA(0, "file: " __FILE__ "\nline: " SVL_TO_STRING(__LINE__), "SVL_VERIFY", MB_ICONERROR | MB_OK); ::DebugBreak(); }
  #define SVL_ASSERT(expr) if (!(expr)) { ::MessageBoxA(0, "file: " __FILE__ "\nline: " SVL_TO_STRING(__LINE__) "\ncondition: " #expr, "SVL_ASSERT", MB_ICONERROR | MB_OK); ::DebugBreak(); }
  #define SVL_ASSERT_FALSE() { ::MessageBoxA(0, "file: " __FILE__ "\nline: " SVL_TO_STRING(__LINE__), "SVL_ASSERT", MB_ICONERROR | MB_OK); ::DebugBreak(); }
  #define SVL_IN_DEBUG(expr) expr
#else
  #define SVL_CAST(type, object) static_cast<type>(object)
  #define SVL_CAST_TYPE(object)
  #define SVL_VERIFY(expr, cond) expr
  #define SVL_ASSERT(expr)
  #define SVL_ASSERT_FALSE()
  #define SVL_IN_DEBUG(expr)
#endif

#if SVL_DEBUG_LEVEL == 2
  #define SVL_IN_DEBUG_2(expr) expr
  #define SVL_ASSERT_2(expr) SVL_ASSERT(expr)
  #define SVL_ASSERT_FALSE_2() SVL_ASSERT_FALSE()
#else
  #define SVL_IN_DEBUG_2(expr)
  #define SVL_ASSERT_2(expr)
  #define SVL_ASSERT_FALSE_2()
#endif


#if SVL_UNICODE
  #define SVL_MS(x) x ## W
  #define SVL_T(x)  L ## x
#else
  #define SVL_MS(x) x ## A
  #define SVL_T(x)  x
#endif


#if SVL_CPP0X
  #include <array>
  #define SVL_ARRAY(type, size, name) std::tr1::array<type, size> name
  #define SVL_A_O {{
  #define SVL_A_C }}
  #define SVL_STATIC_ASSERT(cond, name) static_assert(cond, name)
  #define SVL_SHARED_PTR std::tr1::shared_ptr
#elif SVL_USE_BOOST
  #include <boost/array.hpp>
  #define SVL_ARRAY(type, size, name) boost::array<type, size> name
  #define SVL_A_O {{
  #define SVL_A_C }}
  #include <boost/static_assert.hpp>
  #define SVL_STATIC_ASSERT(cond, name) BOOST_STATIC_ASSERT(cond)
  #include <boost/shared_ptr.hpp>
  #define SVL_SHARED_PTR boost::shared_ptr
#else
  #define SVL_ARRAY(type, size, name) type name[size]
  #define SVL_A_O {
  #define SVL_A_C }
  namespace svl_assertion
  {
    template <bool condition> struct failure;
    template <> struct failure<false>;
    template <> struct failure<true> {};
    template <std::size_t> struct inspector;
  }
  #define SVL_STATIC_ASSERT(cond, name) typedef svl_assertion::inspector<sizeof(svl_assertion::failure<(cond)>)> z_static_assert_
  #include <svl/detail/shared_ptr.hpp>
  #define SVL_SHARED_PTR svl::shared_ptr
#endif

#if SVL_USE_BOOST_NONCOPYABLE
  #include <boost/noncopyable.hpp>
#endif

#if SVL_USE_BOOST_LEXICAL_CAST
  #include <boost/lexical_cast.hpp>
#endif

#if SVL_USE_BOOST_TRIBOOL
  #include <boost/logic/tribool.hpp>
#endif

#if SVL_USE_BOOST_FORMAT
  #include <boost/format.hpp>
#endif

#if SVL_USE_BOOST_RANGE
  #include <boost/range.hpp>
#endif

#ifdef _MSC_VER
  #if defined _M_IX86
    #pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
  #elif defined _M_IA64
    #pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
  #elif defined _M_X64
    #pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
  #else
    #pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
  #endif
#endif

#endif // SVL_DETAIL_CONFIG_HPP
