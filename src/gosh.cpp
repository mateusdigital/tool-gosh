#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <algorithm>
#include <iterator>

#include "BasicTypes.hpp"
#include "Array.hpp"
#include "String.hpp"
#include "Result.hpp"
#include "FileUtils.hpp"
#include "OS.hpp"
#include "PathUtils.hpp"
#include "Terminal.hpp"


//
// Constants
//
constexpr auto ACTION_HELP_STR      = "gosh_opt_help";
constexpr auto ACTION_VERSION_STR   = "gosh_opt_version";
constexpr auto ACTION_LIST_STR      = "gosh_opt_list";
constexpr auto ACTION_LIST_LONG_STR = "gosh_opt_list-long";
constexpr auto ACTION_REMOVE_STR    = "gosh_opt_remove";
constexpr auto ACTION_ADD_STR       = "gosh_opt_add";
constexpr auto ACTION_UPDATE_STR    = "gosh_opt_update";
constexpr auto ACTION_PRINT_STR     = "gosh_opt_print";
constexpr auto ACTION_EXISTS_STR    = "gosh_opt_exists_bookmark";

constexpr auto GOSH_INSTALL_PATH = ".stdmatt/gosh";

constexpr auto BOOKMARK_COMMENT_CHAR   = '#';
constexpr auto BOOKMARK_SEPARATOR_CHAR = ':';

template <typename To_Type, typename From_Type>
To_Type Cast(From_Type value) { return static_cast<To_Type>(value); }

template <typename ...Args> 
static void 
WriteOutput(String const &fmt, Args... args)
{

}


//
// Macros
//
#define Die(_fmt_, ...) do { } while(0)


static char const * const
SafeGetArg(int argc, char const *argv[], int index)
{
    if(index > (argc-1)) {
        return nullptr;
    }
    return argv[1];
}

static void
Exit(int code)
{
    exit(code);
}

static void
ShowHelp()
{

}

static void
ShowVersion()
{

}



//
// Bookmarks
//
struct Bookmark {
    std::string name;
    std::string path;
};

struct Bookmarks 
{
    size_t          count;
    Array<Bookmark> list;
}; // struct Bookmarks

static String
GetBookmarksPath()
{
    String const &user_home = PathUtils::GetUserHome();
    String const full_path = PathUtils::Join(user_home, GOSH_INSTALL_PATH);

    return full_path;
}

static Bookmarks
LoadBookmarks()
{
    String const bookmarks_path = GetBookmarksPath();
    if(!PathUtils::IsFile(bookmarks_path)) {
        PathUtils::CreateDirResult_t create_dir_result = PathUtils::CreateDir(
            PathUtils::Dirname(bookmarks_path),
            PathUtils::CreateDirOptions::Recursive
        );
        if(create_dir_result.HasFailed()) {
            // @todo(stdmatt): Die with log...
        }

        PathUtils::CreateFileResult_t create_file_result = PathUtils::CreateFile(
            bookmarks_path,
            PathUtils::CreateFileOptions::IgnoreIfExists
        );
        if(create_dir_result.HasFailed()) {
            // @todo(stdmatt): Die with log...
        }
    }

    FileUtils::ReadAllLinesResult_t read_result = FileUtils::ReadAllLines(bookmarks_path);
    if(read_result.HasFailed()) {
        Die(
            "Failed to load bookmarks file!\n    Path: (%s)",
            Terminal::Colored(bookmarks_path, Terminal::Color::Magenta)
        );
    }


    Bookmarks bookmarks;
    size_t const lines_count = read_result.value.Count();
    for(size_t line_no = 0;
        line_no < lines_count;
        ++line_no)
    {
        String &line = read_result.value[line_no];
        line.Trim();

        if(line.IsEmptyOrWhitespace()) {
            continue;
        }
        if(line.StartsWith(BOOKMARK_COMMENT_CHAR)) {
            continue;
        }

        Array<String> components = line.Split(BOOKMARK_SEPARATOR_CHAR);
        if(components.Count() != 2) {
            Die(
                "Error parsing bookmarks!\n    Path: (%s)\n    Line: (%d)",
                Terminal::Colored(Terminal::Color::Magenta, bookmarks_path),
                Terminal::Colored(Terminal::Color::Blue   , line_no)
            );
        }

        bookmarks.list.PushBack({
            components[0],
            components[1]
        });
    }

    return bookmarks;
}

static void
SaveBookmarks(Bookmarks const &bookmarks)
{
    Array<String> lines;
    for(Bookmark const &bookmark : bookmarks.list) {
        String const line = String::Format(
            "%s %c %s",
            bookmark.name,
            BOOKMARK_SEPARATOR_CHAR,
            bookmark.path
        );

        lines.PushBack(line);
    }

    String const bookmarks_path = GetBookmarksPath();
    FileUtils::WriteResult_t write_result = FileUtils::WriteAllLines(
        bookmarks_path,
        lines,
        OS::NewLineString(),
        FileUtils::WriteMode::Overwrite
    );

    if(write_result.HasFailed()) {
        Die(
            "Error writting bookmarks!\n    Path: (%s)\n    Error: (%d)",
            Terminal::Colored(Terminal::Color::Magenta, bookmarks_path),
            Terminal::Colored(Terminal::Color::Blue   , Cast<int>(write_result.value))
        );
    }
}


