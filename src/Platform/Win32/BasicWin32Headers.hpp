#pragma once
// Arkadia
#include "Basic.hpp"

#if (ARK_CURRENT_OS == ARK_OS_WINDOWS)
// Headers
#include <Shlobj.h>
#include <shlwapi.h>
// Libs
#pragma comment(lib, "Shlwapi.lib")

//
// Undefine windows shit...
//
#undef CreateFile
#undef CreateDirectory

#endif // (ARK_CURRENT_OS == ARK_OS_WINDOWS)
