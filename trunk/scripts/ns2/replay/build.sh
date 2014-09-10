#!/bin/bash

if [[ $# == 0 ]]; then
    echo "Building connection vectors"

    for bw in 0.5 1 2 3 4 5 6 8 10 12 14 16 20 24 30 40 50 60 80 100
    do
        echo -n "   - $bw Mbps "
        for scale in 0.30 0.40 0.50 0.60 0.70 0.80 0.90 1.00 \
                     1.10 1.20 1.30 1.40 1.50 1.60 1.70 1.80 1.90 2.00
        do
            ./cvectool inbound.cvec -b $bw -s $scale > "inbound-$bw""m-$scale.cvec" 2>/dev/null
            echo -n "."
        done
        echo ""
    done
elif [[ $# == 1 && $1 == "clean" ]]; then
    for bw in 0.5 1 2 3 4 5 6 8 10 12 14 16 20 24 30 40 50 60 80 100
    do
        for scale in 0.30 0.40 0.50 0.60 0.70 0.80 0.90 1.00 \
                     1.10 1.20 1.30 1.40 1.50 1.60 1.70 1.80 1.90 2.00
        do
            rm -f "inbound-$bw""m-$scale.cvec"
        done
    done
else
    echo "Illegal arguments"
fi

