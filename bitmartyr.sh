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

while getopts "w:h:s:p:d:f:" opt; do
    case "$opt" in
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
    make
    PROGRAMS[$PROG_INDEX]="./AIs/$exe/bin/$(make name)"
    PROG_INDEX=$((PROG_INDEX+1))
    cd ..
done
cd ..

if [[ -z "$OUTFILE" ]]; then
    ./bitmartyr $WIDTH $HEIGHT $SCALE $POPULATION $DURATION ${PROGRAMS[@]}
else
    ./bitmartyr $WIDTH $HEIGHT $SCALE $POPULATION $DURATION ${PROGRAMS[@]} -- $OUTFILE
fi
