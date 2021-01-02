// Header
#include "Terminal.hpp"

// Usings
using namespace ark;

namespace Private {
    Terminal::OutputMode _curr_output_mode = Terminal::OutputMode::OnlyIfTerminal;
} // namespace Private


//
//
//
void       
Terminal::SetOutputMode(OutputMode const mode)
{
    Private::_curr_output_mode = mode;
}

Terminal::OutputMode 
Terminal::GetOutputMode()
{
    return Private::_curr_output_mode;
}

bool 
Terminal::IsConnectedToTTY()
{
    // @todo(stdmatt): Check if stdout is a tty - Dec 28, 2020
    return false;
}


//
//
//
// Color
// @todo(stdmatt): Find the correct values for color for each platform - Dec 28, 2020
Terminal::Color const Terminal::Color::None = Terminal::Color(0);

Terminal::Color const Terminal::Color::Black = Terminal::Color(1);
Terminal::Color const Terminal::Color::White = Terminal::Color(2);

Terminal::Color const Terminal::Color::Red   = Terminal::Color(1);
Terminal::Color const Terminal::Color::Blue  = Terminal::Color(1);
Terminal::Color const Terminal::Color::Green = Terminal::Color(1);

Terminal::Color const Terminal::Color::Cyan    = Terminal::Color(1);
Terminal::Color const Terminal::Color::Magenta = Terminal::Color(1);
Terminal::Color const Terminal::Color::Yellow  = Terminal::Color(1);

// Attributes
// @todo(stdmatt): Find the correct values for attributes for each platform - Dec 28, 2020
Terminal::Attributes const Terminal::Attributes::Empty = Terminal::Attributes(0);

String 
Terminal::Colored(
    String     const &str,
    Color      const fg_color /* = Color::None       */,
    Color      const bg_color /* = Color::None       */,
    Attributes const attr     /* = Attributes::Empty */)
{
    OutputMode const mode = GetOutputMode();
    if (mode == OutputMode::Never) {
        return str;
    }
    // @todo(stdmatt): Add the color output... Dec 28, 2020
    else if (mode == OutputMode::OnlyIfTerminal && IsConnectedToTTY()) {
        return str;
    }

    return str;
}
