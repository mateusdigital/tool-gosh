## @todo(stdmatt): Add the license header.... 3/17/2021, 11:00:20 AM

##
## Constants
##
##------------------------------------------------------------------------------
## Script
$SCRIPT_FULLPATH = $MyInvocation.MyCommand.Path;
$SCRIPT_DIR      = Split-Path "$SCRIPT_FULLPATH" -Parent;
$HOME_DIR        = "$env:USERPROFILE";
## Program
$PROGRAM_NAME            = "gosh";
$PROGRAM_SOURCE_PATH     = "$SCRIPT_DIR/src";
$PROGRAM_INSTALL_PATH    = "$HOME_DIR/.stdmatt_bin";
$PROGRAM_INSTALL_SUBPATH = "$PROGRAM_INSTALL_PATH/$PROGRAM_NAME";

##
## Script
##
##------------------------------------------------------------------------------
echo "Installing ...";

## Create the install directory...
if (-not (Test-Path -LiteralPath $PROGRAM_INSTALL_SUBPATH)) {
    echo "Creating directory at: ";
    echo "    $PROGRAM_INSTALL_SUBPATH";
    $null = New-Item -Path $PROGRAM_INSTALL_SUBPATH -ItemType Directory;
}

## Copy the file to the install dir...
cp -Force $PROGRAM_SOURCE_PATH/gosh-core.py $PROGRAM_INSTALL_SUBPATH/gosh-core.py
cp -Force $PROGRAM_SOURCE_PATH/gosh.ps1     $PROGRAM_INSTALL_PATH/gosh.ps1

echo "$PROGRAM_NAME was installed at:";
echo "    $PROGRAM_INSTALL_PATH";
echo "You might need add it to the PATH";

echo "Done... ;D";
echo "";
