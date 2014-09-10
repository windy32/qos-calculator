#!/bin/bash

# Note: this script is optimized for multi-core processors

echo -e "bw\tnum\tpct\tqlen\tmaxq\tavgq\tmaxd\tavgd\trate" > summary-ftp.log
echo "---------------------------------------------------------------------------" >> summary-ftp.log

run() # params: bw, ftps, pct, qlen, suffix
{
    if [ $# -eq 5 ]; then
        ns shaping.tcl "" $2 0 "$1"Mb 1460 $3 $4 $5
        echo -ne "$1\t$2\t$3\t$4\t" >> $5.tmp
        awk -f queue.awk link-isp-$5.tr $1 1.0 brief >> $5.tmp
        awk -f throughput.awk link-local-$5.tr $1 1.0 >> $5.tmp
        rm link-isp-$5.tr link-local-$5.tr
    else
        echo "Illegal arguments"
    fi
}

low_bandwidth()
{
    for bw in 0.5 1 2 3 # 4 Loops
    do
        for ftp in 1 2 3 4 5 6 8 10 12 14 16 20 24 30 40 # 15 Loops
        do
            echo -n "$bw Mbps ($ftp ftps) "
            for pct in 0.50 0.52 0.54 0.56 0.58 0.60 0.62 0.64 0.66 0.68 \
                       0.70 0.72 0.74 0.76 0.78 0.80 0.82 0.84 0.86 0.88 \
                       0.90 0.92 0.94 0.96 0.98 1.00 # 26 Loops
            do
                # for qlen in 2 3 4 5 6 7 8 # 7 Loops
                run $bw $ftp $pct 2 1 &
                run $bw $ftp $pct 3 2 &
                run $bw $ftp $pct 4 3 &
                run $bw $ftp $pct 5 4 &
                run $bw $ftp $pct 6 5 &
                run $bw $ftp $pct 7 6 &
                run $bw $ftp $pct 8 7 &

                # wait until all functions return
                wait
                
                cat 1.tmp >> summary-ftp.log
                cat 2.tmp >> summary-ftp.log
                cat 3.tmp >> summary-ftp.log
                cat 4.tmp >> summary-ftp.log
                cat 5.tmp >> summary-ftp.log
                cat 6.tmp >> summary-ftp.log
                cat 7.tmp >> summary-ftp.log

                rm 1.tmp 2.tmp 3.tmp 4.tmp 5.tmp 6.tmp 7.tmp            
                echo -n "."
            done
            echo ""
        done
    done
}

medium_bandwidth()
{
    for bw in 4 5 6 8 # 4 Loops
    do
        for ftp in 1 2 3 4 5 6 8 10 12 14 16 20 24 30 40 # 15 Loops
        do
            echo -n "$bw Mbps ($ftp ftps) "
            for pct in 0.80 0.82 0.84 0.86 0.88 0.90 0.92 0.94 0.96 0.98 1.00 # 11 Loops
            do
                #for qlen in 2 3 4 5 6 7 8 # 7 Loops
                run $bw $ftp $pct 2 1 &
                run $bw $ftp $pct 3 2 &
                run $bw $ftp $pct 4 3 &
                run $bw $ftp $pct 5 4 &
                run $bw $ftp $pct 6 5 &
                run $bw $ftp $pct 7 6 &
                run $bw $ftp $pct 8 7 &

                # wait until all functions return
                wait
                
                cat 1.tmp >> summary-ftp.log
                cat 2.tmp >> summary-ftp.log
                cat 3.tmp >> summary-ftp.log
                cat 4.tmp >> summary-ftp.log
                cat 5.tmp >> summary-ftp.log
                cat 6.tmp >> summary-ftp.log
                cat 7.tmp >> summary-ftp.log

                rm 1.tmp 2.tmp 3.tmp 4.tmp 5.tmp 6.tmp 7.tmp            
                echo -n "."
            done
            echo ""
        done
    done
}

high_bandwidth()
{
    for bw in 10 12 14 16 20 24 # 6 Loops
    do
        for ftp in 1 2 3 4 5 6 8 10 12 14 16 20 24 30 40 # 15 Loops
        do
            echo -n "$bw Mbps ($ftp ftps) "
            for pct in 0.80 0.82 0.84 0.86 0.88 0.90 0.92 0.94 0.96 0.98 1.00 # 11 Loops
            do
                # for qlen in 2 3 4 5 6 7 8 10 12 16 # 10 Loops
                run $bw $ftp $pct 2 1 &
                run $bw $ftp $pct 3 2 &
                run $bw $ftp $pct 4 3 &
                run $bw $ftp $pct 5 4 &
                run $bw $ftp $pct 6 5 &
                run $bw $ftp $pct 7 6 &
                run $bw $ftp $pct 8 7 &
                run $bw $ftp $pct 10 8 &
                run $bw $ftp $pct 12 9 &
                run $bw $ftp $pct 16 10 &

                # wait until all functions return
                wait

                cat 1.tmp >> summary-ftp.log
                cat 2.tmp >> summary-ftp.log
                cat 3.tmp >> summary-ftp.log
                cat 4.tmp >> summary-ftp.log
                cat 5.tmp >> summary-ftp.log
                cat 6.tmp >> summary-ftp.log
                cat 7.tmp >> summary-ftp.log
                cat 8.tmp >> summary-ftp.log
                cat 9.tmp >> summary-ftp.log
                cat 10.tmp >> summary-ftp.log

                rm 1.tmp 2.tmp 3.tmp 4.tmp 5.tmp 6.tmp 7.tmp 8.tmp 9.tmp 10.tmp
                echo -n "."
            done
            echo ""
        done
    done
}

very_high_bandwidth()
{
    for bw in 30 40 50 60 80 100 # 6 Loops
    do
        for ftp in 1 2 3 4 5 6 8 10 12 14 16 20 24 30 40 # 15 Loops
        do
            echo -n "$bw Mbps ($ftp ftps) "
            for pct in 0.90 0.91 0.92 0.93 0.94 0.95 0.96 0.97 0.98 0.99 1.00 # 11 Loops
            do
                # for qlen in 4 6 8 10 12 14 16 18 20 32 64 128 256 # 12 Loops
                run $bw $ftp $pct 4 1 &
                run $bw $ftp $pct 6 2 &
                run $bw $ftp $pct 8 3 &
                run $bw $ftp $pct 10 4 &
                run $bw $ftp $pct 12 5 &
                run $bw $ftp $pct 14 6 &

                # wait until all functions return
                wait

                run $bw $ftp $pct 16 7 &
                run $bw $ftp $pct 20 8 &
                run $bw $ftp $pct 32 9 &
                run $bw $ftp $pct 64 10 &
                run $bw $ftp $pct 128 11 &
                run $bw $ftp $pct 256 12 &

                # wait until all functions return
                wait

                cat 1.tmp >> summary-ftp.log
                cat 2.tmp >> summary-ftp.log
                cat 3.tmp >> summary-ftp.log
                cat 4.tmp >> summary-ftp.log
                cat 5.tmp >> summary-ftp.log
                cat 6.tmp >> summary-ftp.log
                cat 7.tmp >> summary-ftp.log
                cat 8.tmp >> summary-ftp.log
                cat 9.tmp >> summary-ftp.log
                cat 10.tmp >> summary-ftp.log
                cat 11.tmp >> summary-ftp.log
                cat 12.tmp >> summary-ftp.log

                rm 1.tmp 2.tmp 3.tmp 4.tmp 5.tmp 6.tmp
                rm 7.tmp 8.tmp 9.tmp 10.tmp 11.tmp 12.tmp
                echo -n "."
            done
            echo ""
        done
    done
}

low_bandwidth
medium_bandwidth
high_bandwidth
very_high_bandwidth

