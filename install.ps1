#!/usr/bin/env pwsh
##----------------------------------------------------------------------------##
##                        _      _                 _   _                      ##
##                       | |    | |               | | | |                     ##
##                    ___| |_ __| |_ __ ___   __ _| |_| |_                    ##
##                   / __| __/ _` | '_ ` _ \ / _` | __| __|                   ##
##                   \__ \ || (_| | | | | | | (_| | |_| |_                    ##
##                   |___/\__\__,_|_| |_| |_|\__,_|\__|\__|                   ##
##                                                                            ##
##                                                                            ##
##  File      : install.ps1                                                   ##
##  Project   : gosh                                                          ##
##  Date      : 10 Apr, 2021                                                  ##
##  License   : GPLv3                                                         ##
##  Author    : stdmatt <stdmatt@pixelwizards.io>                             ##
##  Copyright : stdmatt - 2021, 2022                                          ##
##                                                                            ##
##  Description :                                                             ##
##                                                                            ##
##---------------------------------------------------------------------------~##

##----------------------------------------------------------------------------##
## Constants                                                                  ##
##----------------------------------------------------------------------------##
##------------------------------------------------------------------------------
## Script
$SCRIPT_FULLPATH = $MyInvocation.MyCommand.Path;
$SCRIPT_DIR      = Split-Path "$SCRIPT_FULLPATH" -Parent;
$HOME_DIR        = if($HOME -eq "") { "$env:USERPROFILE" } else { $HOME };
## Program
$PROGRAM_NAME              = "gosh";
$DIRECTORY_NAME            = "${PROGRAM_NAME}_";
$PROGRAM_SOURCE_PATH       = "$SCRIPT_DIR/$PROGRAM_NAME";
$PROGRAM_INSTALL_ROOT_PATH = "$HOME_DIR/.stdmatt/bin";
$PROGRAM_INSTALL_SUB_PATH  = "$PROGRAM_INSTALL_ROOT_PATH/$DIRECTORY_NAME";


##----------------------------------------------------------------------------##
## Script                                                                     ##
##----------------------------------------------------------------------------##
##------------------------------------------------------------------------------
echo "Installing ...";

## Create the install directory...
if (-not (Test-Path -LiteralPath $PROGRAM_INSTALL_SUB_PATH)) {
    echo "Creating directory at: ";
    echo "    $PROGRAM_INSTALL_SUB_PATH";
    $null = (New-Item -Path $PROGRAM_INSTALL_SUB_PATH -ItemType Directory -Force);
}

## Copy the file to the install dir...
Copy-Item -Force $PROGRAM_SOURCE_PATH/gosh2.py     `
                 $PROGRAM_INSTALL_SUB_PATH/gosh2.py

echo "$PROGRAM_NAME was installed at:";
echo "    $PROGRAM_INSTALL_ROOT_PATH";
echo "You might need add it to the PATH";

echo "Done... ;D";
echo "";
