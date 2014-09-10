#!/bin/bash

# The best buffer size (in packets) for different bandwidth
qlen=(3 3 3 4 4 4 4 5 5 5 6 6 7 7 8 10 12 14 16 20)

# The best percentage of bandwidth used
pct=(0.60 0.72 0.74 0.74 0.80 0.80 0.80 0.82 0.84 0.84 0.86 0.88 0.88 0.90 0.92 0.93 0.94 0.95 0.96 0.96)

# Analysis
i=0
for bw in 0.5 1 2 3 4 5 6 8 10 12 14 16 20 24 30 40 50 60 80 100 # 20 Loops
do
    for scale in 0.30 0.40 0.50 0.60 0.70 0.80 0.90 1.00 \
                 1.10 1.20 1.30 1.40 1.50 1.60 1.70 1.80 1.90 2.00 # 18 Loops
    do
        awk -f validate.awk summary.log $bw $scale ${pct[$i]} ${qlen[$i]}
    done
    i=`expr $i + 1`
done

