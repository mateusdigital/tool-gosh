#!/usr/bin/env python3
##~---------------------------------------------------------------------------##
##                        _      _                 _   _                      ##
##                    ___| |_ __| |_ __ ___   __ _| |_| |_                    ##
##                   / __| __/ _` | '_ ` _ \ / _` | __| __|                   ##
##                   \__ \ || (_| | | | | | | (_| | |_| |_                    ##
##                   |___/\__\__,_|_| |_| |_|\__,_|\__|\__|                   ##
##                                                                            ##
##  File      : gosh-core.py                                                  ##
##  Project   : gosh                                                          ##
##  Date      : Sep 28, 2015                                                  ##
##  License   : GPLv3                                                         ##
##  Author    : stdmatt <stdmatt@pixelwizards.io>                             ##
##  Copyright : stdmatt - 2015 - 2020                                         ##
##                                                                            ##
##  Description :                                                             ##
##                                                                            ##
##---------------------------------------------------------------------------~##


##----------------------------------------------------------------------------##
## Imports                                                                    ##
##----------------------------------------------------------------------------##
import os;
import os.path;
import sys;
import getopt;
import pdb;
import subprocess;
from difflib import SequenceMatcher as SM;
from mcow_py_termcolor import termcolor;

##----------------------------------------------------------------------------##
## Constants / Globals                                                        ##
##----------------------------------------------------------------------------##
PROGRAM_NAME      = "gosh";
PROGRAM_VERSION   = "1.0.0";
PROGRAM_COPYRIGHT = "2015 - 2020";

##------------------------------------------------------------------------------
class Constants:
    ##
    ## Where the bookmarks will be stored.
    PATH_DIR_RC  = os.path.expanduser("~/.stdmatt");
    PATH_FILE_RC = os.path.expanduser(os.path.join(PATH_DIR_RC, "goshrc.txt"));

    ##
    ## Some chars that are important to gosh.
    ## This char is used to pass the values back to gosh shell script.
    OUTPUT_META_CHAR   = "#";
    BOOKMARK_SEPARATOR = ":";

    ##
    ## Kind of getopt flags but fixed in positions.
    ACTION_HELP            = "gosh_opt_help";
    ACTION_VERSION         = "gosh_opt_version";
    ACTION_LIST            = "gosh_opt_list";
    ACTION_LIST_LONG       = "gosh_opt_list-long";
    ACTION_REMOVE          = "gosh_opt_remove";
    ACTION_ADD             = "gosh_opt_add";
    ACTION_UPDATE          = "gosh_opt_update";
    ACTION_PRINT           = "gosh_opt_print";
    ACTION_EXISTS_BOOKMARK = "gosh_opt_exists_bookmark";

    ##--------------------------------------------------------------------------
    ## OSes names
    ##  Thanks to ICB on StackOverflow
    ##      https://stackoverflow.com/a/13874620/5482197
    ## .---------------------.----------.
    ## | System              | Value    |
    ## |---------------------|----------|
    ## | Linux (2.x and 3.x) | linux2   |
    ## | Windows             | win32    |
    ## | Windows/Cygwin      | cygwin   |
    ## | Mac OS X            | darwin   |
    ## | OS/2                | os2      |
    ## | OS/2 EMX            | os2emx   |
    ## | RiscOS              | riscos   |
    ## | AtheOS              | atheos   |
    ## | FreeBSD 7           | freebsd7 |
    ## | FreeBSD 8           | freebsd8 |
    ## '---------------------'----------'
    OS_NAME_CYGWIN    = "cygwin";
    OS_NAME_WINDOWS   = "win32";
    OS_NAME_GNU_LINUX = "linux";
    OS_NAME_NT        = "win32";
    OS_NAME_OSX       = "darwin";
    OS_NAME_BSD       = "bsd";

##------------------------------------------------------------------------------
class Globals:
    bookmarks     = {};    ## Our bookmarks dictionary.
    opt_no_colors = False; ## If user wants color or not.


##----------------------------------------------------------------------------##
## Colored Class                                                              ##
##----------------------------------------------------------------------------##
##------------------------------------------------------------------------------
class C:
    @staticmethod
    def red(msg):
        return C._colored(msg, termcolor.RED);

    @staticmethod
    def blue(msg):
        return C._colored(msg, termcolor.BLUE);

    @staticmethod
    def magenta(msg):
        return C._colored(msg, termcolor.MAGENTA);

    @staticmethod
    def _colored(msg, color):
        if(not Globals.opt_no_colors):
            return termcolor.colored(msg, color);
        return msg;


