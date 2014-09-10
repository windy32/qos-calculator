#!/bin/bash

# The best buffer size (in packets) for different bandwidth
qlen=(3 3 3 4 4 4 4 5 5 5 6 6 7 7 8 10 12 14 16 20)
qlen3x=(9 9 9 12 12 12 12 15 15 15 18 18 21 21 24 30 36 42 48 60)

# The best percentage of bandwidth used
pct=(0.60 0.72 0.74 0.74 0.76 0.78 0.80 0.82 0.84 0.84 0.86 0.88 0.88 0.90 0.92 0.93 0.94 0.95 0.96 0.96)

# Test 1. Use packets as the unit
unit_packet()
{
    echo -e "bw\tnum\tpct\tqlen\tmaxq\tavgq\tmaxd\tavgd\trate" > buffer-unit-pkt.log
    echo "---------------------------------------------------------------------------" >> buffer-unit-pkt.log

    i=0
    for bw in 0.5 1 2 3 4 5 6 8 10 12 14 16 20 24 30 40 50 60 80 100 # 10 Loops
    do
        for num in 1 2 3 4 5 6 8 10 12 14 16 20 24 30 40 # 15 Loops
        do
            ns shaping.tcl "" $num 0 "$bw"Mb 460 ${pct[$i]} ${qlen[$i]}
            echo -ne "$bw\t$num\t${pct[$i]}\t${qlen[$i]}\t" >> buffer-unit-pkt.log
            awk -f queue.awk link-isp.tr $bw 1.0 brief >> buffer-unit-pkt.log
            awk -f throughput.awk link-local.tr $bw 1.0 >> buffer-unit-pkt.log
        done
        i=`expr $i + 1`
    done
}

# Test 2. Use byte as unit
unit_byte()
{
    echo -e "bw\tnum\tpct\tqlen\tmaxq\tavgq\tmaxd\tavgd\trate" > buffer-unit-byte.log
    echo "---------------------------------------------------------------------------" >> buffer-unit-byte.log

    i=0
    for bw in 0.5 1 2 3 4 5 6 8 10 12 14 16 20 24 30 40 50 60 80 100 # 10 Loops
    do
        for num in 1 2 3 4 5 6 8 10 12 14 16 20 24 30 40 # 15 Loops
        do
            ns shaping.tcl "" $num 0 "$bw"Mb 460 ${pct[$i]} ${qlen3x[$i]}
            echo -ne "$bw\t$num\t${pct[$i]}\t${qlen3x[$i]}\t" >> buffer-unit-byte.log
            awk -f queue.awk link-isp.tr $bw 1.0 brief >> buffer-unit-byte.log
            awk -f throughput.awk link-local.tr $bw 1.0 >> buffer-unit-byte.log
        done
        i=`expr $i + 1`
    done
}

if [ $# -eq 0 ]; then
    unit_packet
    unit_byte
elif [ $# -eq 1 ]; then
    if [ $1 = "packet" ]; then
        unit_packet
    elif [ $1 = "byte" ]; then
        unit_byte
    fi
fi

