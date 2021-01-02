#pragma once

//
#include "BasicTypes.hpp"
#include "Array.hpp"
#include "String.hpp"
#include "Result.hpp"

namespace ark { namespace FileUtils {

//
//
//
namespace ErrorCodes {
    constexpr i32 FILE_NOT_FOUND      = 1;
    constexpr i32 FILE_ALREADY_EXISTS = 2;
    constexpr i32 INVALID_FILENAME    = 3;
    constexpr i32 INVALID_PATH        = 4;
} // namespace ErrorCodes


//
// Read File 
//
typedef Result<Array<String>> ReadAllLinesResult_t;
ReadAllLinesResult_t ReadAllLines(String const &path);


//
// Write File
//
enum class WriteMode 
{
    Append,
    Overwrite,
};

typedef Result<size_t> WriteResult_t;
WriteResult_t WriteAllLines(
    String        const &path,
    Array<String> const &lines,
    String        const &new_line,
    WriteMode     const  write_mode);


} // namespace FileUtils
} // namespace ark