##----------------------------------------------------------------------------##
## Read / Write Functions                                                     ##
##----------------------------------------------------------------------------##
##------------------------------------------------------------------------------
def ensure_rc_file():
    ##
    ## This will ensure that the RC path and file exists.
    if(not os.path.isdir(Constants.PATH_DIR_RC)):
        os.makedirs(Constants.PATH_DIR_RC);

    if(not os.path.isfile(Constants.PATH_FILE_RC)):
        open(Constants.PATH_FILE_RC, "w").close();

##------------------------------------------------------------------------------
def read_bookmarks():
    ##
    ## Ensure that rc file exists...
    ensure_rc_file();

    ##
    ## Open the filename and read all bookmarks that are in format of:
    ##    BookmarkName : BookmarkSeparator (Note that the ':' is the separator)
    try:
        bookmarks_file = open(Constants.PATH_FILE_RC);

        for bookmark in bookmarks_file.readlines():
            bookmark   = bookmark.replace("\n", "");
            name, path = bookmark.split(Constants.BOOKMARK_SEPARATOR);

            ##
            ## Trim all white spaces.
            name = name.replace(" ", "");
            path = path.lstrip().rstrip();

            Globals.bookmarks[name] = path;

    except Exception as e:
        ##
        ## Failed to unpack, this is because the bookmarks aren't in form of
        ##    Name SEPARATOR Path.
        ## So state it to user, so he could correct manually.
        help_msg = "{0} {1} {2} {3}".format(
            "Check if all values are in form of: ",
            C.blue   ("BookmarkName"),
            C.magenta(Constants.BOOKMARK_SEPARATOR),
            C.blue   ("BookmarkPath")
        );

        msg = "{0} ({1})\n{2}".format(
            "Bookmarks file is corrupted.",
            C.blue(Constants.PATH_FILE_RC),
            help_msg
        );

        print_fatal(msg);

    finally:
        bookmarks_file.close();

##------------------------------------------------------------------------------
def write_bookmarks():
    ##
    ## Save the bookmarks in disk. Sort them before just as convenience for
    ## who wants to mess with them in an editor.
    bookmarks_str = "";
    for key in sorted(Globals.bookmarks.keys()):
        bookmarks_str += "{0} {1} {2}\n".format(
            key,
            Constants.BOOKMARK_SEPARATOR,
            Globals.bookmarks[key]
        );

    ##
    ## Write and close.
    ensure_rc_file();
    try:
        bookmarks_file = open(Constants.PATH_FILE_RC, "w");
        bookmarks_file.write(bookmarks_str);

    except Exception as e:
        print_fatal("Error while writing file. {0}".format(str(e)));

    finally:
        bookmarks_file.close();


##----------------------------------------------------------------------------##
## Helper Functions                                                           ##
##----------------------------------------------------------------------------##
##------------------------------------------------------------------------------
def bookmark_exists(name):
    read_bookmarks();
    return None if name is None else name in Globals.bookmarks.keys();

##------------------------------------------------------------------------------
def name_for_fuzzy_name(fuzzy_name):
    best_score = 0;
    best_name  = None;

    for k in Globals.bookmarks.keys():
        score = SM(None, k, fuzzy_name).ratio();
        ## COWNOTE(n2omatt): For debug only...
        # print "Name  : ", fuzzy_name;
        # print "Key   : ", k;
        # print "Score : ", score;
        # print "BScore: ", best_score;
        # print "--";

        if(score > best_score):
            best_name  = k;
            best_score = score;

        ##
        ## Best match... couldn't be better.
        if(best_score == 1):
            return k;

    return best_name;

##------------------------------------------------------------------------------
def path_for_bookmark(name):
    read_bookmarks();
    return Globals.bookmarks[name];

##------------------------------------------------------------------------------
def bookmark_for_path(path):
    if(path is None or len(path) == 0):
        path = ".";

    read_bookmarks();
    abs_path = canonize_path(path);

    for bookmark_name in Globals.bookmarks.keys():
        bookmark_path = Globals.bookmarks[bookmark_name];
        bookmark_path = canonize_path(bookmark_path);

        if(bookmark_path == abs_path):
            return bookmark_name;

    return None;

