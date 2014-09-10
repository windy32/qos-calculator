# usage: awk -f contour-map.awk summary.log bandwidth <ftps | scale> <delay | throughput>
BEGIN {
    bw = ARGV[2]
    scale = ARGV[3]
    
    if (ARGC == 5 && ARGV[4] == "delay") {
        output = "delay"
    } else if (ARGC == 5 && ARGV[4] == "throughput") {
        output = "throughput"
    } else {
        print "Illegal arguments"
        exit
    }
    ARGC = 2
}
{
    if (NF == 10) {
        if ($1 == bw && $2 == scale) {
            if (output == "delay")
                printf("%s\t%s\t%s\n", $3, $4, $7);
            else
                printf("%s\t%s\t%s\n", $3, $4, $9);
        }
    }
}

