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
##  Date      : Aug 12, 2015                                                  ##
##  License   : GPLv3                                                         ##
##  Author    : stdmatt <stdmatt@pixelwizards.io>                             ##
##  Copyright : stdmatt - 2015 - 2019                                         ##
##                                                                            ##
##  Description :                                                             ##
##                                                                            ##
##---------------------------------------------------------------------------~##

$SCRIPT_FULLPATH = $MyInvocation.MyCommand.Path;
$SCRIPT_DIR      = Split-Path "$SCRIPT_FULLPATH" -Parent;
$GOSH_EXE        = "$SCRIPT_DIR/gosh/gosh-core.py";


##----------------------------------------------------------------------------##
## Gosh                                                                       ##
##----------------------------------------------------------------------------##
##
## No args, just list the bookmarks.
if($args.Count -eq 0) {
    & python3 $GOSH_EXE --help;
    return;
}

## Concat all the argumetn to make a string.
## Then search for the string for any flags.
## If we find this flags means that gosh is doing an action
## and we don't need to care about changing the directory.
## Otherwise we need to capture the output of it and make the
## poweshell to cd to that dir ;D
$cmd_line=" "
for(($i = 0); ($i -lt $args.Count); (++$i)) {
    $cmd_line += $args[$i].ToString();
}

$has_short_flags = $cmd_line.Contains(" -");
$has_long_flags  = $cmd_line.Contains(" --");

## Making an action...
if(($has_short_flags) -or ($has_long_flags)) {
    & python3 $GOSH_EXE $args;
}
## Changing directory...
else {
    $path = (& python3 $GOSH_EXE $args);
    cd $path;
}