##------------------------------------------------------------------------------
def ensure_valid_bookmark_name_or_die(name):
    ##
    ## Check if name isn't empty...
    if(name is None or len(name) == 0):
        print_fatal("Missing arguments - name");

    ##
    ## Check if this name is a valid name.
    if(((Constants.OUTPUT_META_CHAR   in name) or
        (Constants.BOOKMARK_SEPARATOR in name))):
        print_fatal(
            "{0} ('{1}', '{2}') chars.".format(
                "Bookmark name cannot contains",
                Constants.OUTPUT_META_CHAR,
                Constants.BOOKMARK_SEPARATOR
            )
        );

##------------------------------------------------------------------------------
def ensure_valid_path_or_die(path):
    path = canonize_path(path);
    if(not os.path.isdir(path)):
        print_fatal("Path ({0}) is not a valid directory.".format(C.magenta(path)));

##------------------------------------------------------------------------------
def ensure_bookmark_existence_or_die(name, bookmark_shall_exists):
    if(bookmark_exists(name) and bookmark_shall_exists == False):
        print_fatal("Bookmark ({0}) already exists.".format(C.blue(name)));

    if(not bookmark_exists(name) and bookmark_shall_exists == True):
        print_fatal("Bookmark ({0}) doesn't exists.".format(C.blue(name)));

##------------------------------------------------------------------------------
def canonize_path(path):
    path = path.lstrip().rstrip();
    path = os.path.realpath(os.path.expanduser(path));

    return path;

##------------------------------------------------------------------------------
def get_os_name():
    name = sys.platform;

    if  (Constants.OS_NAME_CYGWIN    in name): return Constants.OS_NAME_CYGWIN;
    if  (Constants.OS_NAME_WINDOWS   in name): return Constants.OS_NAME_WINDOWS;
    elif(Constants.OS_NAME_GNU_LINUX in name): return Constants.OS_NAME_GNU_LINUX;
    elif(Constants.OS_NAME_OSX       in name): return Constants.OS_NAME_OSX;
    elif(Constants.OS_NAME_BSD       in name): return Constants.OS_NAME_BSD;
    else:
        raise NotImplementedError;

##------------------------------------------------------------------------------
def make_relative_path(path):
    path = path.lstrip().rstrip();

    if(get_os_name() == Constants.OS_NAME_CYGWIN):
        home_path = _get_home_path_for_cygwin(path);
    else:
        home_path = canonize_path("~");

    rel_path = "~/" + os.path.relpath(path, home_path);

    return rel_path;

##------------------------------------------------------------------------------
def remove_enclosing_quotes(value):
    return value.strip("'");


##----------------------------------------------------------------------------##
## Helper functions (Cygwin)                                                  ##
##----------------------------------------------------------------------------##
def _run_process(cmd):
    process = subprocess.Popen(
        [cmd],
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,
        shell=True
    );

    return process.stdout.read().decode("UTF-8").replace("\n", "");

##------------------------------------------------------------------------------
def _get_home_path_for_cygwin(path):
    ##--------------------------------------------------------------------------
    ## COWNOTE(n2omatt): This is needed because in one configuration of
    ##   the home folder in cygwin makes the path looks wrong.
    ##   The situation is when the user set the its home folder to be a
    ##   symbolic link to a folder on NT.
    ##   For example if the user makes the home folder be:
    ##      C:/Users/USERNAME
    ##   The realpath would be /cygdrive/c/Users/Username but yet the
    ##   unix tools would see the home folder as:
    ##      /home/USERNAME
    ##   This way the os.path.relpath doesn't works as expected
    ##   making a bookmark of the path:
    ##      $HOME/Documents/Projects/N2OMatt/dots
    ##   Be seen as:
    ##      ~/../../cygdrive/c/Users/n2omatt/Documents/Projects/N2OMatt/dots
    ##   Instead of:
    ##      ~/Documents/Projects/N2OMatt/dots
    ##
    ##   So this function get's the REAL home path taking in account the
    ##   fact that the path might be on NT "field".
    home_path = os.path.expanduser("~");

    nt_home = _run_process("cygpath -w {0}".format(home_path));
    nt_home = nt_home.replace("\\", "/");

    unix_home = _run_process("cygpath -u {0}".format(nt_home));
    return canonize_path(unix_home);


##----------------------------------------------------------------------------##
## Print Functions                                                            ##
##----------------------------------------------------------------------------##
##------------------------------------------------------------------------------
def print_fatal(msg):
    print("{0} {1}".format(C.red("[FATAL]"), msg));
    exit(1);

