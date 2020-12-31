#pragma once

//
#include "BasicTypes.hpp"
#include "String.hpp"


namespace Terminal {

//
//
//
enum class OutputMode {
    Always, 
    Never, 
    OnlyIfTerminal
}; // enum OutputMode

void       SetOutputMode(OutputMode const mode);
OutputMode GetOutputMode(); 

bool IsConnectedToTTY();


//
//
//
struct Color
{
    static Color const None;

    static Color const Black;
    static Color const White;

    static Color const Red;
    static Color const Blue;
    static Color const Green;

    static Color const Cyan;
    static Color const Magenta;
    static Color const Yellow;
    
    constexpr explicit Color(u32 const v)
        : value(v)
    {
        // Empty...
    }

    u32 value;
}; // struct Color

struct Attributes
{
    static Attributes const Empty;

    constexpr explicit Attributes(u32 const v)
        : value(v)
    {
        // Empty...
    }

    u32 value;
}; // struct Attributes

String Colored(
    String     const &str,
    Color      const fg_color = Color::None,
    Color      const bg_color = Color::None,
    Attributes const attr     = Attributes::Empty);


}; // namespace Terminal
