// Arkadia
#include "BasicTypes.hpp"
#include "CommandLine.hpp"
#include "Array.hpp"
#include "String.hpp"
#include "Result.hpp"
#include "FileUtils.hpp"
#include "OS.hpp"
#include "PathUtils.hpp"
#include "Terminal.hpp"

using namespace ark;

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

constexpr auto GOSH_INSTALL_PATH       = ".stdmatt/gosh";
constexpr auto GOSH_BOOKMARKS_FILENAME = "bookmarks.txt";

constexpr auto BOOKMARK_COMMENT_CHAR   = '#';
constexpr auto BOOKMARK_SEPARATOR_CHAR = ':';

#include "fmt/core.h"

template <typename ...Args> 
ark_internal_function void
WriteOutput(String const &fmt, Args... args)
{
    fmt::print(fmt.CStr(), args...);
}


//
// Macros
//
template <typename ...Args>
ark_internal_function void
Die(String const &fmt, Args ...args)
{
    String const formatted = String::Format(fmt, args...);
    WriteOutput("[FATAL] {}\n", formatted);
    exit(1);
}

ark_internal_function void
ShowHelp(u32 const code)
{
    printf("%s\n",
"Usage:\n"
"    gosh                        (Same as gosh -l)\n"
"    gosh <name>                 (To change the directory)\n"
"    gosh -h | -v                (Show help | version)\n"
"    gosh -l | -L                (Show list of bookmarks)\n"
"    gosh -p <name>              (Show path for bookmark)\n"
"    gosh -e <path>              (Show bookmark for path)\n"
"    gosh -a | -u <name> <path>  (Add | Update bookmark)\n"
"    gosh -r <name>              (Remove the bookmark)\n"
"    \n"
"Options:\n"
"    *-h --help     : Show this screen.\n"
"    *-v --version  : Show app version and copyright.\n"
"    \n"
"    *-e --exists <path>  : Print the Bookmark for path.\n"
"    *-p --print  <name>  : Print the path of Bookmark.\n"
"    \n"
"    *-l --list       : Show all Bookmarks (no Paths).\n"
"    *-L --list-long  : Show all Bookmarks and Paths.\n"
"    \n"
"    *-a --add    <name> <path>  : Add a Bookmark with specified path.\n"
"    *-r --remove <name>         : Remove a Bookmark.\n"
"    *-u --update <name> <path>  : Update a Bookmark to path.\n"
"    \n"
"    -n --no-colors : Print the output without colors.\n"
"    \n"
"Notes:\n"
"    If <path> is blank the current dir is assumed.\n"
"    \n"
"    Options marked with * are exclusive, i.e. the gosh will run that\n"
"    and exit after the operation.\n"
);
    exit(code);
}

ark_internal_function void
ShowVersion(u32 code)
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
    Array<Bookmark> items;
}; // struct Bookmarks

ark_internal_function String
GetBookmarksPath()
{
    String const &user_home = PathUtils::GetUserHome();
    String const full_path = PathUtils::Join(
        user_home,
        GOSH_INSTALL_PATH,
        GOSH_BOOKMARKS_FILENAME
    );
    return full_path;
}

ark_internal_function Bookmarks
LoadBookmarks()
{
    String const bookmarks_path = GetBookmarksPath();
    printf("%s\n", bookmarks_path.CStr());
    fflush(stdout);
    if (!PathUtils::IsFile(bookmarks_path)) {
        PathUtils::CreateDirResult_t const create_dir_result = PathUtils::CreateDir(
            PathUtils::Dirname(bookmarks_path),
            PathUtils::CreateDirOptions::Recursive
        );
        if (create_dir_result.HasFailed()) {
            // @todo(stdmatt): Die with log...
        }

        PathUtils::CreateFileResult_t create_file_result = PathUtils::CreateFile(
            bookmarks_path,
            PathUtils::CreateFileOptions::IgnoreIfExists
        );
        if (create_dir_result.HasFailed()) {
            // @todo(stdmatt): Die with log...
        }
    }

    FileUtils::ReadAllLinesResult_t read_result = FileUtils::ReadAllLines(bookmarks_path);
    if (read_result.HasFailed()) {
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

        if (line.IsEmptyOrWhitespace()) {
            continue;
        }
        if (line.StartsWith(BOOKMARK_COMMENT_CHAR)) {
            continue;
        }

        Array<String> components = line.Split(BOOKMARK_SEPARATOR_CHAR);
        if (components.Count() != 2) {
            Die(
                "Error parsing bookmarks!\n    Path: (%s)\n    Line: (%d)",
                Terminal::Colored(bookmarks_path, Terminal::Color::Magenta),
                Terminal::Colored(line_no,        Terminal::Color::Blue)
            );
        }

        bookmarks.items.PushBack({
            components[0],
            components[1]
        });
    }

    return bookmarks;
}

