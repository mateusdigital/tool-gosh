#pragma once

// Arkadia
#include "Discovery.hpp"


namespace ark { namespace DebugUtils {

bool IsDebuggerPresent();
void BreakInDebugger  ();

//
// ARK_DEBUG_BREAK()
// 
#if (ARK_IS_DEBUG_BUILD)
    #define ARK_DEBUG_BREAK()                    \
        do {                                     \
            ark::DebugUtils::BreakInDebugger();  \
        } while(0)
#else // !(ARK_IS_DEBUG_BUILD)
    #define ARK_DEBUG_BREAK()   \
        do {                    \
            /* Empty */         \
        } while(0)
#endif // #if defined(ARK_BUILD_DEBUG)


//
// ARK_ONLY_IN_DEBUG({ ... })
//
#if (ARK_IS_DEBUG_BUILD)
    #define ARK_ONLY_IN_DEBUG(_code_) do { _code_ } while(0)
#else // !(ARK_IS_DEBUG_BUILD)
    #define ARK_ONLY_IN_DEBUG(_code_) do { /* Empty */ } while(0)
#endif // #if defined(ARK_BUILD_DEBUG)

} // namespace DebugUtils
} // namespace ark
