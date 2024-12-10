#!/bin/bash 
var1=${PWD}
var2=${var1}/awaken/bin
while [ 1 ]
do
export LD_LIBRARY_PATH=${PWD}/awaken/libs/x64

cd $var2 
./awaken_offline_sample

cd $var1
aplay awaken.wav
arecord -D "plughw:0,0" -f S16_LE -r 16000 -d 5 -t wav 16k.wav
./asrmain 
./app
./ttsmain 
aplay result.wav

done
