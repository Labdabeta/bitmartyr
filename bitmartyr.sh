#!/bin/bash

OPTIND=1

# Default parameters
WIDTH=100
HEIGHT=100
SCALE=5
POPULATION=5
DURATION=1000
OUTFILE=
declare -a PROGRAMS

while getopts "?w:h:s:p:d:f:" opt; do
    case "$opt" in
        '?')
            echo "$0 [-w WIDTH] [-h HEIGHT] [-s SCALE] [-p POPULATION] [-d DURATION] [-f OUTFILE]"
            ;;
        w)
            WIDTH=$OPTARG
            ;;
        h)
            HEIGHT=$OPTARG
            ;;
        s)
            SCALE=$OPTARG
            ;;
        p)
            POPULATION=$OPTARG
            ;;
        d)
            DURATION=$OPTARG
            ;;
        f)
            OUTFILE=$OPTARG
            ;;
    esac
done

shift $((OPTIND-1))

[ "$1" = "--" ] && shift

# Make sure the bitmartyr executable exists
if [ ! -f bitmartyr ]; then
    make
fi

cd AIs
PROG_INDEX=0
for exe in "$@"
do
    cd $exe
    make >/dev/null 2>&1
    PROGRAMS[$PROG_INDEX]="./AIs/$exe/bin/$(make name)"
    if [ ! -f "bin/$(make name)" ]; then
        echo "Could not locate bin/$(make name)."
        exit 1
    fi
    PROG_INDEX=$((PROG_INDEX+1))
    cd ..
done
cd ..

if [[ -z "$OUTFILE" ]]; then
    ./bitmartyr $WIDTH $HEIGHT $SCALE $POPULATION $DURATION ${PROGRAMS[@]}
else
    ./bitmartyr $WIDTH $HEIGHT $SCALE $POPULATION $DURATION ${PROGRAMS[@]} -- $OUTFILE
fi
