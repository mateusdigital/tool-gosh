#pragma once

// std
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <numeric>
// Arkadia
#include <limits>

#include "Assert.hpp"

//
// Numeric Types
//
// uint
typedef uint8_t   u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
// int
typedef int8_t   i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;
// float
typedef float  f32;
typedef double f64;

template <typename T>
struct NumericLimits
    : public std::numeric_limits<T>
{
    static constexpr T Max = std::numeric_limits<T>::max();
};
//
//
//
namespace ark {

template <typename T>
struct MinMax
{
    T min;
    T max;

    constexpr MinMax(T min, T max)
        : min(min)
        , max(max)
    {
        // @todo(stdmatt): Format string is assuming ints... Jan 02, 2021
        ARK_ASSERT(min <= max, "min needs to <= max - min(%d), max(%d)", min, max);
    }
}; // struct MinMax

} // namespace ark
