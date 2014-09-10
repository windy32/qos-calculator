# usage: awk -f validate.awk summary.log bw scale pct qlen

BEGIN {
    bw = ARGV[2]
    scale = ARGV[3]
    pct = ARGV[4]
    qlen = ARGV[5]
    ARGC = 2

    max_throughput = 0
    min_delay = 1000
    max_delay = 0
    avg_delay = 0
    count = 0
    
    delay = 0
    throughput = 0
}
{
    if (NF == 10 && $1 == bw && $2 == scale)
    {
        if ($9 > max_throughput)
        {
            max_throughput = $9
        }
        
        if ($7 > max_delay)
        {
            max_delay = $7
        }
        
        if ($7 < min_delay)
        {
            min_delay = $7
        }
        
        avg_delay += $7
        count += 1
        
        if ($3 == pct && $4 == qlen)
        {
            delay = $7
            throughput = $9
        }
    }
}

END {
    printf("%s\t%s\t", bw, scale);
    printf("%d\t%d\t%d\t", min_delay, avg_delay / count, max_delay);
    printf("%s\t", max_throughput);
    printf("%d\t%s\t", delay, throughput);
    printf("%.1f%%\n", throughput * 100.0 / max_throughput);
}

