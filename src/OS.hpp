#pragma once

// Arkadia
#include "BasicTypes.hpp"
#include "String.hpp"

namespace ark {
namespace OS {

// @todo(stdmatt): Make this constants and ARK_FORCE_INLINE... Dec 21, 2020
ARK_FORCE_INLINE String NewLineString      () { return "\n"; }


// @todo(stdmatt): Make it for GNU and macOS - Dec 20, 2020
ARK_FORCE_INLINE String PathSeparatorString         () { return "/"; }
ARK_FORCE_INLINE String PathAlternateSeparatorString() { return "\\"; }

ARK_FORCE_INLINE char   PathSeparatorChar           () { return '/'; }
ARK_FORCE_INLINE char   PathAlternateSeparatorChar  () { return '\\'; }

ARK_FORCE_INLINE String PathCurrentDirectoryString() { return ".";  }
ARK_FORCE_INLINE String PathParentDirectoryString () { return ".."; }

ARK_FORCE_INLINE bool
IsCharAPathSeperator(char const c) 
{
    return PathSeparatorChar         () == c
        || PathAlternateSeparatorChar() == c;
}

} // namespace OS
} // namespace ark
