#pragma once

#include "Discovery.hpp"
#include "BasicTypes.hpp"
#include "Macros.hpp"
#include "Assert.hpp"
#include "CodeUtils.hpp"


namespace ark {
class String;


template <typename ...Args>
ark_internal_function void
PrintLn(String const &fmt, Args... args)
{
    fmt::print(fmt.CStr(), args...);
    printf("\n");
}


} // namespace ark
