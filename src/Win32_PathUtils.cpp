// Header
#include "PathUtils.hpp"
// @todo(stdmatt): Organize the windows headers... Dec 20, 2020
// Windows Headers
#include <Shlobj.h>
#undef CreateFile
#undef CreateDirectory
// Arkadia
#include "OS.hpp"
#include "FileUtils.hpp"

// Usings
using namespace ark;

namespace Private {
} // namespace Private


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
    String result = a;
    if(!b.IsEmpty()) { 
        String const &path_separator = OS::PathSeparatorString();
        if(a.IsEmpty()) {
            result = b;
        } else {
            result += path_separator + b;
        }

        if(!c.IsEmpty()) {
            result += path_separator + c;
        }
    }
    
    return result;
}


String 
PathUtils::Canonize(
    String               const &path,
    CanonizeCaseOptions  const case_options  /* = CanonizeCaseOptions::DoNotChange  */,
    CanonizeSlashOptions const slash_options /* = CanonizeSlashOptions::ChangeToForwardSlashes */)
{
    Array<String> components = path.Split({
        OS::PathSeparatorChar(), OS::PathAlternateSeparatorChar()
    });

    String final_path = String::CreateWithCapacity(path.Length());
    for(String component : components) {
        if(case_options == CanonizeCaseOptions::ChangeToLowerCase) {
            component.ToLower();
        } else if(case_options == CanonizeCaseOptions::ChangeToUpperCase) {
            component.ToUpper();
        }
        char const slash_char =
            slash_options == CanonizeSlashOptions::ChangeToBackwardSlashes ? '\\' :
            slash_options == CanonizeSlashOptions::ChangeToForwardSlashes  ? '/'  :
            path[final_path.Length()];

        final_path += component;
        final_path += slash_char;

    }

    return final_path;
}

String 
PathUtils::MakeRelative(
    String const &path, 
    String const &to_what_path)
{
    if(path.IsEmpty() || to_what_path.IsEmpty()) {
        return "";
    }

    // @todo(stdmatt): Creating too many copies... Dec 21, 2020
    String path_canonized    = Canonize(to_what_path);
    String to_what_canonized = Canonize(to_what_path);
    
    // @design(stdmatt): Doing super adhoc now, but we should look how .net does... Dec 21, 2020
    return path;
}


bool 
PathUtils::IsFile(String const &path)
{
    // @todo(stdmatt): Check if this is correct.. Dec 21, 2020
    DWORD const file_attributes = GetFileAttributesA(path.CStr());
    bool  const result          = (file_attributes != INVALID_FILE_ATTRIBUTES
                              &&  !(file_attributes & FILE_ATTRIBUTE_DIRECTORY));
    return result;
}

bool 
PathUtils::IsDir(String const &path)
{
    // @todo(stdmatt): Check if this is correct.. Dec 21, 2020

    DWORD const file_attributes = GetFileAttributesA(path.CStr());
    bool  const result          = (file_attributes != INVALID_FILE_ATTRIBUTES
                              &&  (file_attributes & FILE_ATTRIBUTE_DIRECTORY));
    return result;
}

String 
PathUtils::Dirname(String const &path)
{
    // c:/some/nice/dir  -> c:/some/nice
    // c:/some/nice/dir/ -> c:/some/nice/dir

    // @todo(stdmatt): Handle the OS::PathAlternateChar - Dec 21, 2020
    size_t const last_index = path.FindLastIndexOf(OS::PathSeparatorChar());
    return path.SubString(0, last_index);
}

String 
PathUtils::Basename(String const &path)
{
    // c:/some/nice/dir  -> c:/some/nice
    // c:/some/nice/dir/ -> c:/some/nice/dir

    // @todo(stdmatt): Handle the OS::PathAlternateChar - Dec 21, 2020
    size_t const last_index = path.FindLastIndexOf(OS::PathSeparatorChar());
    return path.SubString(last_index, path.Length());
}


//
// Create Dir
//
PathUtils::CreateDirResult_t 
PathUtils::CreateDir(
    String           const &path, 
    CreateDirOptions const options)
{
    //
    // Non Recursive
    if(options == CreateDirOptions::NonRecursive) {
        BOOL const result = CreateDirectoryA(path.c_str(), nullptr);
        if(result) {
            return CreateDirResult_t::Success(true);
        }

        DWORD const last_error = GetLastError();
        if(last_error == ERROR_ALREADY_EXISTS) {
            return CreateDirResult_t::Fail({ FileUtils::ErrorCodes::FILE_ALREADY_EXISTS, path});
        } else if(last_error == ERROR_PATH_NOT_FOUND) {
            return CreateDirResult_t::Fail({ FileUtils::ErrorCodes::INVALID_PATH, path});
        }
        return CreateDirResult_t::Fail({ Error::UNKNOWN_ERROR, path});
    }

    // Recursive
    String const  canonized_path  = PathUtils::Canonize(path);
    Array<String> path_components = canonized_path.Split(OS::PathSeparatorChar());

    String curr_path;
    for(String const &component : path_components) {
        curr_path = PathUtils::Join(curr_path, component);
        bool const is_dir = PathUtils::IsDir(curr_path);

        printf("%s\n", curr_path.CStr());
        fflush(stdout);
        if(!is_dir) {
            CreateDirResult_t const result = PathUtils::CreateDir(curr_path, CreateDirOptions::NonRecursive);
            if(result.HasFailed()) {
                return result;
            }
        }
    }

    return CreateDirResult_t::Success(true);
}

//
// Create File
//
PathUtils::CreateFileResult_t 
PathUtils::CreateFile(
    String            const &filename, 
    CreateFileOptions const options)
{
    bool const file_exists = PathUtils::IsFile(filename);
    if (file_exists && options == CreateFileOptions::ErrorIfExists) {
        return CreateFileResult_t::Fail({ FileUtils::ErrorCodes::FILE_ALREADY_EXISTS, filename });
    }

    // @todo(stdmatt): Check if the filename is a valid filename - Dec 21, 2020...
    DWORD open_mode = TRUNCATE_EXISTING;
    if (options == CreateFileOptions::ForceCreation) {
        open_mode = TRUNCATE_EXISTING;
    }
    else if(options == CreateFileOptions::IgnoreIfExists) {
        open_mode = CREATE_NEW;
    }

    HANDLE const win32_handle = CreateFileA(
        filename.c_str(),
        GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_DELETE | FILE_SHARE_READ | FILE_SHARE_WRITE,
        nullptr,
        open_mode,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    // @todo(stdmatt): Check for errors.... Dec 21, 2020
    CloseHandle(win32_handle);

    return CreateFileResult_t::Success(true);
}
