## @todo(stdmatt): Add the license header....

##
## Constants
##
##------------------------------------------------------------------------------
## Script
SCRIPT_DIR=$(dirname "$0");
HOME_DIR="$HOME";
## Program
PROGRAM_NAME="gosh";
PROGRAM_SOURCE_PATH="$SCRIPT_DIR/src";
PROGRAM_INSTALL_PATH="$HOME_DIR/.stdmatt_bin";
PROGRAM_INSTALL_SUBPATH="$PROGRAM_INSTALL_PATH/$PROGRAM_NAME";


##
## Script
##
##------------------------------------------------------------------------------
echo "Installing ...";

## Create the install directory...
if [ ! -d "$PROGRAM_INSTALL_SUBPATH" ]; then
    echo "Creating directory at: ";
    echo "    $PROGRAM_INSTALL_SUBPATH";
    mkdir -p "$PROGRAM_INSTALL_SUBPATH" > /dev/null;
fi;

## Copy the file to the install dir...
cp -f "$PROGRAM_SOURCE_PATH/gosh-core.py" "$PROGRAM_INSTALL_SUBPATH/gosh-core.py";
cp -f "$PROGRAM_SOURCE_PATH/gosh.sh"      "$PROGRAM_INSTALL_SUBPATH/gosh.sh";

echo "$PROGRAM_NAME was installed at:";
echo "source $PROGRAM_INSTALL_SUBPATH/gosh.sh";

echo "Done... ;D";
echo "";
