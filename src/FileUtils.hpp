#pragma once

//
#include "BasicTypes.hpp"
#include "Array.hpp"
#include "String.hpp"
#include "Result.hpp"

namespace FileUtils {

namespace ErrorCodes {
    constexpr int FILE_NOT_FOUND   = 1;
    constexpr int INVALID_FILENAME = 2;
};


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



}; // namespace FileUtils.hpp
