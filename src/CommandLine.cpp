// Header
#include "CommandLine.hpp"

namespace ark {

namespace Private {
    Array<String> s_CommandLineComponents;
} // namespace Private

//
//
//
void
CommandLine::Set(i32 const argc, char const *argv[])
{
    Private::s_CommandLineComponents.Clear();
    for(i32 i = 0; i < argc; ++i) {
        Private::s_CommandLineComponents.PushBack(argv[i]);
    }
}

void
CommandLine::Set(String const &str)
{
    Private::s_CommandLineComponents = str.Split(' ');
}
    
String
CommandLine::GetAsString()
{
    return String::Join(Private::s_CommandLineComponents);
}

Array<String>
CommandLine::GetAsArray()
{
    return Private::s_CommandLineComponents;
}


u32
CommandLine::ArgsCount()
{
    return Private::s_CommandLineComponents.Count();
}

const char * const
CommandLine::GetArgRaw(u32 const index)
{
    return Private::s_CommandLineComponents[index].CStr();
}

//
//
//
CommandLine::Argument::Argument(
    String                  const &short_name,
    String                  const &long_name,
    String                  const &description,
    ArgumentParseCallback_t const &parse_callback)
    : _short_name(short_name)
    , _long_name(long_name)
    , _description(description)
    , _parse_callback(parse_callback)
{
    // @todo(stdmatt): Assert that things are not empty... Dec 30, 2020
}

//
//
//

String const CommandLine::Parser::ShortFlagPrefix   = "-";
String const CommandLine::Parser::LongFlagPrefix    = "--";
String const CommandLine::Parser::WindowsFlagPrefix = "/";

char const CommandLine::Parser::LongFlagSeparator    = '=';
char const CommandLine::Parser::WindowsFlagSeparator = ':';


CommandLine::Parser::Parser()
    : _cmd_line(GetAsArray())
{
    // Empty...
}

CommandLine::Parser::Parser(String const &cmd_line_str)
    : _cmd_line(cmd_line_str.Split(' '))
{
   // Empty...
}

CommandLine::Parser::Parser(i32 argc, char const *argv[])
{
    _cmd_line.Reserve(argc);
    for(i32 i = 0; i < argc; ++i) {
        _cmd_line.PushBack(argv[i]);
    }
}
CommandLine::Parser::Parser(Array<String> const & cmd_line_items)
    : _cmd_line(_cmd_line)
{
    // Empty...
}

CommandLine::Argument &
CommandLine::Parser::CreateArgument(
    String                            const &short_name,
    String                            const &long_name,
    String                            const &description,
    Argument::ArgumentParseCallback_t const &parse_callback)
{
    // Try to find it before...
    Argument *arg = FindArgumentByName(short_name);
    if(arg) {
        return *arg;
    }
    arg = FindArgumentByName(long_name);
    if(arg) {
        return *arg;
    }

    // Ok we don't have it, so let's create.
    _arguments.EmplaceBack(short_name, long_name, description, parse_callback);
    return _arguments.Back();
}

CommandLine::Argument const *
CommandLine::Parser::FindArgumentByName(String const &name) const
{
    for(Argument const &arg : _arguments) {
        if(arg.GetShortName() == name || arg.GetLongName() == name) {
            return &arg;
        }
    }
    return nullptr;
}

CommandLine::Parser::ParseResult_t
CommandLine::Parser::Evaluate()
{
    Array<String> clean_components;
    clean_components.Reserve(_cmd_line.Count());
    // Clean up the components of the command line.
    //   This will make ALL the items to be in a different string.
    //   For example:
    //      prog -abc          -> prog -a -b -c
    //      prog --flag= value -> prog --flag value
    for(
        size_t i = 0,
        cmd_line_count = _cmd_line.Count();
        i < cmd_line_count;
        ++i)
    {
        String const &item = _cmd_line[i];
        if(IsShortFlag(item)) {
            Array<String> split_flags = SplitShortFlag(item);
            clean_components.PushBack(split_flags);
        }
        else if(IsLongFlag(item)) {
            Array<String> split_flags = SplitLongFlag(item);
            clean_components.PushBack(split_flags);
        }
        else {
            clean_components.PushBack(item);
        }
    }
    return ParseResult_t::Success(true);
}

bool
CommandLine::Parser::IsShortFlag(String const &item) const
{
    if(item.IsEmptyOrWhitespace()) {
        return false;
    }
    size_t const len = item.Length();
    if(len < 2) {
        return false;
    }

    if(item.StartsWith(Parser::LongFlagPrefix) || item.StartsWith(Parser::WindowsFlagPrefix)) {
        return false;
    }

    return item.StartsWith(Parser::ShortFlagPrefix);
}


bool
CommandLine::Parser::IsLongFlag(String const &item) const
{
   if(IsShortFlag(item)) {
       return false;
   }

    if(item.StartsWith(Parser::LongFlagPrefix) || item.StartsWith(Parser::WindowsFlagPrefix)) {
        return true;
    }

    return false;
}

Array<String>
CommandLine::Parser::SplitShortFlag(String const &item) const
{
    // Given the arguments:
    //    prog -abc
    // We can split the flag in two different ways.
    //    1) prog -a -b -c  : Each char becomes a flag.
    //    2) prog -a bc     : -a is a flag and bc is the value of it.
    // To decide we need to check the current Arguments and see if
    // we have an argument with that name, if so it's a flag
    // otherwise it's a value.
    Array<String> split_items;
    for(size_t i = 1, item_len = item.Length(); i < item_len; ++i) {
        // @todo(stdmatt): O(n^2) just to find the flags, improve it - Dec 31, 2020.
        // @todo(stdmatt): Creating a string just to pass a char - Dec 31, 2020.
        char const c = item[i];
        Argument const * const arg = FindArgumentByName(String(c));
        if(arg) { // This is a flag...
            split_items.PushBack(String::Format("-%c", c));
        } else {
            split_items.PushBack(String(c));
        }
    }

    return split_items;
}

Array<String>
CommandLine::Parser::SplitLongFlag(String const &item) const
{
    Array<String> split_items =  item.Split(
        Array<char>{ Parser::LongFlagSeparator, Parser::WindowsFlagSeparator }
    );

    return split_items;
}


} // namespace ark
