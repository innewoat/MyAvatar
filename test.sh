#!/bin/sh
mkdir output
g++ avatar_a.cpp -o output/avatar_a
./output/avatar_a
ffmpeg -i output/pic0.ppm -y output/pic0.bmp