//
#include "String.hpp"


namespace Terminal {

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

};

struct Attributes
{
    static Attributes const & Empty;
};


String Colored(
    String     const &str,
    Color      const fg_color = Color::None,
    Color      const bg_color = Color::None,
    Attributes const attr     = Attributes::Empty);

}; // namespace Terminal
