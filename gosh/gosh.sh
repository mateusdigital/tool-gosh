#!/usr/bin/env bash
##----------------------------------------------------------------------------##
##                                  ⢀⣀⣀⡀⠒⠒⠦⣄⡀                                 ##
##                            ⠀⢀⣤⣶⡾⠿⠿⠿⠿⣿⣿⣶⣦⣄⠙⠷⣤⡀                              ##
##                          ⠀⣠⡾⠛⠉      ⠀⠈⠙⠻⣿⣷⣄⠘⢿⡄                             ##
##                        ⠀⢀⡾⠋        ⠐⠂⠠⢄⡀⠈⢿⣿⣧⠈⢿⡄                            ##
##                        ⢀⠏  ⠀⢀⠄⣀⣴⣾⠿⠛⠛⠛⠷⣦⡙⢦⠀⢻⣿⡆⠘⡇                            ##
##                            ⡐⢁⣴⡿⠋⢀⠠⣠⠤⠒⠲⡜⣧⢸⠄⢸⣿⡇⠀⡇                            ##
##                          ⠀⡼⠀⣾⡿⠁⣠⢃⡞⢁⢔⣆⠔⣰⠏⡼⠀⣸⣿⠃⢸⠃                            ##
##                          ⢰⡇⢸⣿⡇⠀⡇⢸⡇⣇⣀⣠⠔⠫⠊⠀⣰⣿⠏⡠⠃  ⢀                          ##
##                          ⢸⡇⠸⣿⣷⠀⢳⡈⢿⣦⣀⣀⣀⣠⣴⣾⠟⠁    ⢀⡎                          ##
##                          ⠘⣷⠀⢻⣿⣧⠀⠙⠢⠌⢉⣛⠛⠋⠉      ⣠⠎⠀                          ##
##                          ⠀⠹⣧⡀⠻⣿⣷⣄⡀        ⠀⢀⣠⡾⠃                            ##
##                            ⠈⠻⣤⡈⠻⢿⣿⣷⣦⣤⣤⣤⣤⣤⣴⡾⠛⠉                              ##
##                              ⠈⠙⠶⢤⣈⣉⠛⠛⠛⠛⠋⠉                                  ##
##                                    ⠉⠉⠉⠁                                    ##
##                                                                            ##
##  File      : install.sh                                                    ##
##  Project   : gosh                                                          ##
##  Date      : 02 Jan, 2023                                                  ##
##  License   : GPLv3                                                         ##
##  Author    : mateus-earth <mateus@cosmicpig.digital>                       ##
##  Copyright : mateus-earth - 2023                                           ##
##                                                                            ##
##  Description :                                                             ##
##    Installer for gosh.                                                     ##
##----------------------------------------------------------------------------##

function gosh() {
    ## Constants
    local SCRIPT_FULLPATH="${BASH_SOURCE}";
    local SCRIPT_DIR="$(dirname "$SCRIPT_FULLPATH")";
    local GOSH_EXE="${SCRIPT_DIR}/gosh2.py";

    if [ ! -f "$GOSH_EXE" ]; then
        echo "[gosh] Missing ($GOSH_EXE)";
        exit 1;
    fi;

    ## No args, just list the bookmarks.
    if [ $# -eq 0 ]; then
        python3 $GOSH_EXE --help;
        exit;
    fi;

    ## Concat all the arguments to make a string.
    ## Then search for the string for any flags.
    ## If we find this flags means that gosh is doing an action
    ## and we don't need to care about changing the directory.
    ## Otherwise we need to capture the output of it and make the
    ## poweshell to cd to that dir ;D
    for item in "$@"; do
        if [ "${item:0:2}" == "--" ] || [ "${item:0:1}" == "-" ]; then
            python3 $GOSH_EXE $@;
            break;
        fi;
    done;

    ## Changing directory...
    local target_path=$(python3 $GOSH_EXE $@);
    if [ -d "$target_path" ]; then
        cd $target_path || exit 1;
        echo "[gosh] ${PWD}";
    fi;
}
