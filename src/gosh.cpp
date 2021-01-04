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
constexpr auto GOSH_INSTALL_PATH       = ".stdmatt/gosh";
constexpr auto GOSH_BOOKMARKS_FILENAME = "bookmarks.txt";

constexpr auto BOOKMARK_COMMENT_CHAR   = '#';
constexpr auto BOOKMARK_SEPARATOR_CHAR = '>';
constexpr auto GOSH_EXIT_CHANGE_DIR    = 0;
constexpr auto GOSH_EXIT_NORMAL_OUTPUT = -1;

template <typename ...Args>
ark_internal_function void
PrintLn(String const &fmt, Args... args)
{
    fmt::print(fmt.CStr(), args...);
    printf("\n");
}

#define Exit(_code_) do { printf("\n"); exit(_code_); } while(0)

//
// Helper Functions
//
//------------------------------------------------------------------------------
template <typename ...Args>
ark_internal_function void
Die(String const &fmt, Args ...args)
{
    String const formatted = String::Format(fmt, args...);
    PrintLn("[FATAL] {}", formatted);
    Exit(1);
}

//------------------------------------------------------------------------------
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
"    and Exit after the operation.\n"
);
    Exit(code);
}

//------------------------------------------------------------------------------
ark_internal_function void
ShowVersion(u32 code)
{

}



//
// Bookmarks
//
//------------------------------------------------------------------------------
struct Bookmark {
    String name;
    String path;
};

//------------------------------------------------------------------------------
struct Bookmarks  {
    Array<Bookmark> items;
}; // struct Bookmarks


//------------------------------------------------------------------------------
ark_internal_function bool
operator==(Bookmark const &lhs, Bookmark const &rhs)
{
    return lhs.name == rhs.name;
}

//------------------------------------------------------------------------------
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

//------------------------------------------------------------------------------
ark_internal_function Bookmarks
LoadBookmarks()
{
    String const bookmarks_path = GetBookmarksPath();
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
            "Failed to load bookmarks file!\n    Path: ({})",
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
                "Error parsing bookmarks!\n    Path: ({})\n    Line: ({})",
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

//------------------------------------------------------------------------------
ark_internal_function void
SaveBookmarks(Bookmarks const &bookmarks)
{
    Array<String> lines;
    for(Bookmark const &bookmark : bookmarks.items) {
        String const line = String::Format(
            "{}{}{}",
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
            "Error writing bookmarks!\n    Path: ({})\n    Error: ({})",
            Terminal::Colored(bookmarks_path,     Terminal::Color::Magenta),
            Terminal::Colored(write_result.value, Terminal::Color::Blue)
        );
    }
}


//------------------------------------------------------------------------------
enum class EnsureBookmarkOptions {
    MustExist, 
    MustNotExist
};

//------------------------------------------------------------------------------
ark_internal_function void
EnsureBookmark(
    Bookmarks             const &bookmarks,
    String                const &clean_name,
    EnsureBookmarkOptions const options)
{
    auto const it = std::find_if(
        std::begin(bookmarks.items),
        std::end  (bookmarks.items),
        [&clean_name](Bookmark const &item){
            return item.name == clean_name;
        }
    );

    if(options == EnsureBookmarkOptions::MustNotExist && it != std::end(bookmarks.items)) {
        Die("Bookmark ({0}) already exists...", clean_name);
    } else if(options == EnsureBookmarkOptions::MustExist && it == std::end(bookmarks.items)) {
        Die("Bookmark ({0}) must exists...", clean_name);
    }
}

//------------------------------------------------------------------------------
ark_internal_function void
EnsureValidPath(String const &path)
{
    if (!PathUtils::IsDir(path)) {
        Die("Invalid Path: ({})", Terminal::Colored(path, Terminal::Color::Magenta));
    }
}


//
// Bookmark Actions
//
//------------------------------------------------------------------------------
enum class ListOptions {
    Short,
    Long
};

//------------------------------------------------------------------------------
ark_internal_function void
ListBookmarks(ListOptions const options)
{
    Bookmarks bookmarks = LoadBookmarks();
    if (bookmarks.items.Count() == 0) {
        PrintLn("No bookmarks yet... :/");
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
            PrintLn("{}", Terminal::Colored(bookmark.name, Terminal::Color::Blue));
            continue;
        }

        size_t const spaces_count = longest_name - bookmark.name.size();
        String const spaces       = String::Repeat(" ", spaces_count);

        PrintLn(
            "{}{} : {}",
            spaces,
            Terminal::Colored(bookmark.name, Terminal::Color::Blue),
            Terminal::Colored(bookmark.path, Terminal::Color::Magenta)
        );
    }
}

