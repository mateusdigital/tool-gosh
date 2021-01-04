// Header
#include "FileUtils.hpp"
//
#include <fstream>
//
#include "PathUtils.hpp"

// Usings
using namespace ark;

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
    while(!file.eof()) {
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
    // @todo(stdmatt): Better error handling - 02 Dec, 2021...
    std::ofstream file;

    if(write_mode == WriteMode::Append) {
       file.open(path, std::ofstream::out | std::ofstream::app);
    } else if(write_mode == WriteMode::Overwrite) {
       file.open(path, std::ofstream::out | std::ofstream::trunc);
    }

    for(String const &line : lines) {
        file << line.CStr();
        file << new_line;
    }
    return WriteResult_t::Success(0);
}
