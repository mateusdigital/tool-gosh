#pragma once

//
#include "BasicTypes.hpp"
#include "String.hpp"

namespace OS {

// @todo(stdmatt): Make this constants and inline... Dec 21, 2020
inline String NewLineString      () { return "\n"; }


// @todo(stdmatt): Make it for GNU and macOS - Dec 20, 2020
inline String PathSeparatorString         () { return "/"; }
inline String PathAlternateSeparatorString() { return "/"; }

inline char   PathSeparatorChar           () { return '/'; }
inline char   PathAlternateSeparatorChar  () { return '/'; }

inline String PathCurrentDirectoryString() { return ".";  }
inline String PathParentDirectoryString () { return ".."; }

inline bool 
IsCharAPathSeperator(char const c) 
{
    return PathSeparatorChar         () == c
        || PathAlternateSeparatorChar() == c;
}

}; // namespace OS