//------------------------------------------------------------------------------
ark_internal_function void
AddBookmark(String const &name, String const &path)
{
    String clean_name = name;
    String clean_path = path;

    if(name.IsEmptyOrWhitespace()) {
        // This makes the name of the bookmark to be the same
        // as the last part of the path
        clean_path = PathUtils::GetCWD();
        clean_name = PathUtils::Basename(clean_path);
    } else if(path.IsEmptyOrWhitespace()) {
        clean_path = PathUtils::GetCWD();
    }

    clean_name.Trim();

    Bookmarks bookmarks = LoadBookmarks();
    EnsureBookmark(bookmarks, clean_name, EnsureBookmarkOptions::MustNotExist);

    String const added_path = PathUtils::MakeRelative(clean_path, PathUtils::GetUserHome());
    EnsureValidPath(added_path);

    bookmarks.items.push_back({clean_name, added_path});
    SaveBookmarks(bookmarks);

    PrintLn(
        "Bookmark added:\n  ({}) - ({})",
        Terminal::Colored(clean_name,Terminal::Color::Blue),
        Terminal::Colored(added_path,Terminal::Color::Magenta)
    );
}

//------------------------------------------------------------------------------
ark_internal_function void
RemoveBookmark(String const &name)
{
    String clean_name = name;
    clean_name.Trim();

    Bookmarks bookmarks = LoadBookmarks();
    EnsureBookmark(bookmarks, clean_name, EnsureBookmarkOptions::MustNotExist);

    size_t const index = bookmarks.items.IndexOf([&clean_name](Bookmark const &item){
        return item.name == clean_name;
    });
    ARK_ASSERT_TRUE(index != Array<String>::InvalidIndex, "Can't find bookmark named ({})", clean_name);

    Bookmark const copy = bookmarks.items[index];
    bookmarks.items.RemoveAt(index);
    SaveBookmarks(bookmarks);

    PrintLn(
        "Bookmark removed :\n  ({}) - ({})",
        Terminal::Colored(copy.name, Terminal::Color::Blue),
        Terminal::Colored(copy.path, Terminal::Color::Magenta)
    );
}

ark_internal_function void
UpdateBookmark(String const &name, String const &path)
{

}

ark_internal_function void
DoesBookmarkExists(String const &name)
{
    String clean_name = name;
    clean_name.Trim();

    Bookmarks bookmarks = LoadBookmarks();
    size_t const index = bookmarks.items.IndexOf([&clean_name](Bookmark const &item){
        return item.name == clean_name;
    });

    if(index == Array<String>::InvalidIndex) {
        PrintLn("No bookmark");
    }
    else {
        PrintLn(
            "Bookmark: ({})",
            Terminal::Colored(clean_name, Terminal::Color::Blue)
        );
    }
}

ark_internal_function size_t
PrintBookmark(String const &name)
{
    String clean_name = name;
    clean_name.Trim();

    Bookmarks const bookmarks = LoadBookmarks();
    size_t const index = bookmarks.items.IndexOf([&clean_name](Bookmark const &item){
        return item.name == clean_name;
    });
    Bookmark const &bookmark = bookmarks.items[index];

    if(index != Array<String>::InvalidIndex) {
        PrintLn(
            "{}",
            Terminal::Colored(bookmark.path, Terminal::Color::Blue)
        );
        return GOSH_EXIT_CHANGE_DIR;
    }

    return GOSH_EXIT_NORMAL_OUTPUT;
}




