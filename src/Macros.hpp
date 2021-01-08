#pragma once
// Arkadia
#include "Discovery.hpp"

//
// Force Inline
//

#define ARK_INLINE   inline

#if (ARK_COMPILER_IS_CLANG || ARK_COMPILER_IS_GCC)
    #define ARK_FORCE_INLINE    inline
    #define ARK_FORCE_NO_INLINE
#elif (ARK_COMPILER_IS_MSVC)
    #define ARK_FORCE_INLINE    __forceinline
    #define ARK_FORCE_NO_INLINE _declspec(noinline)
#else
    #error "Compiler not supported...";
#endif

//
// Optimizations
//
#if (ARK_COMPILER_IS_CLANG || ARK_COMPILER_IS_GCC)
    #define ARK_OPTIMIZE_OFF _Pragma("clang optimize off")
    #define ARK_OPTIMIZE_ON  _Pragma("clang optimize on")
#elif (ARK_COMPILER_IS_MSVC)
    #define ARK_OPTIMIZE_OFF __pragma(optimize("", off))
    #define ARK_OPTIMIZE_ON  __pragma(optimize("", on ))
#else
    #error "Compiler not supported...";
#endif
