// Header
#include "PathUtils.hpp"
// @todo(stdmatt): Organize the windows headers... Dec 20, 2020
// Windows Headers
#include <Shlobj.h>
#include <shlwapi.h>
#undef CreateFile
#undef CreateDirectory
#pragma comment(lib, "Shlwapi.lib")
// Arkadia
#include "OS.hpp"
#include "FileUtils.hpp"
#include "Math.hpp"

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

String
PathUtils::GetCWD()
{
    return MakeAbsolute(".");
}

bool
PathUtils::IsAbs(String const& path)
{
    return PathIsRoot(path.CStr()) == TRUE;
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
PathUtils::MakeAbsolute(String const &path)
{
    // @todo(stdmatt): Don't hardcode the char-type.       - Jan 02, 2021
    // @todo(stdmatt): Don't hardcode the size of the path - Jan 02, 2021
    // @todo(stdmatt): Better error handling               - Jav 02, 2021

    char buffer[MAX_PATH] = {};
    DWORD result = GetFullPathName(path.CStr(), MAX_PATH, buffer, nullptr);
    ARK_ASSERT(result != 0, "Failed to GetFullPathName of: ({})", path);

    return String(buffer);
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
    String const canonized_path    = Canonize(path        );
    String const canonized_to_what = Canonize(to_what_path);
    Array<String> path_split    = canonized_path   .Split(OS::PathSeparatorChar());
    Array<String> to_what_split = canonized_to_what.Split(OS::PathSeparatorChar());

    if(path_split[0] != to_what_split[0]) { // Different drives?
        return canonized_path; // Can't go there relatively...
    }

    size_t right_most_index = 0;
    for(
        size_t i = 0,
        least_count = Math::Min(path_split.Count(),  to_what_split.Count());
        i < least_count;
        ++i)
    {
        if(path_split[i] != to_what_split[i]) {
            break;
        }
        right_most_index = i;
    }

    size_t const diff           = to_what_split.Count() - right_most_index;
    String const parent_dir_str = OS::PathParentDirectoryString() + OS::PathSeparatorString();
    String const relative_path  = String::Repeat(parent_dir_str, diff)
                                + String::Join(
                                    to_what_split.Begin() + right_most_index,
                                    to_what_split.End  (),
                                    OS::PathSeparatorChar()
                                );

    // @todo(stdmatt): Doing super adhoc now, but we should look how .net does... Dec 21, 2020
    return relative_path;
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
    String const abs_path = MakeAbsolute(path);
    Array<String> components = abs_path.Split({
        OS::PathSeparatorChar(), OS::PathAlternateSeparatorChar()
    });

    String final_path = String::CreateWithCapacity(abs_path.Length());
    for(String component : components) {
        if(case_options == CanonizeCaseOptions::ChangeToLowerCase) {
            component.ToLower();
        } else if(case_options == CanonizeCaseOptions::ChangeToUpperCase) {
            component.ToUpper();
        }
        char const slash_char =
            slash_options == CanonizeSlashOptions::ChangeToBackwardSlashes ? '\\' :
            slash_options == CanonizeSlashOptions::ChangeToForwardSlashes  ? '/'  :
            abs_path[final_path.Length()];

        final_path += component;
        final_path += slash_char;
    }
    // @todo(stdmatt): Super slopply way to ensure that we don't have the trailing
    // slash char on the path, find a better way to do it - Jan 02, 2021.
    if(final_path.Back() == OS::PathSeparatorChar()) {
        final_path.PopBack();
    }
    return final_path;
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
    String const clean_path = Canonize(path);
    size_t const last_index = clean_path.FindLastIndexOf(OS::PathSeparatorChar());
    String const basename   = clean_path.SubString(last_index + 1, clean_path.Length());

    return basename;
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
