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


##----------------------------------------------------------------------------##
## Helper Functions                                                           ##
##----------------------------------------------------------------------------##
function check_arg
{
    local cmd="$1";
    local arg="$2";

    ##
    ## Empty args...
    if [ -z "$2" ]; then
        arg=".";
        # echo "[FATAL] Option ($cmd) needs a argument.";
        # return 1;
    fi;

    ##
    ## Test the argument to check if this is a short flag.
    ##   This handles the cases that user typed:
    ##   gosh -a -r ola -> The -a needs a flag bug will get the -r.
    if [ ${arg:0:1} == "-" ] && [ ${arg:1:1} != "-" ]; then
        echo "[FATAL] Option ($cmd) needs a non flag argument. Got ($arg) instead.";
        return 1;
    fi;

    ##
    ## Test the argument to check if this is a long flag.
    ##   This handles the cases that user typed:
    ##   gosh -a --remove ola -> The -a needs a flag bug will get the --remove.
    local is_long_flag="";
    if [ ${arg:0:2} == "--" ]; then
       echo "[FATAL] Option ($cmd) needs a non flag argument. Got ($arg) instead.";
       return 1;
    fi;

    # echo "cmd: $cmd arg: $arg"
    return 0;
}


##----------------------------------------------------------------------------##
## Gosh                                                                       ##
##----------------------------------------------------------------------------##
function gosh
{
    local GOSH_CORE=gosh-core.py

    ##
    ## The command line options that we can accept.
    ## They are set with 0 meaning that they're disabled.
    ## Only NO_COLORS options that are set with empty string
    ## because we gonna fill it with a string if user pass the option.
    local OPT_HELP=0;
    local OPT_VERSION=0;
    local OPT_LIST=0;
    local OPT_LIST_LONG=0;
    local OPT_REMOVE=0;
    local OPT_ADD=0;
    local OPT_UPDATE=0;
    local OPT_PRINT=0;
    local OPT_NO_COLORS="";
    local OPT_EXISTS_BOOKMARK=0;

    ##
    ## No args, just list the bookmarks.
    if [ $# -eq 0 ]; then
        $GOSH_CORE "gosh_opt_list";
        return;
    fi;

    local ARG1="";
    local ARG2="";
    local BOOKMARK="";
    while [ $# -gt 0 ]; do
        case $1 in
            ##
            ## Those flags don't need arguments.
            -h | --help     ) OPT_HELP=1                ;;
            -v | --version  ) OPT_VERSION=1             ;;
            -l | --list     ) OPT_LIST=1                ;;
            -L | --list-long) OPT_LIST_LONG=1           ;;
            -n | --no-colors) OPT_NO_COLORS="no-colors" ;;

            ##
            ## Those flags needs 1 argument.
            -r | --remove)
                    OPT_REMOVE=1;
                    ARG1=$2; check_arg $1 $2 || return 1;

                    shift;  #Shift 1 time because we consumed 1 arg.
                ;;
            -e | --exists)
                    OPT_EXISTS_BOOKMARK=1;
                    ARG1=$2; check_arg $1 $2 || return 1;

                    shift;  #Shift 1 time because we consumed 1 arg.
                ;;
            -p | --print )
                    OPT_PRINT=1;
                    ARG1=$2; check_arg $1 $2 || return 1;
                    shift;  #Shift 1 time because we consumed 1 arg.
                ;;

            ##
            ## Those flags needs 2 arguments.
            -a | --add)
                    OPT_ADD=1;
                    ARG1=$2; check_arg $1 $ARG1 || return 1;
                    ARG2=$3; check_arg $1 $ARG2 || return 1;

                    shift; shift; #Shift 2 times because we consumed 2 args.
                ;;
            -u | --update)
                    OPT_UPDATE=1;
                    ARG1=$2; check_arg $1 $ARG1 || return 1;
                    ARG2=$3; check_arg $1 $ARG2 || return 1;

                    shift; shift; #Shift 2 times because we consumed 2 args.
                ;;

            ##
            ## Invalid flag.
            -* | --*)
                    echo "[FATAL] Invalid flag ($1)"
                    return 1;
                ;;

            ##
            ## Bookmark option.
            *)
                    BOOKMARK="$1";
                    shift;
                ;;
        esac
        shift
    done;

    ############################################################################
    ## Debug...
    # echo "OPT_HELP            = $OPT_HELP";
    # echo "OPT_VERSION         = $OPT_VERSION";
    # echo "OPT_LIST            = $OPT_LIST";
    # echo "OPT_LIST_LONG       = $OPT_LIST_LONG";
    # echo "OPT_REMOVE          = $OPT_REMOVE";
    # echo "OPT_ADD             = $OPT_ADD";
    # echo "OPT_UPDATE          = $OPT_UPDATE";
    # echo "OPT_PRINT           = $OPT_PRINT";
    # echo "OPT_NO_COLORS       = $OPT_NO_COLORS";
    # echo "OPT_EXISTS_BOOKMARK = $OPT_EXISTS_BOOKMARK";

    # echo "ARG1 = $ARG1";
    # echo "ARG2 = $ARG2";

    # echo "BOOKMARK = $BOOKMARK"
    # echo "$@"
    ############################################################################


    ##
    ## Start checking with command line options were given.
    ##    All options are exclusive, meaning that they'll run and exit after.

    ##
    ## No Args Flags.
    if [ $OPT_HELP = 1 ]; then
        $GOSH_CORE "gosh_opt_help";

    elif [ $OPT_VERSION = 1 ]; then
        $GOSH_CORE "gosh_opt_version";

    elif [ $OPT_LIST = 1 ]; then
        # $OPT_NO_COLORS -> empty if not defined by user.
        $GOSH_CORE "gosh_opt_list" $OPT_NO_COLORS;

    elif [ $OPT_LIST_LONG = 1 ]; then
        # $OPT_NO_COLORS -> empty if not defined by user.
        $GOSH_CORE "gosh_opt_list-long" $OPT_NO_COLORS;

    ##
    ## Flags with 1 arg.
    elif [ $OPT_REMOVE = 1 ]; then
        # $1 -> The name of bookmark.
        # $OPT_NO_COLORS -> empty if not defined by user.
        $GOSH_CORE "gosh_opt_remove" "$ARG1" $OPT_NO_COLORS;

    elif [ $OPT_PRINT = 1 ]; then
        # $ARG1 -> The name of bookmark.
        # $OPT_NO_COLORS -> empty if not defined by user.
        $GOSH_CORE "gosh_opt_print" "$ARG1" $OPT_NO_COLORS

    elif [ $OPT_EXISTS_BOOKMARK = 1 ]; then
        # $OPT_NO_COLORS -> empty if not defined by user.
        $GOSH_CORE "gosh_opt_exists_bookmark" "$ARG1" $OPT_NO_COLORS;

    ##
    ## Flags with 2 args.
    elif [ $OPT_ADD = 1 ]; then
        # $ARG1 -> The name of bookmark.
        # $ARG2 -> The path - This could be empty, gosh-core will handle this.
        # $OPT_NO_COLORS -> empty if not defined by user.
        $GOSH_CORE "gosh_opt_add" "$ARG1" "$ARG2" $OPT_NO_COLORS;

    elif [ $OPT_UPDATE = 1 ]; then
        # $ARG1 -> The name of bookmark.
        # $ARG2 -> The path - This could be empty, gosh-core will handle this.
        # $OPT_NO_COLORS -> empty if not defined by user.
        $GOSH_CORE "gosh_opt_update" "$ARG1" "$ARG2" $OPT_NO_COLORS;

    ##
    ## Change Path to Boorkmark.
    else
        ## ALWAYS NO COLORS since it will be passed to cd(1) and
        ## we don't want the escape chars on it.
        local RET_VAL=$($GOSH_CORE "gosh_opt_print" "$BOOKMARK");
        $GOSH_CORE "gosh_opt_print" "$BOOKMARK" > /dev/null;

        ##
        ## The gosh-core call was successful?
        if [ $? = 0 ]; then
            #Change the directory.
            RET_VAL=$(echo "$RET_VAL" | sed s#" "#"\ "#g);
            cd "$RET_VAL";
            if [ -z "$OPT_NO_COLORS" ]; then
                echo "Gosh:"; tput setaf 5; echo " $RET_VAL"; tput sgr0;
            else
                echo "$RET_VAL";
            fi;
        else
            echo $RET_VAL;
        fi;
    fi;
}
