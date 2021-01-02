#pragma once


namespace DebugUtils {

bool IsDebuggerPresent();
void BreakInDebugger  ();

//
// ARK_DEBUG_BREAK()
// 
#if defined(ARK_BUILD_DEBUG)
    #define ARK_DEBUG_BREAK()   \
        do {                    \
            BreakInDebugger();  \
        } while(0)
#else
    #define ARK_DEBUG_BREAK()   \
        do {                    \
            /* Empty */         \
        } while(0)
#endif // #if defined(ARK_BUILD_DEBUG)


//
// ARK_ONLY_IN_DEBUG({ ... })
//
#if defined(ARK_BUILD_DEBUG)
    #define ARK_ONLY_IN_DEBUG(_code_) do { _code_ } while(0)
#else
    #define ARK_ONLY_IN_DEBUG(_code_) do { /* Empty */ } while(0)
#endif // #if defined(ARK_BUILD_DEBUG)

} // namespace DebugUtils