//
// Entry Point
//
int
main(int const argc, char const *argv[])
{
    using namespace ark;
    CommandLine::Set(argc, argv);
    // CommandLine::Set("gosh -L");
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
            ShowHelp(GOSH_EXIT_NORMAL_OUTPUT);
        })
    );
    cmd_parser.CreateArgument(
        "v",
        "version",
        "Show version and copyright info",
        CommandLine::Argument::RequiresNoValues,
        EAZ_ARG_FOUND_FUNC({
            ShowVersion(GOSH_EXIT_NORMAL_OUTPUT);
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
            Exit(GOSH_EXIT_NORMAL_OUTPUT);
        })
    );
    cmd_parser.CreateArgument(
        "L",
        String::Empty(),
        "List the bookmarks - Long.",
        CommandLine::Argument::RequiresNoValues,
        EAZ_ARG_FOUND_FUNC({
            ListBookmarks(ListOptions::Long);
            Exit(GOSH_EXIT_NORMAL_OUTPUT);
        })
    );

    //
    // Add / Remove / Update
    //
    cmd_parser.CreateArgument(
        "a",
        "add",
        "Add a new bookmark.",
        CommandLine::Argument::MinMax_t(0, 2)
    );

    cmd_parser.CreateArgument(
        "r",
        "rm",
        "Delete a bookmark bookmark.",
        CommandLine::Argument::MinMax_t(1, 1)
    );

    cmd_parser.CreateArgument(
        "u",
        "update",
        "Update a new bookmark.",
        CommandLine::Argument::MinMax_t(2, 2)
    );
    //
    // Update / Print
    //
    cmd_parser.CreateArgument(
        "e",
        "exists",
        "Check if bookmark exists",
        CommandLine::Argument::MinMax_t(1, 1)
    );

    cmd_parser.CreateArgument(
        "p",
        "print",
        "Print bookmark's path",
        CommandLine::Argument::MinMax_t(1, 1)
    );

    CommandLine::Parser::ParseResult_t const result = cmd_parser.Evaluate();
    if(result.HasFailed()) {
        Die("{}", result.GetError().error_msg);
    }

    CommandLine::Argument *arg = nullptr;
    //
    // Add / Remove / Update
    //
    arg = cmd_parser.FindArgumentByName("add");
    if(arg->WasFound()) {
        String const name = arg->ValuesCount() == 1 ? arg->GetValues().Back() : "";
        String const path = arg->ValuesCount() == 2 ? arg->GetValues().Back() : ".";
        AddBookmark(name, path);
        Exit(GOSH_EXIT_NORMAL_OUTPUT);
    }
    arg = cmd_parser.FindArgumentByName("rm");
    if(arg->WasFound()) {
        RemoveBookmark(arg->GetValues().Front());
        Exit(GOSH_EXIT_NORMAL_OUTPUT);
    }
    arg = cmd_parser.FindArgumentByName("update");
    if(arg->WasFound()) {
        UpdateBookmark(arg->GetValues().Front(), arg->GetValues().Back());
        Exit(GOSH_EXIT_NORMAL_OUTPUT);
    }

    //
    // Exists / Print
    //
    arg = cmd_parser.FindArgumentByName("exists");
    if(arg->WasFound()) {
        DoesBookmarkExists(arg->GetValues().Front());
        Exit(GOSH_EXIT_NORMAL_OUTPUT);
    }
    arg = cmd_parser.FindArgumentByName("print");
    if(arg->WasFound() || cmd_parser.GetPositionalValues().Count() > 1) {
        size_t const value = PrintBookmark(arg->GetValues().Front());
        Exit(value);
    }

    Exit(GOSH_EXIT_NORMAL_OUTPUT);
}
