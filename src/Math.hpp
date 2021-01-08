#pragma once

// Arkadia
#include "Basic.hpp"

namespace ark { namespace Math {

// @todo(stdmatt): If is numeric is best to not pass by reference... Jan 02, 2021
template<typename Type>
ARK_FORCE_INLINE constexpr const Type&
Clamp(Type const &_value, Type const &_min, Type const &_max)
{
    return _value < _min ? _min :
           _value > _max ? _max :
           _value;
}

// @todo(stdmatt): If is numeric is best to not pass by reference... Jan 02, 2021
template<typename Type>
ARK_FORCE_INLINE constexpr const Type&
Min(Type const &a, Type const &b)
{
    return (a < b) ? a : b;
}

} // namespace Math
} // namespace ark
