#!/bin/sh
g++ $1.cpp -o $1

for i in `seq -f '%04g' 0 99`
do
    # echo $i
    cd /Users/nodaryohey/Documents/1_kyoPro/1_AtCoder/4AHC/013
    cat tools/in/$i.txt | ./$1 > tools/out/$i.txt
    cd tools
    # pwd
    cargo run --release --bin vis in/$i.txt out/$i.txt
    mv vis.html vis/$i.html
done