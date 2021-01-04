#pragma once

// Arkadia
#include "Result.hpp"
#include "String.hpp"

namespace ark { namespace PathUtils {

String GetUserHome();
String GetCWD     ();

bool IsAbs (String const &path);
bool IsFile(String const &path);
bool IsDir (String const &path);

String MakeAbsolute(String const &path);
String MakeRelative(String const &path, String const &to_what_path);


// @todo(stdmatt): Make it a variadic template... Dec 20, 2020.
String Join(
    String const &a,
    String const &b = "",
    String const &c = ""
);

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

String Canonize(
    String               const &path,
    CanonizeCaseOptions  const case_options  = CanonizeCaseOptions ::DoNotChange,
    CanonizeSlashOptions const slash_options = CanonizeSlashOptions::ChangeToForwardSlashes
);

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
