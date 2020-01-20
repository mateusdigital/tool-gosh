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
##  Date      : Jan 02, 2018                                                  ##
##  License   : GPLv3                                                         ##
##  Author    : stdmatt <stdmatt@pixelwizards.io>                             ##
##  Copyright : stdmatt - 2018 - 2020                                         ##
##                                                                            ##
##  Description :                                                             ##
##                                                                            ##
##---------------------------------------------------------------------------~##

##----------------------------------------------------------------------------##
## Constants                                                                  ##
##----------------------------------------------------------------------------##
PW_SHELLSCRIPT_UTILS="/usr/local/src/stdmatt/shellscript_utils/main.sh";
INSTALL_DIR="/usr/local/bin";


##----------------------------------------------------------------------------##
## Helper Functions                                                           ##
##----------------------------------------------------------------------------##
##------------------------------------------------------------------------------
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

##------------------------------------------------------------------------------
ensure_install_directory()
{
    pw_can_write_at_path "$INSTALL_DIR";
    test $? != 0 &&  \
        pw_log_fatal "Can't write at path ($INSTALL_DIR)";

    if [ ! -d "$INSTALL_DIR" ]; then
        pw_log_warning                                                             \
            "The installation directory ($INSTALL_DIR) doesn't exists."            \
            "This indeed shows that it might be not in the PATH variable as well." \
            "gosh will fails to execute because it'll not find the gosh-core.py"   \
            ""                                                                     \
            "A directory will be created at ($INSTALL_DIR) automatically, but"     \
            "make sure to add the ($INSTALL_DIR) to your path."                    \
            ""                                                                     \
            "PATH=\$PATH:$INSTALL_DIR"                                             \
            ""                                                                     ;

    fi;

    local SUDO=$(pw_get_sudo_path);
    $SUDO mkdir -pv "$INSTALL_DIR";
}


##----------------------------------------------------------------------------##
## Script                                                                     ##
##----------------------------------------------------------------------------##
##
## Make sure that we have the shellscript_utls lib.
if [ ! -e "$PW_SHELLSCRIPT_UTILS" ]; then
    echo "Error - The stdmatt's shellscript_utils library wasn't found at path:";
    echo "    ($PW_SHELLSCRIPT_UTILS)."
    echo "";
    echo "This library is required to use gosh, so you need to install it first.";
    echo "You can find it at the following url:";
    echo "    (https://gitlab.com/stdmatt-libs/shellscript_utils)";
    echo "";
    echo "Aborting...";

    exit 1;
fi;

source "$PW_SHELLSCRIPT_UTILS";


##
## Install the files.
ensure_install_directory;

SCRIPT_DIR=$(pw_get_script_dir);
cp -f $SCRIPT_DIR/gosh-core.py  $INSTALL_DIR/gosh-core.py
cp -f $SCRIPT_DIR/gosh.sh       $INSTALL_DIR/gosh.sh

chmod 755 $INSTALL_DIR/gosh-core.py
chmod 755 $INSTALL_DIR/gosh.sh


##
## Add a entry on the .bash_rc / .bash_profile so we can use the gosh.
DEFAULT_BASH_RC=$(pw_get_default_bashrc_or_profile);
USE_BASH_RC=$(pw_getopt_exists "$@" "--bashrc");
USE_BASH_PROFILE=$(pw_getopt_exists $@ "--bash-profile");

USER_HOME=$(pw_find_real_user_home);
if [ -n "$USE_BASH_RC" ]; then
    _install_source_on "$USER_HOME/.bashrc";
elif [ -n "$USE_BASH_PROFILE" ]; then
    _install_source_on "$USR_HOME/.bash_profile";
else
    _install_source_on $DEFAULT_BASH_RC;
fi