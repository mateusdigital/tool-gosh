#pragma once


namespace DebugUtils {

bool IsDebuggerPresent();
void BreakInDebugger  ();

//
// EAZ_DEBUG_BREAK()
// 
#if defined(EAZ_BUILD_DEBUG)
    #define EAZ_DEBUG_BREAK()   \
        do {                    \
            BreakInDebugger();  \
        } while(0)
#else
    #define EAZ_DEBUG_BREAK()   \
        do {                    \
            /* Empty */         \
        } while(0)
#endif // #if defined(EAZ_BUILD_DEBUG)


//
// EAZ_ONLY_IN_DEBUG({ ... })
//
#if defined(EAZ_BUILD_DEBUG)
    #define EAZ_ONLY_IN_DEBUG(_code_) do { _code_ } while(0)
#else
    #define EAZ_ONLY_IN_DEBUG(_code_) do { /* Empty */ } while(0)
#endif // #if defined(EAZ_BUILD_DEBUG)

} // namespace DebugUtils