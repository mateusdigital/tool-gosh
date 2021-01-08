#pragma once

// Arkadia
#include "BasicTypes.hpp"
#include "Discovery.hpp"
#include "String.hpp"


namespace ark { namespace OS {

#if (ARK_CURRENT_OS == ARK_OS_WINDOWS)
    ARK_FORCE_INLINE char           NewLineChar                 () { return '\n'; }
    ARK_FORCE_INLINE char           PathSeparatorChar           () { return '/';  }
    ARK_FORCE_INLINE char           PathAlternateSeparatorChar  () { return '\\'; }
#else // (ARK_CURRENT_OS == ARK_OS_WINDOWS)
    ARK_FORCE_INLINE char           NewLineChar                 () { return '\n'; }
    ARK_FORCE_INLINE char           PathSeparatorChar           () { return '/';  }
    ARK_FORCE_INLINE char           PathAlternateSeparatorChar  () { return '/';  }
#endif // (ARK_CURRENT_OS == ARK_OS_WINDOWS)

ARK_FORCE_INLINE String const & NewLineString               () { ark_local_persist String s_str(NewLineChar               ()); return s_str; }
ARK_FORCE_INLINE String const & PathSeparatorString         () { ark_local_persist String s_str(PathSeparatorChar         ()); return s_str; }
ARK_FORCE_INLINE String const & PathAlternateSeparatorString() { ark_local_persist String s_str(PathAlternateSeparatorChar()); return s_str; }

ARK_FORCE_INLINE String const & PathCurrentDirectoryString() { ark_local_persist String s_str(".");  return s_str; }
ARK_FORCE_INLINE String const & PathParentDirectoryString () { ark_local_persist String s_str(".."); return s_str; }

ARK_FORCE_INLINE bool
IsCharAPathSeparator(char const c)
{
    return PathSeparatorChar         () == c
        || PathAlternateSeparatorChar() == c;
}

} // namespace OS
} // namespace ark