##------------------------------------------------------------------------------
def print_help():
    print("""Usage:
  gosh                        (Same as gosh -l)
  gosh <name>                 (To change the directory)
  gosh -h | -v                (Show help | version)
  gosh -l | -L                (Show list of bookmarks)
  gosh -p <name>              (Show path for bookmark)
  gosh -e <path>              (Show bookmark for path)
  gosh -a | -u <name> <path>  (Add | Update bookmark)
  gosh -r <name>              (Remove the bookmark)

Options:
  *-h --help     : Show this screen.
  *-v --version  : Show app version and copyright.

  *-e --exists <path>  : Print the Bookmark for path.
  *-p --print  <name>  : Print the path of Bookmark.

  *-l --list       : Show all Bookmarks (no Paths).
  *-L --list-long  : Show all Bookmarks and Paths.

  *-a --add    <name> <path>  : Add a Bookmark with specified path.
  *-r --remove <name>         : Remove a Bookmark.
  *-u --update <name> <path>  : Update a Bookmark to path.

  -n --no-colors : Print the output without colors.

Notes:
  If <path> is blank the current dir is assumed.

  Options marked with * are exclusive, i.e. the gosh will run that
  and exit after the operation.
""");
    exit(0);

##------------------------------------------------------------------------------
def print_version():
    msg = "\n".join([
        "{program_name} - {program_version} - stdmatt <stdmatt@pixelwizards.io>",
        "Copyright (c) {program_copyright} - stdmatt",
        "This is a free software (GPLv3) - Share/Hack it",
        "Check http://stdmatt.com for more :)"]);

    msg = msg.format(
        program_name=PROGRAM_NAME,
        program_version=PROGRAM_VERSION,
        program_copyright=PROGRAM_COPYRIGHT
    );

    print(msg);
    exit(0);


##----------------------------------------------------------------------------##
## Action Functions                                                           ##
##----------------------------------------------------------------------------##
##------------------------------------------------------------------------------
def action_list_bookmarks(long = False):
    read_bookmarks();

    if(len(Globals.bookmarks) == 0):
        print("No bookmarks yet... :/");
        exit(0);

    ##
    ## Get the greater bookmark's name length. It will
    ## be used to align all the bookmark's name.
    max_len = max(map(len, Globals.bookmarks.keys()));
    for key in sorted(Globals.bookmarks.keys()):
        spaces = " " * (max_len - len(key)); #Put spaces to align the names.
        path   = Globals.bookmarks[key];

        if(long):
            print("{_key}{_spaces} : {_path}".format(
                _key    = C.blue(key),
                _spaces = spaces,
                _path   = C.magenta(path)
            ));
        else:
            print(C.blue(key));

    exit(0);

##------------------------------------------------------------------------------
def action_add_bookmark(name, path):
    if(path is None or len(path) == 0):
        path = ".";

    if((name is None or len(name) == 0) and path == "."):
        name = os.path.basename(canonize_path("."));

    #Must be valid name.
    ## ensure_valid_bookmark_name_or_die(name);

    ## Load from file.
    read_bookmarks();

    ## Check if we have this bookmark, since we are adding we cannot have it.
    ensure_bookmark_existence_or_die(name, bookmark_shall_exists=False);

    ## Check if path is valid path.
    added_path = make_relative_path(path);
    ensure_valid_path_or_die(added_path);

    ## Name and Path are valid... Add it and inform the user.
    Globals.bookmarks[name] = added_path;
    msg = "Bookmark added:\n  ({0}) - ({1})".format(
        C.blue   (name),
        C.magenta(added_path)
    );
    print(msg);

    write_bookmarks(); #Save to file
    exit(0);

##------------------------------------------------------------------------------
def action_remove_bookmark(name):
    ## Load from file.
    read_bookmarks();
    name = name_for_fuzzy_name(name);

    ## Must be valid name.
    ensure_valid_bookmark_name_or_die(name);

    ## Check if we actually have a bookmark with this name.
    ensure_bookmark_existence_or_die(name, bookmark_shall_exists=True);

    ## Bookmark exists... Remove it and inform the user.
    del Globals.bookmarks[name];
    print("Bookmark removed:\n  ({0})".format(C.blue(name)));

    write_bookmarks(); #Save to file
    exit(0);

