#! /bin/bash
#############################################################
#
#  -Script copies a source directory and all its
#   subdirectories to destination address. It only copies
#   .png files into copy directory, ignoring any other file type.
#   It also converts them to the .png files to .jpg files
#
#  -Script checks for input errors and calls a recursive
#   function copyDir with the current directory to be copied
#
#  -CopyDir function copys all the subdirectories to the 
#   destination folder. It also converts all png files to jpg
#   files in the destination folder.
#
#   Usage:  $ chmod u+x copyDir.sh
#           $ ./copyDir.sh [source] [destination]
#
#############################################################

function copyDir 
{
    cd $1
    end=1

    shopt -s nullglob
    for FILE in *; do
            if [ -d "$FILE" ]; then
                copyDir $FILE
                end=0
            fi
        done  

    if [ $end -eq 1 ]; then
            destdir=${PWD/$from/$dest}
            mkdir -p $destdir
    fi

    
    for FILE in *.png; do
                cur="$PWD/$FILE"
                destfile=${cur/$from/$dest}
                destfile2=${destfile/".png"/".jpg"}
                convert $cur $destfile2
        done
    cd ..
}

if [ ! $# -eq 2 ]; then
    echo Number of arguments is incorrect
    echo Correct usage: $0 [source] [destination]
    exit
fi

if [ -d "$1" ]; then
    if [ -w "$2" ]; then
        from=$1
        dest=$2
        copyDir $1
    else
        echo Cannot write to destination directory
        exit 
    fi
else
    echo Source directory does not exist
    exit
fi
