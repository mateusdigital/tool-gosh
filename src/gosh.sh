#!/usr/bin/env bash
##~---------------------------------------------------------------------------##
##                        _      _                 _   _                      ##
##                    ___| |_ __| |_ __ ___   __ _| |_| |_                    ##
##                   / __| __/ _` | '_ ` _ \ / _` | __| __|                   ##
##                   \__ \ || (_| | | | | | | (_| | |_| |_                    ##
##                   |___/\__\__,_|_| |_| |_|\__,_|\__|\__|                   ##
##                                                                            ##
##  File      : gosh.sh                                                       ##
##  Project   : gosh                                                          ##
##  Date      : Jul 15, 2021                                                  ##
##  License   : GPLv3                                                         ##
##  Author    : stdmatt <stdmatt@pixelwizards.io>                             ##
##  Copyright : stdmatt - 2015 - 2021                                         ##
##                                                                            ##
##  Description :                                                             ##
##                                                                            ##
##---------------------------------------------------------------------------~##

_GOSH_SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"
_GOSH_EXE="$_GOSH_SCRIPT_DIR/gosh-core.py";

##----------------------------------------------------------------------------##
## Gosh                                                                       ##
##----------------------------------------------------------------------------##
function gosh()
{
    ##
    ## No args, just list the bookmarks.
    if [ $# == 0 ]; then
        python3 $_GOSH_EXE --help;
        return;
    fi

    ## Concat all the arguments to make a string.
    ## Then search for the string for any flags.
    ## If we find this flags means that gosh is doing an action
    ## and we don't need to care about changing the directory.
    ## Otherwise we need to capture the output of it and make the
    ## poweshell to cd to that dir ;D
    cmd_line="$@";
    has_flags=$(echo "$cmd_line" | grep "-"); ## @XXX(stdmatt): Find a way to check if we have flags... Jul 15, 21

    ## Making an action...
    if [ -n "$has_flags" ]; then
        python3 $_GOSH_EXE $@;
    else
        path=$(python3 $_GOSH_EXE $@);
        if [ -d "$path" ]; then
            echo "gosh: $path";
            cd $path
        else
            echo $path;
        fi;
    fi;
}
