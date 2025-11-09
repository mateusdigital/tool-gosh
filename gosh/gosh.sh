##----------------------------------------------------------------------------##
##                               *       +                                    ##
##                         '                  |                               ##
##                     ()    .-.,="``"=.    - o -                             ##
##                           '=/_       \     |                               ##
##                        *   |  '=._    |                                    ##
##                             \     `=./`,        '                          ##
##                          .   '=.__.=' `='      *                           ##
##                 +                         +                                ##
##                      O      *        '       .                             ##
##                                                                            ##
##  File      : gosh.sh                                                       ##
##  Project   : gosh                                                          ##
##  Date      : 2023-01-02                                                    ##
##  License   : See project's COPYING.TXT for full info.                      ##
##  Author    : mateus.digital <hello@mateus.digital>                         ##
##  Copyright : mateus.digital - 2023 - 2025                                  ##
##                                                                            ##
##  Description :                                                             ##
##                                                                            ##
##----------------------------------------------------------------------------##

## -----------------------------------------------------------------------------
function gosh() {
    ## Constants
    local SCRIPT_FULLPATH="${BASH_SOURCE}";
    local SCRIPT_DIR="$(dirname "$SCRIPT_FULLPATH")";
    local GOSH_EXE="${SCRIPT_DIR}/gosh_/gosh2.py";

    if [ ! -f "$GOSH_EXE" ]; then
        echo "[gosh] Missing ($GOSH_EXE)";
        return 1;
    fi;

    ## No args, just list the bookmarks.
    if [ $# -eq 0 ]; then
        python3 $GOSH_EXE --help;
        return 0;
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
            return;
        fi;
    done;

    ## Changing directory...
    local target_path=$(python3 $GOSH_EXE $@);
    if [ -d "$target_path" ]; then
        cd $target_path || exit 1;
        echo "[gosh] ${PWD}";
        return
    fi;
}
