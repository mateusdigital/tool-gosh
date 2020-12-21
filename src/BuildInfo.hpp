#pragma once
//
#include "BasicTypes.hpp"


namespace BuildInfo {

//
// Version
//
char const * const VersionString();
size_t             VersionMajor ();
size_t             VersionMinor ();
size_t             VersionBaby  ();

//
// Assertions
//
bool               IsAssertEnabled();


}; // namespace BuildInfo
