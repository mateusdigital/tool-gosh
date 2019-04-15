#!/usr/bin/env bash
##~---------------------------------------------------------------------------##
##                     _______  _______  _______  _     _                     ##
##                    |   _   ||       ||       || | _ | |                    ##
##                    |  |_|  ||       ||   _   || || || |                    ##
##                    |       ||       ||  | |  ||       |                    ##
##                    |       ||      _||  |_|  ||       |                    ##
##                    |   _   ||     |_ |       ||   _   |                    ##
##                    |__| |__||_______||_______||__| |__|                    ##
##                             www.amazingcow.com                             ##
##  File      : install.sh                                                    ##
##  Project   : Gosh                                                          ##
##  Date      : Jan 02, 2018                                                  ##
##  License   : GPLv3                                                         ##
##  Author    : n2omatt <n2omatt@amazingcow.com>                              ##
##  Copyright : AmazingCow - 2018                                             ##
##                                                                            ##
##  Description :                                                             ##
##                                                                            ##
##---------------------------------------------------------------------------~##

##----------------------------------------------------------------------------##
## Constants                                                                  ##
##----------------------------------------------------------------------------##
PW_SHELLSCRIPT_UTILS="/usr/local/src/pixelwizards/shellscript_utils/main.sh";
INSTALL_DIR="/usr/local/bin";


##----------------------------------------------------------------------------##
## Helper Functions                                                           ##
##----------------------------------------------------------------------------##
_install_source_on()
{
    local PATH_TO_INSTALL="$1";
    echo "Installing dots on ($PATH_TO_INSTALL)";

    if [ ! -e "$PATH_TO_INSTALL" ]; then
        pw_log_fatal "$PATH_TO_INSTALL does not exists - Aborting...";
    fi;

    local result=$(cat "$PATH_TO_INSTALL" | grep "source $INSTALL_DIR/gosh.sh");
    if [ -z "$result" ]; then
        echo "## stdmatt's gosh ##" >> "$PATH_TO_INSTALL";
        echo "source $INSTALL_DIR/gosh.sh" >> "$PATH_TO_INSTALL";
    fi;

    echo "Done...";
}


##----------------------------------------------------------------------------##
## Script                                                                     ##
##----------------------------------------------------------------------------##
##
## Make sure that we have the shellscript_utls lib.
if [ ! -e "$PW_SHELLSCRIPT_UTILS" ]; then
    echo "Error - The Pixel Wizard's shellscript_utils library wasn't found at path:";
    echo "    ($PW_SHELLSCRIPT_UTILS)."
    echo "";
    echo "This library is required to use manpdf, so you need to install it first.";
    echo "You can find it at the following url:";
    echo "    (https://gitlab.com/pixelwizardsio/libs/shellscript_utils)";
    echo "";
    echo "Aborting...";

    exit 1;
fi;

source "$PW_SHELLSCRIPT_UTILS";


##
## Install the files.
cp -f ./gosh-core.py  $INSTALL_DIR/gosh-core
cp -f ./gosh.sh       $INSTALL_DIR/gosh

chmod 755 $INSTALL_DIR/gosh-core
chmod 755 $INSTALL_DIR/gosh


##
## Add a entry on the .bash_rc / .bash_profile so we can use the gosh.
DEFAULT_BASH_RC=$(pw_get_default_bashrc_or_profile);
USE_BASH_RC=$(pw_getopt_exists "$@" "--bashrc");
USE_BASH_PROFILE=$(pw_getopt_exists $@ "--bash-profile");

if [ -n "$USE_BASH_RC" ]; then
    _install_source_on "$HOME/.bashrc";
elif [ -n "$USE_BASH_PROFILE" ]; then
    _install_source_on "$HOME/.bash_profile";
else
    _install_source_on $DEFAULT_BASH_RC;
fi
