#!/bin/sh
g++ $1.cpp -o $1
g++ summary.cpp -o summary

cd tools
rm score.txt
echo $2 > score.txt
x=`expr $2 - 1`

for i in `seq -f '%04g' 0 $x`
do
    cd /Users/nodaryohey/Documents/1_kyoPro/1_AtCoder/4AHC/013
    echo $i
    ./$1 < tools/in/$i.txt > tools/out/$i.txt
    cd tools
    head -n 1 in/$i.txt | tail -n 1 >> score.txt
    cargo run --release --bin vis in/$i.txt out/$i.txt >> score.txt
    mv vis.html vis/$i.html
done


../summary < score.txt >> ../summary.txt
