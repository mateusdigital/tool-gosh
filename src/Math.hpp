#pragma once

// Arkadia
#include "Macros.hpp"
namespace ark {

template<typename Type>
ARK_FORCE_INLINE constexpr const Type&
Clamp(Type const &_value, Type const &_min, Type const &_max)
{
    return _value < _min ? _min :
           _value > _max ? _max :
           _value;
}

} // namespace ark
