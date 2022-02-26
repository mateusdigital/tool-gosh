#!/usr/bin/env python3
##~---------------------------------------------------------------------------##
##                        _      _                 _   _                      ##
##                    ___| |_ __| |_ __ ___   __ _| |_| |_                    ##
##                   / __| __/ _` | '_ ` _ \ / _` | __| __|                   ##
##                   \__ \ || (_| | | | | | | (_| | |_| |_                    ##
##                   |___/\__\__,_|_| |_| |_|\__,_|\__|\__|                   ##
##                                                                            ##
##  File      : gosh2.py                                                      ##
##  Project   : gosh                                                          ##
##  Date      : Sep 28, 2015 - gosh-core.py                                   ##
##              Feb 25, 2022 - gosh2.py                                       ##
##  License   : GPLv3                                                         ##
##  Author    : stdmatt <stdmatt@pixelwizards.io>                             ##
##  Copyright : stdmatt - 2015 - 2022                                         ##
##                                                                            ##
##  Description :                                                             ##
##                                                                            ##
##---------------------------------------------------------------------------~##

##----------------------------------------------------------------------------##
## Imports                                                                    ##
##----------------------------------------------------------------------------##
import argparse
import os;
import os.path;
import sys;
import getopt;
import pdb;
import subprocess;
from difflib import SequenceMatcher as SM;


##----------------------------------------------------------------------------##
## Constants / Globals                                                        ##
##----------------------------------------------------------------------------##
##------------------------------------------------------------------------------
PROGRAM_NAME      = "gosh";
PROGRAM_VERSION   = "3.0.0";
PROGRAM_COPYRIGHT = "2015 - 2022";
##------------------------------------------------------------------------------
## Location of the paths file.
BOOKMARKS_FILE_DIR  = os.path.expanduser("~/.stdmatt/config");
BOOKMARKS_FILE_PATH = os.path.join(BOOKMARKS_FILE_DIR, "gosh-paths.txt");
##------------------------------------------------------------------------------
## Some chars that are important to gosh.
## This char is used to pass the values back to gosh shell script.
OUTPUT_META_CHAR   = "#";
BOOKMARK_SEPARATOR = ";";

##----------------------------------------------------------------------------##
## Helper Functions                                                           ##
##----------------------------------------------------------------------------##
##------------------------------------------------------------------------------
def name_for_fuzzy_name(fuzzy_name):
    best_score = 0;
    best_name  = None;

    for k in bookmarks.keys():
        score = SM(None, k, fuzzy_name).ratio();
        if(score > best_score):
            best_name  = k;
            best_score = score;
        if(best_score == 1):
            return k;

    return best_name;

##------------------------------------------------------------------------------
def ensure_valid_bookmark_name_or_die(name):
    ##
    ## Check if name isn't empty...
    if(name is None or len(name) == 0):
        print_fatal("Missing arguments - name");

    ##
    ## Check if this name is a valid name.
    if(((OUTPUT_META_CHAR   in name) or
        (BOOKMARK_SEPARATOR in name))):
        print_fatal("Bookmark name cannot contains, ('{1}', '{2}') chars.".format(
            OUTPUT_META_CHAR, BOOKMARK_SEPARATOR
        ));

##------------------------------------------------------------------------------
def canonize_path(path):
    path = path.lstrip().rstrip();
    path = os.path.normpath(os.path.normcase(os.path.abspath(os.path.expanduser(path))));
    return path.replace("\\", "/");


##----------------------------------------------------------------------------##
## Print Functions                                                            ##
##----------------------------------------------------------------------------##
##------------------------------------------------------------------------------
def print_fatal(msg):
    print("{0} {1}".format("[FATAL]", msg));
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


##----------------------------------------------------------------------------##
## Script                                                                     ##
##----------------------------------------------------------------------------##
##
## Parse the Command Line
parser = argparse.ArgumentParser(add_help=False);

parser.add_argument("-h", "--help"   ,   dest=None       ,            action="store_true");
parser.add_argument("-v", "--version",   dest=None       ,            action="store_true");
parser.add_argument("-e", "--exists" ,   dest="exists"   , nargs=1  , action="store");
parser.add_argument("-p", "--print"  ,   dest="print"    , nargs=1  , action="store");
parser.add_argument("-l", "--list"   ,   dest=None       ,            action="store_true");
parser.add_argument("-L", "--list-long", dest=None       ,            action="store_true");
parser.add_argument("-a", "--add"    ,   dest="add"      , nargs="*", action="store");
parser.add_argument("-r", "--remove" ,   dest="remove"   , nargs=1  , action="store");
parser.add_argument("-u", "--update" ,   dest="update"   , nargs=2  , action="store");

