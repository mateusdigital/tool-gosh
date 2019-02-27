##----------------------------------------------------------------------------##
##               █      █                                                     ##
##               ████████                                                     ##
##             ██        ██                                                   ##
##            ███  █  █  ███        gosh_bash-completions.sh                  ##
##            █ █        █ █        Gosh                                      ##
##             ████████████                                                   ##
##           █              █       Copyright (c) 2015, 2016                  ##
##          █     █    █     █      AmazingCow - www.AmazingCow.com           ##
##          █     █    █     █                                                ##
##           █              █       N2OMatt - n2omatt@amazingcow.com          ##
##             ████████████         www.amazingcow.com/n2omatt                ##
##                                                                            ##
##                  This software is licensed as GPLv3                        ##
##                 CHECK THE COPYING FILE TO MORE DETAILS                     ##
##                                                                            ##
##    Permission is granted to anyone to use this software for any purpose,   ##
##   including commercial applications, and to alter it and redistribute it   ##
##               freely, subject to the following restrictions:               ##
##                                                                            ##
##     0. You **CANNOT** change the type of the license.                      ##
##     1. The origin of this software must not be misrepresented;             ##
##        you must not claim that you wrote the original software.            ##
##     2. If you use this software in a product, an acknowledgment in the     ##
##        product IS HIGHLY APPRECIATED, both in source and binary forms.     ##
##        (See opensource.AmazingCow.com/acknowledgment.html for details).    ##
##        If you will not acknowledge, just send us a email. We'll be         ##
##        *VERY* happy to see our work being used by other people. :)         ##
##        The email is: acknowledgment_opensource@AmazingCow.com              ##
##     3. Altered source versions must be plainly marked as such,             ##
##        and must not be misrepresented as being the original software.      ##
##     4. This notice may not be removed or altered from any source           ##
##        distribution.                                                       ##
##     5. Most important, you must have fun. ;)                               ##
##                                                                            ##
##      Visit opensource.amazingcow.com for more open-source projects.        ##
##                                                                            ##
##                                  Enjoy :)                                  ##
##----------------------------------------------------------------------------##

_gosh()
{
    #This script was pratically wrote like was found in:
    #https://www.debian-administration.org/article/317/An_introduction_to_bash_completion_part_2
    local cur prev opts
    COMPREPLY=()
    cur="${COMP_WORDS[COMP_CWORD]}"
    prev="${COMP_WORDS[COMP_CWORD-1]}"

    #Short options
    if [[ "$cur" == - ]]; then
        COMPREPLY=( $( compgen -W '-h -v -l -L -r -a -p -n -e' -- "$cur" ) )
        return 0;

    #Long Options.
    elif [[ "$cur" == -* ]]; then
        COMPREPLY=( $( compgen -W '--help --version --list --list-long --remove
        --add --update --print --no-colors --exists' -- "$cur" ) )
        return 0;

    #Bookmarks.
    else
        opts="$(gosh-core gosh_opt_list no-colors)" #List in short way without colors.
        COMPREPLY=( $(compgen -W "${opts}" -- ${cur}) )
        return 0
    fi

    return 1;
}

complete -F _gosh gosh
