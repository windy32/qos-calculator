# usage: awk -f best-buffer-size.awk summary.log bw qlen
BEGIN {
    bw = ARGV[2]
    qlen = ARGV[3]
    ARGC = 2

    # initialize
    avg_delay = 0
    avg_throughput = 0
    count = 0
}
{
    if (NF == 10)
    {
        if ($1 == bw && $4 == qlen)
        {
            if ($7 < 100)
            {
                avg_delay += $7
                avg_throughput += $9
                count += 1
            }
        }
    }
}

END {
    if (count > 0)
    {
        printf("%.1f\t%.3f\n", avg_delay / count, avg_throughput / count / (bw * 125));
    }
    else
    {
        printf("-\t-\n");
    }
}