parser.add_argument("values", nargs="*"); ## Positional Values
args = parser.parse_args();

##
## Help and Version doesn't need anything...
if(args.help):
    print_help();
    exit(0);
elif(args.version):
    print_version();
    exit(0);

##
##
bookmarks             = {} ## Name : Path dictionary
bookmarks_file        = None;
something_was_changed = False;


##
## This will ensure that the bookmarks file exists.
if(not os.path.isdir(BOOKMARKS_FILE_DIR)):
    os.makedirs(BOOKMARKS_FILE_DIR);
if(not os.path.isfile(BOOKMARKS_FILE_PATH)):
    open(BOOKMARKS_FILE_PATH, "w").close();

##
## Open the filename and read all bookmarks that are in format of:
##    BookmarkName : BookmarkSeparator (Note that the ':' is the separator)
bookmarks_file = open(BOOKMARKS_FILE_PATH, "w+");
for line in bookmarks_file.readlines():
    bookmark   = bookmark.replace("\n", "");
    name, path = bookmark.split(BOOKMARK_SEPARATOR);

    ## Trim all white spaces.
    name = name.replace(" ", "");
    path = path.lstrip().rstrip();

    bookmarks[name] = path;

##
## Exists
if(args.exists):
    if(path is None or len(path) == 0):
        path = ".";

    bookmark_name = None;
    clean_path    = canonize_path(path);

    for name in bookmarks.keys():
        path = bookmarks[name];
        path = canonize_path(path);
        if(path == clean_path):
            bookmark_name = name;
            break;

    if(bookmark_name is None):
        print("No bookmark");
    else:
        print("Bookmark: ({0})".format(bookmark_name));

##
## Print
elif(args.print):
    if(len(name) == 0):
        print_fatal("Missing args - name.");

    clean_name = name_for_fuzzy_name(name);
    if(clean_name not in bookmarks.keys()):
        print("Bookmark ({0}) doesn't exists.".format(name));
        exit(1);

    ## Bookmark exists, check if path is valid.
    path       = bookmarks[clean_name];
    clean_path = canonize_path(path);

    if(not os.path.isdir(clean_path)):
        print("Bookmark ({0}) exists but it's path is invalid ({2})".format(
            clean_name, clean_path
        ));
    else:
        print(clean_path);

##
## List
elif(args.list or args.list_long):
    if(len(bookmarks) == 0):
        print("No bookmarks yet... :/");

    ## Get the greater bookmark's name length. It will
    ## be used to align all the bookmark's name.
    else:
        max_len = max(map(len, bookmarks.keys()));
        for key in sorted(bookmarks.keys()):
            spaces = " " * (max_len - len(key)); ## Put spaces to align the names.
            path   = bookmarks[key];

            if(args.list_long):
                print("{0}{1} : {2}".format(key, spaces, path));
            else:
                print(key);

##
## Remove
elif(args.remove):
    clean_name = name_for_fuzzy_name(name);

    ## Must be valid name.
    ensure_valid_bookmark_name_or_die(name);

    ## Check if we actually have a bookmark with this name.
    if(name not in bookmarks.keys()):
        print_fatal("Bookmark ({0}) doesn't exists.".format(name));

    ## Bookmark exists... Remove it and inform the user.
    del bookmarks[name];
    print("Bookmark removed:\n  ({0})".format(name));

##
## Add
##   args.add can be called without any argument, meaning that we want
##   to add the current path with the current base name as bookmark
##   so we need to compare it agaisnt None otherwise we can't capture
##   the case when we do gosh -a
elif(args.add is not None):
    if(name == "." and path == "."):
        name = os.path.basename(canonize_path("."));

    ## Check if we have this bookmark, since we are adding we cannot have it.
    if(name in bookmarks.keys()):
        print_fatal("Bookmark ({0}) already exists.".format(name));


    ## Check if path is valid path.
    added_path = canonize_path(path);
    if(not os.path.isdir(added_path)):
        print_fatal("Path ({0}) is not a valid directory.".format(added_path));

    ## Name and Path are valid... Add it and inform the user.
    bookmarks[name] = added_path;
    print("Bookmark added:\n  ({0}) - ({1})".format(name,added_path));

elif(args.values):
    action_print_bookmark(*args.values);


##
## Save the bookmarks in disk. Sort them before just as convenience for
## who wants to mess with them in an editor.
if(something_was_changed):
    bookmarks_str = "";
    for key in sorted(bookmarks.keys()):
        bookmarks_str += "{0} {1} {2}\n".format(
            key, BOOKMARK_SEPARATOR, bookmarks[key]
        );
    bookmarks_file.write(bookmarks_str);
