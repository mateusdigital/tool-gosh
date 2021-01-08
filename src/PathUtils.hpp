#pragma once

// Arkadia
#include "Basic.hpp"
#include "Result.hpp"
#include "String.hpp"
#include "OS.hpp"


namespace ark { namespace PathUtils {

//
// Enums / Typedefs / Constants
//
// Only takes effect on Windows Platform, ignored on others.
enum class CanonizeCaseOptions {
    DoNotChange,
    ChangeToLowerCase,
    ChangeToUpperCase,
}; // enum class CanonizeCaseOptions

// Only takes effect on Windows Platform, ignored on others.
enum class CanonizeSlashOptions {
    DoNotChange,
    ChangeToForwardSlashes,
    ChangeToBackwardSlashes
}; // enum CanonizeSlashOptions


bool IsAbs (String const &path);
bool IsFile(String const &path);
bool IsDir (String const &path);

String MakeAbsolute(String const &path);
String MakeRelative(String const &path, String const &to_what_path);
String Canonize(
    String               const &path,
    CanonizeCaseOptions  const case_options  = CanonizeCaseOptions ::DoNotChange,
    CanonizeSlashOptions const slash_options = CanonizeSlashOptions::ChangeToForwardSlashes
);

String GetUserHome();
ARK_INLINE String GetCurrentDir() { return MakeAbsolute(OS::PathCurrentDirectoryString()); }
ARK_INLINE String GetParentDir () { return MakeAbsolute(OS::PathParentDirectoryString ()); }

Array<String> GetDirectoryEntries(String const &path);

template <typename ...Args>
String
Join(String const &a, Args... args)
{
    String result;
    int unpack[]{0, (result += ToString(args), 0)...};
    static_cast<void>(unpack);
    return result;
};


String Dirname (String const &path);
String Basename(String const &path);


//
// Create Dir
//
enum class CreateDirOptions { NonRecursive, Recursive };
typedef Result<bool> CreateDirResult_t;

CreateDirResult_t CreateDir(String const &path, CreateDirOptions const options);

//
// Create File
//
enum class CreateFileOptions { IgnoreIfExists, ErrorIfExists, ForceCreation };
typedef Result<bool> CreateFileResult_t;

CreateFileResult_t CreateFile(String const &filename, CreateFileOptions const options);

} // namespace PathUtils
} // namespace ark
