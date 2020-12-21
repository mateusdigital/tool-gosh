// Header
#include "PathUtils.hpp"
// @todo(stdmatt): Organize the windows headers... Dec 20, 2020
// Windows Headers
#include <Shlobj.h>

#undef CreateFile



String 
PathUtils::GetUserHome() 
{
    // @todo(stdmatt): Don't hardcode the char-type.        Dec 20, 2020
    // @todo(stdmatt): Don't hardcode the size of the path. Dec 20, 2020
    char path[MAX_PATH];
    HRESULT result = SHGetFolderPathA(nullptr, CSIDL_PROFILE, nullptr, 0, path);
    
    if(!SUCCEEDED(result)) { // @todo(stdmatt): Handle error... Dec 20, 2020
    }

    return String(path);
}

// @todo(stdmatt): Make it a variadic template... Dec 20, 2020.
String 
PathUtils::Join(
    String const &a,
    String const &b,
    String const &c)
{
    
}


String 
PathUtils::Canonize(String const &path)
{

}

String 
PathUtils::MakeRelative(String const &path, String const &to_what_path)
{

}


bool 
PathUtils::IsFile(String const &path)
{

}

bool 
PathUtils::IsDir(String const &path)
{

}

String 
PathUtils::Dirname(String const &path)
{

}

String 
PathUtils::Basename(String const &path)
{

}


//
// Create Dir
//
PathUtils::CreateDirResult_t 
PathUtils::CreateDir(
    String           const &path, 
    CreateDirOptions const options)
{

}

//
// Create File
//
PathUtils::CreateFileResult_t 
PathUtils::CreateFile(
    String            const &filename, 
    CreateFileOptions const options)
{

}