ark_internal_function void
SaveBookmarks(Bookmarks const &bookmarks)
{
    Array<String> lines;
    for(Bookmark const &bookmark : bookmarks.items) {
        String const line = String::Format(
            "%s %c %s",
            bookmark.name,
            BOOKMARK_SEPARATOR_CHAR,
            bookmark.path
        );

        lines.PushBack(line);
    }

    String const bookmarks_path = GetBookmarksPath();
    FileUtils::WriteResult_t const write_result = FileUtils::WriteAllLines(
        bookmarks_path,
        lines,
        OS::NewLineString(),
        FileUtils::WriteMode::Overwrite
    );

    if (write_result.HasFailed()) {
        Die(
            "Error writting bookmarks!\n    Path: (%s)\n    Error: (%d)",
            Terminal::Colored(bookmarks_path,     Terminal::Color::Magenta),
            Terminal::Colored(write_result.value, Terminal::Color::Blue)
        );
    }
}


enum class EnsureBookmarkOptions {
    MustExist, 
    MustNotExist
};

ark_internal_function void
EnsureBookmark(String const &clean_name, EnsureBookmarkOptions const options)
{

}

ark_internal_function void
EnsureValidPath(String const &path)
{
    if (!PathUtils::IsDir(path)) {
        Die("Invalid Path: (%s)", Terminal::Colored(path, Terminal::Color::Magenta));
    }
}


//
// Bookmark Actions
//
enum class ListOptions {
    Short,
    Long
};

