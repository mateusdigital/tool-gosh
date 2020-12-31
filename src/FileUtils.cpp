// Header
#include "FileUtils.hpp"
//
#include <fstream>
//
#include "PathUtils.hpp"

// Usings
using namespace eaz;

//
// Read File
//
FileUtils::ReadAllLinesResult_t 
FileUtils::ReadAllLines(String const &path)
{
    if(!PathUtils::IsFile(path)) {
        return ReadAllLinesResult_t::Fail({ ErrorCodes::FILE_NOT_FOUND, path });
    }

    // @todo(stdmatt): Better error handling - 20 Dec, 2020...
    std::ifstream file(path.CStr());

    Array<String> lines; // @todo(stdmatt): Try to preallocate some memory... 20, Dec 2020.
    while(true) {
        std::string s;
        std::getline(file, s);
        lines.PushBack(s);
    }

    return ReadAllLinesResult_t::Success(lines);
}

//
// Write File
//
FileUtils::WriteResult_t 
FileUtils::WriteAllLines(
    String        const &path,
    Array<String> const &lines,
    String        const &new_line,
    WriteMode     const  write_mode)
{
    return WriteResult_t::Success(0);
}
