#ifndef STREAMUTIL_H
#define STREAMUTIL_H


#if defined(Q_OS_WIN32)
#include <windows.h>
#elif defined(Q_OS_LINUX)
#endif

#include <cstdint>

namespace PH {
using Int8    = std::int8_t;
using UInt8   = std::uint8_t;
using Int16   = std::int16_t;
using UInt16  = std::uint16_t;
using Int32   = std::int32_t;
using UInt32  = std::uint32_t;
using Int64   = std::int64_t;
using UInt64  = std::uint64_t;
using IntPtr  = std::intptr_t;
using UIntPtr = std::uintptr_t;



#if defined(_MSC_VER)
    #define PH_HAVE_INT64 1
#elif defined(__GNUC__) || defined(__clang__)
    #define PH_HAVE_INT64 1
#elif defined(__SUNPRO_CC)
    #define PH_HAVE_INT64 1
#elif defined(__IBMCPP__)
    #define PH_HAVE_INT64 1
#elif defined(_DIAB_TOOL)
    #define PH_HAVE_INT64 1
#endif

#if !defined(PH_IOS_INIT_HACK)
#	if defined(_MSC_VER) && (!defined(_STLP_MSVC) || defined(_STLP_NO_OWN_IOSTREAMS))
#		define PH_IOS_INIT_HACK 1
    // QNX with Dinkumware but not GNU C++ Library
#	elif defined(__QNX__) && !defined(__GLIBCPP__)
#		define PH_IOS_INIT_HACK 1
    // Linux with Clang libc++
#   elif defined(__linux) && defined(_LIBCPP_VERSION)
#		define PH_IOS_INIT_HACK 1
#	endif
#endif

#if defined(PH_IOS_INIT_HACK)
#	define ph_ios_init(buf)
#else
#	define ph_ios_init(buf) init(buf)
#endif

}

#endif // STREAMUTIL_H
