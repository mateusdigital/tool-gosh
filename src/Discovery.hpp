#pragma once

//
// Assertions
//
#define ARK_DISABLE_ASSERTIONS 0
#define ARK_ENABLE_ASSERTIONS  1

#define ARK_ASSERTION_MODE ARK_DISABLE_ASSERTIONS // @todo(stdmatt): This should come from the build system - Jan 06, 2021
#if (ARK_ASSERTION_MODE == ARK_ENABLE_ASSERTIONS)
    #define ARK_ASSERTION_IS_ENABLED true
#endif // (ARK_ASSERTION_MODE == ARK_ENABLE_ASSERTIONS)

//
// Build Mode
//
#define ARK_BUILD_MODE_DEBUG   1
#define ARK_BUILD_MODE_RELEASE 2

#define ARK_CURRENT_BUILD_MODE ARK_BUILD_MODE_DEBUG
#if (ARK_CURRENT_BUILD_MODE == ARK_BUILD_MODE_DEBUG)
    #define ARK_IS_DEBUG_BUILD   true
    #define ARK_IS_RELEASE_BUILD false
#else
    #define ARK_IS_DEBUG_BUILD   false
    #define ARK_IS_RELEASE_BUILD true
#endif // (ARK_ASSERTION_MODE == ARK_ENABLE_ASSERTIONS)

//
// OS
//
#define ARK_OS_INVALID          0
#define ARK_OS_WINDOWS          1
#define ARK_OS_GNU_LINUX        2
#define ARK_OS_BSD              3
#define ARK_OS_MAC              4
#define ARK_OS_UNIX             5

#if defined( _WIN32 )
    #define ARK_CURRENT_OS    ARK_OS_WINDOWS
    #define ARK_OS_IS_WINDOWS true
#else
    #error "OS not supported...";
#endif // Windows


//
// Compiler
//
#define ARK_COMPILER_INVALID 0
#define ARK_COMPILER_GCC     1
#define ARK_COMPILER_CLANG   2
#define ARK_COMPILER_MSVC    3

#if defined(__clang__)
    #define ARK_CURRENT_COMPILER  ARK_COMPILER_CLANG
    #define ARK_COMPILER_IS_CLANG true
#elif defined(__GNUC__)
    #define ARK_CURRENT_COMPILER ARK_COMPILER_GCC
    #define ARK_COMPILER_IS_GCC  true
#elif defined(_MSC_VER)
    #define ARK_CURRENT_COMPILER ARK_COMPILER_MSVC
    #define ARK_COMPILER_IS_MSVC true
#else
    #error "Compiler not supported...";
#endif
