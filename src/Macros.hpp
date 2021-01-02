#pragma once


#if (ARK_COMPILER == ARK_COMPILER_CLANG)
    #define ARK_FORCE_INLINE    inline
    #define ARK_FORCE_NO_INLINE

    #define ARK_OPTIMIZE_OFF _Pragma("clang optimize off")
    #define ARK_OPTIMIZE_ON  _Pragma("clang optimize on")
#else
    #define ARK_FORCE_INLINE    __forceinline
    #define ARK_FORCE_NO_INLINE _declspec(noinline)


    #define ARK_OPTIMIZE_OFF __pragma(optimize("", off))
    #define ARK_OPTIMIZE_ON  __pragma(optimize("", on ))

#endif // if (ARK_COMPILER == ARK_COMPILER_CLANG)
