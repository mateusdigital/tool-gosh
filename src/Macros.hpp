#pragma once


#if (EAZ_COMPILER == EAZ_COMPILER_CLANG)
    #define EAZ_FORCE_INLINE    inline
    #define EAZ_FORCE_NO_INLINE

    #define EAZ_OPTIMIZE_OFF _Pragma("clang optimize off") 
    #define EAZ_OPTIMIZE_ON  _Pragma("clang optimize on")
#else
    #define EAZ_FORCE_INLINE    __forceinline
    #define EAZ_FORCE_NO_INLINE _declspec(noinline)


    #define EAZ_OPTIMIZE_OFF __pragma(optimize("", off))
    #define EAZ_OPTIMIZE_ON  __pragma(optimize("", on ))

#endif // if (EAZ_COMPILER == EAZ_COMPILER_CLANG)
