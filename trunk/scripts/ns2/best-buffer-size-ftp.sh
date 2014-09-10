#!/bin/sh

for bw in 0.5 1 2 3 4 5 6 8
do
    for qlen in 2 3 4 5 6 7 8
    do
        echo -n "$bw\t$qlen\t"
        awk -f best-buffer-size-ftp.awk summary-ftp.log $bw $qlen
    done
done

for bw in 10 12 14 16 20 24
do
    for qlen in 2 3 4 5 6 7 8 10 12 16
    do
        echo -n "$bw\t$qlen\t"
        awk -f best-buffer-size-ftp.awk summary-ftp.log $bw $qlen
    done
done

for bw in 30 40 50 60 80 100
do
    for qlen in 4 6 8 10 12 14 16 20 32 64 128 256
    do
        echo -n "$bw\t$qlen\t"
        awk -f best-buffer-size-ftp.awk summary-ftp.log $bw $qlen
    done
done