##------------------------------------------------------------------------------
def action_update_bookmark(name, path):
    if(path is None or len(path) == 0):
        path = ".";

    name = name_for_fuzzy_name(name);

    ## Must be valid name.
    ensure_valid_bookmark_name_or_die(name);

    ## Load from file.
    read_bookmarks();

    ## Check if we have this bookmark, since we are updating we must have it.
    ensure_bookmark_existence_or_die(name, bookmark_shall_exists=True);

    ## Check if path is valid path.
    updated_path = make_relative_path(path);
    ensure_valid_path_or_die(updated_path);

    ## Bookmark exists and path is valid... Update it and inform the user.
    Globals.bookmarks[name] = updated_path;
    msg = "Bookmark updated:\n  ({0}) - ({1})".format(
        C.blue   (name),
        C.magenta(updated_path)
    );
    print(msg);

    write_bookmarks(); #Save to file
    exit(0);

##------------------------------------------------------------------------------
def action_bookmark_exists(path):
    ## Load from file.
    read_bookmarks();

    bookmark_name = bookmark_for_path(path);
    if(bookmark_name is None):
        print("No bookmark");
        exit(1);
    else:
        print("Bookmark: ({0})".format(C.blue(bookmark_name)));
        exit(0);

##------------------------------------------------------------------------------
def action_print_bookmark(name):
    ## Load from file.
    read_bookmarks();

    if(len(name) == 0):
        print_fatal("Missing args - name.");

    name = name_for_fuzzy_name(name);

    if(not bookmark_exists(name)):
        msg = "Bookmark ({0}) doesn't exists.".format(C.blue(name));
        print(msg);
        exit(1);

    ## Bookmark exists, check if path is valid.
    bookmark_path = path_for_bookmark(name);
    bookmark_path = canonize_path(bookmark_path);

    if(not os.path.isdir(bookmark_path)):
        msg = "Bookmark ({0}) {1} ({2})".format(
            C.blue(name),
            "exists but it's path is invalid.",
            C.magenta(bookmark_path)
        );
        print(msg);
        exit(1);

    ## Bookmark and path are valid.
    ## Print the path to gosh shell script change the directory.
    print(bookmark_path);
    exit(0);


##----------------------------------------------------------------------------##
## Script Initialization                                                      ##
##----------------------------------------------------------------------------##
def main():
    ##
    ## gosh will pass as last parameter with user want color or not.
    ## So we grab this information and remove them from list because
    ## The other options will use the length of the list as a way to check
    ## if the arguments are ok.
    args = sys.argv[1:]
    if(args[-1] == "no-colors"):
        Globals.opt_no_colors = True;
        args.pop();

    ##
    ## gosh.sh is using getopt(1) and it passes the arguments inside a pair
    ## of single quotes - This method will remove them.
    first_arg  = remove_enclosing_quotes(args[0]);
    second_arg = remove_enclosing_quotes(args[1]) if len(args) > 1 else "";
    third_arg  = remove_enclosing_quotes(args[2]) if len(args) > 2 else "";

    ##
    ## All the command line options are exclusive operations. i.e
    ## they will run the requested command and exit after it.
    ## Help / Version.
    if(Constants.ACTION_HELP    == first_arg): print_help();
    if(Constants.ACTION_VERSION == first_arg): print_version();

    ##
    ## List.
    if(Constants.ACTION_LIST      == first_arg): action_list_bookmarks();
    if(Constants.ACTION_LIST_LONG == first_arg): action_list_bookmarks(long=True);

    ##
    ## Add
    if(Constants.ACTION_ADD == first_arg):
        action_add_bookmark (second_arg, third_arg);
    ##
    ## Remove
    if(Constants.ACTION_REMOVE == first_arg):
        action_remove_bookmark(second_arg);
    ##
    ## Update
    if(Constants.ACTION_UPDATE == first_arg):
        action_update_bookmark(second_arg, third_arg);
    ##
    ## Exists
    if(Constants.ACTION_EXISTS_BOOKMARK == first_arg):
        action_bookmark_exists(second_arg);
    ##
    ## Print
    if(Constants.ACTION_PRINT == first_arg):
        action_print_bookmark(second_arg);


if(__name__ == "__main__"):
    #If any error occurs in main, means that user is trying to use
    #the gosh-core instead of gosh. Since gosh always pass the parameters
    #even user didn't. So inform the user that the correct is use gosh.
    # try:
        main();
    # except Exception, e:
    #     # print_fatal("You should use gosh not gosh-core. (Exception: ({0}))".format(e));
    #     raise e;
