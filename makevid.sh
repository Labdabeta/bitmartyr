#!/bin/bash

base_name=$1

mogrify -format png $1*.bmp
rm $1*.bmp
ffmpeg -r 20 -i $1%d.png $1.mp4
rm $1*.png
