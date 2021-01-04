#pragma once
// std
#include <functional>
// Arkadia
#include "Assert.hpp"
#include "BasicTypes.hpp"
#include "CodeUtils.hpp"
#include "Macros.hpp"
#include "Array.hpp"
#include "Debug.hpp"
#include "String.hpp"
#include "Result.hpp"


namespace ark { namespace CommandLine {

//
//
//
void Set(i32 const argc, char const *argv[]);
void Set(String const &str);

String        GetAsString();
Array<String> GetAsArray ();

u32                ArgsCount();
const char * const GetArgRaw(u32 const index);


//
//
//
enum class ArgumentType
{
    Posix,    ///< [Flag] Single | [Prefix] '-'  | [Value] ' '      |  cmd   -f     value
    GNU,      ///< [Flag] Multi  | [Prefix] '--' | [Value] ' ', '=' |  cmd --flag = value
    Windows   ///< [Flag] Multi  | [Prefix] '/'  | [Value] ' ', ":' |  cmd  /flag : value
}; // enum Argument Type


class Argument
{
public:
    enum class ParseStatus {
        Valid, Invalid
    }; // enum ParseStatus

    typedef std::function<ParseStatus(String const &)> ArgumentParseCallback_t;
    typedef MinMax<u32> MinMax_t;

    static const MinMax_t RequiresNoValues;
    static const MinMax_t RequiresOneValue;

public:
    Argument(
        String                  const &short_name,
        String                  const &long_name,
        String                  const &description,
        MinMax_t                const &values_requirement,
        ArgumentParseCallback_t const &parse_callback);

public:
    String const & GetShortName  () const { return _short_name;  }
    String const & GetLongName   () const { return _long_name;   }
    String const & GetDescription() const { return _description; }

    Array<String> const & GetValues          () const { return _values;             }
    bool                  HasValues          () const { return !_values.IsEmpty();  }
    size_t                ValuesCount        () const { return _values.Count();     }
    MinMax_t const &      GetValueRequirement() const { return _values_requirement; }

    void SetAsFound()        { _found = true; }
    bool WasFound  () const  { return _found; }

public:
    ParseStatus
    ParseArgument(String const &value = String::Empty())
    {
        ParseStatus status = ParseStatus::Valid;

        if(_parse_callback) {
            status = _parse_callback(value);
        }
        if(status == ParseStatus::Valid && !value.IsEmptyOrWhitespace()) {
            _values.PushBack(value);
        }

        return status;
    }

private:
    String _short_name;
    String _long_name;
    String _description;

    Array<String>           _values;
    MinMax_t                _values_requirement;
    ArgumentParseCallback_t _parse_callback;

    bool _found = false;
}; // class Argument


class Parser
{
public:
    struct ErrorCodes
    {
        static i32 const INVALID_FLAG         = 1;
        static i32 const NOT_ENOUGH_ARGUMENTS = 2;
        static i32 const FAILED_ON_PARSE      = 3;
    };

    static String const ShortFlagPrefix;
    static String const LongFlagPrefix;
    static String const WindowsFlagPrefix;

    static char const LongFlagSeparator;
    static char const WindowsFlagSeparator;

    typedef Result<bool> ParseResult_t;

public:
    explicit Parser();
    explicit Parser(String const &cmd_line_str);
    explicit Parser(i32 argc, char const *argv[]);
    explicit Parser(Array<String> const & cmd_line_items);

public:
    void CreateArgument(
        String                            const &short_name,
        String                            const &long_name,
        String                            const &description,
        Argument::MinMax_t                const &values_requirements,
        Argument::ArgumentParseCallback_t const &parse_callback = nullptr);

    Argument const * FindArgumentByName(String const &name) const;
    Argument       * FindArgumentByName(String const &name) {
        Argument const * const arg = Cast<const Parser*>(this)->FindArgumentByName(name);
        return Cast<Argument *>(arg);
    }

    Array<Argument> const & GetArguments       () const { return _arguments;         }
    Array<String>   const & GetPositionalValues() const { return _positional_values; }

public:
    ParseResult_t Evaluate();

private:
    bool IsShortFlag(String const &item) const;
    bool IsLongFlag (String const &item) const;
    bool IsFlag     (String const &item) const { return IsShortFlag(item) || IsLongFlag(item); }

    Array<String> SplitShortFlag(String const &item) const;
    Array<String> SplitLongFlag (String const &item) const;

private:
    Array<Argument> _arguments;
    Array<String>   _positional_values;
    Array<String>   _cmd_line;
}; // class Parser


#define EAZ_ARG_FOUND_FUNC(_code_)                        \
    [](String const &value) {                             \
        _code_;                                           \
        return CommandLine::Argument::ParseStatus::Valid; \
    }

} // namespace CommandLine
} // namespace ark