ark_internal_function void
ListBookmarks(ListOptions const options)
{
    Bookmarks bookmarks = LoadBookmarks();
    if (bookmarks.items.Count() == 0) {
        WriteOutput("No bookmarks yet... :/");
        return;
    }

    // Find the longest bookmark name.
    // This will be used to make the bookmarks aligned.
    size_t longest_name = 0;
    for(size_t i = 0; i < bookmarks.items.Count(); ++i) {
        if (longest_name < bookmarks.items[i].name.size()) {
            longest_name = bookmarks.items[i].name.size();
        }
    }

    // Display output alphabetcally...
    std::sort(
        std::begin(bookmarks.items),
        std::end  (bookmarks.items),
        [](Bookmark const &lhs, Bookmark const &rhs) {
            return lhs.name < rhs.name;
        }
    );

    for(size_t i = 0; i < bookmarks.items.Count(); ++i) {
        Bookmark const &bookmark = bookmarks.items[i];
        if (options == ListOptions::Short) {
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

ark_internal_function void
AddBookmark(String const &name, String const &path)
{
    String clean_name = name;
    String clean_path = path;

    if (path.IsEmptyOrWhitespace()) {
        clean_path = ".";
    }

    if (name.IsEmptyOrWhitespace()) {
        // This makes the name of the bookmark to be the same
        // as the last part of the path
        clean_name = PathUtils::Basename(PathUtils::Canonize("."));
    }


    clean_name.Trim();
    EnsureBookmark(clean_name, EnsureBookmarkOptions::MustExist);

    String const added_path = PathUtils::MakeRelative(clean_name, PathUtils::GetUserHome());
    EnsureValidPath(added_path);


    Bookmarks bookmarks = LoadBookmarks();
    bookmarks.items.push_back({clean_name, added_path});
    SaveBookmarks(bookmarks);

    WriteOutput(
        "Bookmark added:\n  (%s) - (%s)",
        Terminal::Colored(clean_name,Terminal::Color::Blue),
        Terminal::Colored(added_path,Terminal::Color::Magenta)
    );
}

ark_internal_function void
RemoveBookmark(String const &name)
{

}

ark_internal_function void
UpdateBookmark(String const &name, String const &path)
{

}

ark_internal_function void
DoesBookmarkExists(String const &name)
{

}

ark_internal_function void
PrintBookmark(String const &name)
{

}




//
// Entry Point
//
int
main(int argc, char const *argv[])
{
    using namespace ark;
    CommandLine::Set(argc, argv);
    CommandLine::Set("gosh -e");
    CommandLine::Parser cmd_parser;
    //
    // Help / Version
    //
    cmd_parser.CreateArgument(
        "h",
        "help",
        "Display this screen",
        CommandLine::Argument::RequiresNoValues,
        EAZ_ARG_FOUND_FUNC({
            ShowHelp(EXIT_SUCCESS);
        })
    );
    cmd_parser.CreateArgument(
        "v",
        "version",
        "Show version and copyright info",
        CommandLine::Argument::RequiresNoValues,
        EAZ_ARG_FOUND_FUNC({
            ShowVersion(EXIT_SUCCESS);
        })
    );

    //
    // List
    //
    cmd_parser.CreateArgument(
        "l",
        String::Empty(),
        "List the bookmarks - Short.",
        CommandLine::Argument::RequiresNoValues,
        EAZ_ARG_FOUND_FUNC({
            ListBookmarks(ListOptions::Short);
        })
    );
    cmd_parser.CreateArgument(
        "L",
        String::Empty(),
        "List the bookmarks - Long.",
        CommandLine::Argument::RequiresNoValues,
        EAZ_ARG_FOUND_FUNC({
            ListBookmarks(ListOptions::Long);
        })
    );

    //
    // Add / Remove / Update
    //
    CommandLine::Argument const &add_arg = cmd_parser.CreateArgument(
        "a",
        "add",
        "Add a new bookmark.",
        CommandLine::Argument::MinMax_t(2, 2)
    );

    CommandLine::Argument const &remove_arg = cmd_parser.CreateArgument(
        "r",
        "rm",
        "Delete a bookmark bookmark.",
        CommandLine::Argument::MinMax_t(1, 1)
    );

    CommandLine::Argument const update_arg = cmd_parser.CreateArgument(
        "u",
        "update",
        "Update a new bookmark.",
        CommandLine::Argument::MinMax_t(2, 2)
    );
    //
    // Update / Print
    //
    CommandLine::Argument const exists_arg = cmd_parser.CreateArgument(
        "e",
        "exists",
        "Check if bookmark exists",
        CommandLine::Argument::MinMax_t(1, 1)
    );

    CommandLine::Argument const print_arg = cmd_parser.CreateArgument(
        "p",
        "print",
        "Print bookmark's path",
        CommandLine::Argument::MinMax_t(1, 1)
    );

    CommandLine::Parser::ParseResult_t result = cmd_parser.Evaluate();
    if(result.HasFailed()) {
        Die("{}", result.GetError().error_msg);
    }

    //
    // Add / Remove / Update
    //
    if(add_arg.HasValues()) {
        AddBookmark(add_arg.GetValues().Front(), add_arg.GetValues().Back());
    }
    if(remove_arg.HasValues()) {
        RemoveBookmark(remove_arg.GetValues().Front());
    }
    if(update_arg.HasValues()) {
        UpdateBookmark(update_arg.GetValues().Front(), update_arg.GetValues().Back());
    }

    //
    // Exists / Print
    //
    if(exists_arg.HasValues()) {
        DoesBookmarkExists(exists_arg.GetValues().Front());
    }
    if(print_arg.HasValues()) {
        PrintBookmark(print_arg.GetValues().Front());
    }
}
