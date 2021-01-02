#pragma once
// std
#include <algorithm>
// Arkadia
#include "CodeUtils.hpp"
#include "Macros.hpp"

namespace ark { namespace Algo {

template <typename Dst_Type, typename Src_Type>
ARK_FORCE_INLINE void
Append(Dst_Type &dst, Src_Type const &src)
{
    dst.Reserve(dst.Count() + src.Count());
    std::copy(src.begin(), src.end(), std::back_inserter(dst));
}

} // namespace Algo
} // namespace ark
