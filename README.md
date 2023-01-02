# gosh

**Made with <3 by [mateus-earth](http://mateus-earth).**

## Description:

```gosh``` is a simple bash bookmark utility, enabling the user to navigate
thru the paths with more convienience. It's created to reduce the stress of
transverse manually very long paths or places that is often refered to.

Example:

As usual, you are **very welcomed** to **share** and **hack** it and I'd love to hear **feedback** if you have it :).

## Usage:
```
Usage:
  gosh                   (Same as gosh -l)
  gosh <name>            (To change the directory)
  gosh -h | -v           (Show help | version)
  gosh -l | -L           (Show list of bookmarks)
  gosh -p <name>         (Show path for bookmark)
  gosh -e <path>         (Show bookmark for path)
  gosh -a <name> <path>  (Add bookmark)
  gosh -d <name>         (Delete the bookmark)

Options:
  *-h --help     : Show this screen.
  *-v --version  : Show app version and copyright.

  *-e --exists <path>  : Print the Bookmark for path.
  *-p --print  <name>  : Print the path of Bookmark.

  *-l --list       : Show all Bookmarks (no Paths).
  *-L --list-long  : Show all Bookmarks and Paths.

  *-a --add    <name> <path>  : Add a Bookmark with specified path.
  *-r --delete <name>         : Delete a Bookmark.

Notes:
  If <path> is blank the current directory is assumed.

  Options marked with * are exclusive, i.e. the gosh will run that
  and exit after the operation.
```

## Dependencies:

- python3
- bash

## Installation:

- Run: ```install.sh``` as normal user.



## License:

This software is released under [GPLv3](https://www.gnu.org/licenses/gpl-3.0.en.html).


## Others:

There's more FLOSS things at [https://mateus.earth](https://mateus.earth) :)
