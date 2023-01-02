##----------------------------------------------------------------------------##
##                       ~+                                                   ##
##                                                                            ##
##                                 *       +                                  ##
##                           '                  |                             ##
##                       ()    .-.,="``"=.    - o -                           ##
##                             '=/_       \     |                             ##
##                          *   |  '=._    |                                  ##
##                               \     `=./`,        '                        ##
##                            .   '=.__.=' `='      *                         ##
##                   +                         +                              ##
##                        O      *        '       . jgs                       ##
##                                                                            ##
##  File      : install.sh                                                    ##
##  Project   : gosh                                                          ##
##  Date      : 02 Jan, 2023                                                  ##
##  License   : GPLv3                                                         ##
##  Author    : stdmatt <stdmatt@pixelwizards.io>                             ##
##  Copyright : stdmatt - 2023                                                ##
##                                                                            ##
##  Description :                                                             ##
##    Installer for gosh.                                                     ##
##----------------------------------------------------------------------------##


##----------------------------------------------------------------------------##
## Constants                                                                  ##
##----------------------------------------------------------------------------##
##------------------------------------------------------------------------------
## Script
readonly SCRIPT_FULLPATH="${BASH_SOURCE}";
readonly SCRIPT_DIR="$(dirname "$SCRIPT_FULLPATH")";

## Program
readonly PROGRAM_NAME="gosh";
readonly SOURCE_PROGRAM_PATH="${SCRIPT_DIR}/${PROGRAM_NAME}";
readonly INSTALL_PROGRAM_PATH="${HOME}/.mateus-earth/bin/${PROGRAM_NAME}";


##----------------------------------------------------------------------------##
## Script                                                                     ##
##----------------------------------------------------------------------------##
##------------------------------------------------------------------------------
echo "==> Installing ($PROGRAM_NAME)...";
if [ -d "${INSTALL_PROGRAM_PATH}" ]; then
    rm -rf "${INSTALL_PROGRAM_PATH}" || exit 1;
fi;

echo "==> Creating directory at: (${INSTALL_PROGRAM_PATH})";
mkdir -p "${INSTALL_PROGRAM_PATH}" || exit 1;

echo "==> Copying files...";
cp -r "${SOURCE_PROGRAM_PATH}"/* "${INSTALL_PROGRAM_PATH}";

echo "==> ${PROGRAM_NAME} was installed at: ${INSTALL_PROGRAM_PATH}";
echo "==> You need to source it to use: ";
echo "    source \"${INSTALL_PROGRAM_PATH}/gosh.sh\"";

echo "==> Done... ;D";
echo "";