enum class EnsureBoorkmarkOptions {
    MustExist, 
    MustNotExist
};

static void 
EnsureBookmark(String const &clean_name, EnsureBoorkmarkOptions const options)
{

}

static void 
EnsureValidPath(String const &path)
{
    if(!PathUtils::IsDir(path)) {
        Die("Invalid Path: (%s)", Terminal::Colored(Terminal::Color::Magenta, path));
    }
}
//
// Bookmark Actions
//
enum class ListOptions {
    Short,
    Long
};

static void
ListBookmarks(ListOptions const options)
{
    Bookmarks bookmarks = LoadBookmarks();
    if(bookmarks.count == 0) {
        WriteOutput("No bookmarks yet... :/");
        return;
    }

    // Find the longest bookmark name.
    // This will be used to make the bookmarks aligned.
    size_t longest_name = 0;
    for(size_t i = 0; i < bookmarks.count; ++i) {
        if(longest_name < bookmarks.list[i].name.size()) {
            longest_name = bookmarks.list[i].name.size();
        }
    }

    // Display output alphabetcally...
    std::sort(
        std::begin(bookmarks.list),
        std::end  (bookmarks.list),
        [](Bookmark const &lhs, Bookmark const &rhs) {
            return lhs.name < rhs.name;
        }
    );

    for(size_t i = 0; i < bookmarks.count; ++i) {
        Bookmark const &bookmark = bookmarks.list[i];
        if(options == ListOptions::Short) {
            WriteOutput(Terminal::Colored(bookmark.name, Terminal::Color::Blue));
            continue;
        }

        int const spaces_count = longest_name - bookmark.name.size();
        WriteOutput(
            "%-*s : %s",
            spaces_count,
            Terminal::Colored(bookmark.name, Terminal::Color::Blue),
            Terminal::Colored(bookmark.path, Terminal::Color::Magenta)
        );
    }

}

static void
AddBookmark(char const * const name, char const * const path)
{
    String clean_name = name;
    String clean_path = path;

    if(CStrIsNullEmptyOrWhitespace(path)) {
        clean_path = ".";
    }

    if(CStrIsNullEmptyOrWhitespace(name)) {
        // This makes the name of the bookmark to be the same
        // as the last part of the path
        clean_name = PathUtils::Basename(PathUtils::Canonize("."));
    }


    clean_name.Trim();
    EnsureBookmark(clean_name, EnsureBoorkmarkOptions::MustExist);

    String added_path = PathUtils::MakeRelative(clean_name, PathUtils::GetUserHome());
    EnsureValidPath(added_path);


    Bookmarks bookmarks = LoadBookmarks();
    bookmarks.list.push_back({clean_name, added_path});
    SaveBookmarks(bookmarks);

    WriteOutput(
        "Bookmark added:\n  (%s) - (%s)",
        Terminal::Colored(clean_name,Terminal::Color::Blue),
        Terminal::Colored(added_path,Terminal::Color::Magenta)
    );
}

static void
RemoveBookmark(char const * const name)
{

}

static void
UpdateBookmark(char const * const name, char const * const path)
{

}

static void
DoesBookmarkExists(char const * const name)
{

}

static void
PrintBookmark(char const * const name)
{

}




//
// Entry Point
//
int
main(int argc, char const *argv[])
{
    if(argc < 2 ) {
        ShowHelp();
        Exit(1);
    }

    char const * const first_arg  = SafeGetArg(argc, argv, 0);
    char const * const second_arg = SafeGetArg(argc, argv, 1);
    char const * const third_arg  = SafeGetArg(argc, argv, 2);

    //
    // Help / Version
    //
    if(CStrEquals(ACTION_HELP_STR, first_arg)) {
        ShowHelp();
        Exit(0);
    } else if(CStrEquals(ACTION_VERSION_STR, first_arg)) {
        ShowVersion();
        Exit(0);
    }
    //
    // List
    //
    else if(CStrEquals(ACTION_LIST_STR, first_arg)) {
        ListBookmarks(ListOptions::Short);
        Exit(0);
    } else if(CStrEquals(ACTION_LIST_LONG_STR, first_arg)) {
        ListBookmarks(ListOptions::Long);
        Exit(0);
    }
    //
    // Add
    //
    else if(CStrEquals(ACTION_ADD_STR, first_arg)) {
        AddBookmark(second_arg, third_arg);
        Exit(0);
    }
    //
    // Remove
    //
    else if(CStrEquals(ACTION_REMOVE_STR, first_arg)) {
        RemoveBookmark(second_arg);
        Exit(0);
    }
    //
    // Update
    //
    else if(CStrEquals(ACTION_UPDATE_STR, first_arg)) {
        UpdateBookmark(second_arg, third_arg);
        Exit(0);
    }
    //
    // Exists
    //
    else if(CStrEquals(ACTION_EXISTS_STR, first_arg)) {
        DoesBookmarkExists(second_arg);
        Exit(0);
    }
    //
    // Print
    //
    else if(CStrEquals(ACTION_PRINT_STR, first_arg)) {
        PrintBookmark(second_arg);
        Exit(0);
    }
}
