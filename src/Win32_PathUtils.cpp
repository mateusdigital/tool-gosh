// Header
#include "PathUtils.hpp"
// @todo(stdmatt): Organize the windows headers... Dec 20, 2020
// Windows Headers
#include <Shlobj.h>
#undef CreateFile
// 
#include "OS.hpp"
#include "FileUtils.hpp"
namespace Private {
}



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
        result += b;

        if(!c.IsEmpty()) {
            result += c;
        }
    }
    
    return result;
}


String 
PathUtils::Canonize(
    String               const &path,
    CanonizeCaseOptions  const case_options  /* = CanonizeCaseOptions::DoNotChange  */,
    CanonizeSlashOptions const slash_options /* = CanonizeSlashOptions::DoNotChange */)
{
    Array<String> components;
    Array<char>   separators;

    size_t left_index               = 0;
    size_t right_index              = 0;
    size_t components_string_length = 0;

    size_t const path_length = path.length();
    for (size_t left_index = 0, right_index = 0; 
        right_index < path_length; 
        ++right_index) 
    {
        char const c = path[right_index];
        if (!OS::IsCharAPathSeperator(c)) {
            continue;
        }

        String component = path.SubString(left_index, right_index);
        char const separator = (slash_options == CanonizeSlashOptions::ChangeToForwardSlashes ) ? '/' :
                               (slash_options == CanonizeSlashOptions::ChangeToBackwardSlashes) ? '\\' :
                               path[right_index];

        if (component.IsEmpty()) {
            continue;
        }
    
        left_index = (right_index + 1);
        
        //
        if (component == OS::PathCurrentDirectoryString()) {
            continue;
        } else if (component == OS::PathParentDirectoryString()) {
            // @todo(stdmatt): What to do when components are empty?? Dec 21, 2020
            if (!components.IsEmpty()) {
                components.PopBack();
                separators.PopBack();
            }
        }

        // @notice(stdmatt): The Canonize-XYZ-Options are just useful on Windows...
        if (case_options == CanonizeCaseOptions::ChangeToLowerCase) {
            component.ToUpper();
        } else if (case_options == CanonizeCaseOptions::ChangeToLowerCase) {
            component.ToLower();
        }

        components_string_length += component.Length();
        components.PushBack(component);
        separators.PushBack(separator);
    }
    
    String canonized_path = String::CreateWithCapacity(
        components_string_length + separators.Count()
    );

    size_t const components_count = components.Count();
    for (size_t i = 0; i < components_count ; ++i) {
        canonized_path.PushBack(components[i]);
        if (i < components_count - 1) {
            canonized_path.PushBack(separators[i]);
        }
    }

    return canonized_path;
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
    bool  const result          = (file_attributes & FILE_ATTRIBUTE_NORMAL) != 0;
    return result;
}

bool 
PathUtils::IsDir(String const &path)
{
    // @todo(stdmatt): Check if this is correct.. Dec 21, 2020

    DWORD const file_attributes = GetFileAttributesA(path.CStr());
    bool  const result          = (file_attributes & FILE_ATTRIBUTE_DIRECTORY) != 0;
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
    return CreateDirResult_t::Fail({});
}

//
// Create File
//
PathUtils::CreateFileResult_t 
PathUtils::CreateFile(
    String            const &filename, 
    CreateFileOptions const options)
{
    // @todo(stdmatt): Check if the filename is a valid filename - Dec 21, 2020...
    if (options == CreateFileOptions::ForceCreation) {
        HANDLE win32_handle = CreateFileA(
            filename.c_str(),
            GENERIC_READ | GENERIC_WRITE, 
            FILE_SHARE_DELETE | FILE_SHARE_READ | FILE_SHARE_WRITE, 
            nullptr, 
            TRUNCATE_EXISTING,
            FILE_ATTRIBUTE_NORMAL,
            NULL
        );

        // @todo(stdmatt): Check for errors.... Dec 21, 2020
        CloseHandle(win32_handle);
        
        return CreateFileResult_t::Success(true);
    }

    bool const file_exists = PathUtils::IsFile(filename);
    if (file_exists && options == CreateFileOptions::ErrorIfExists) {
        return CreateFileResult_t::Fail({ FileUtils::ErrorCodes::FILE_ALREADY_EXISTS, filename });
    }

    return CreateFileResult_t::Success(true);
}
