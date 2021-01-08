#pragma once
// Arkadia
#include "Discovery.hpp"

#if (ARK_ASSERTION_IS_ENABLED)
    // @todo(stdmatt): Actually implement the assetions... Jan 06, 2021
    #define ARK_ASSERT(_expr_, _fmt_, ...)       do { } while(0)
    #define ARK_ASSERT_TRUE(_expr_, _fmt_, ...)  do { } while(0)
    #define ARK_ASSERT_FALSE(_expr_, _fmt_, ...) do { } while(0)
#else // (ARK_ASSERTION_IS_ENABLED)
    #define ARK_ASSERT(_expr_, _fmt_, ...)       do { } while(0)
    #define ARK_ASSERT_TRUE(_expr_, _fmt_, ...)  do { } while(0)
    #define ARK_ASSERT_FALSE(_expr_, _fmt_, ...) do { } while(0)
#endif // ARK_ENABLE_ASSERTIONS)
