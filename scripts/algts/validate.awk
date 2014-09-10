BEGIN {
    print "htb (default)"
    print "==================================================="
    print "Bandwidth | Threads | Avg Delay | Max Delay | Rate"
    print "---------------------------------------------------"
    algorithm = 0 # default
}
{
    if ($1 == "Bandwidth" && $5 == "threads") {
        bandwidth = $3
        threads = $7
        printf("%9d   %7d   ", bandwidth, threads);
    }
    
    if ($1 == 1 && $2 == "|") {
        max = 0
        avg = 0
        rate = 0
    }

    if ($1 >= 1 && $1 <= 10 && $2 == "|") {
        max += $5
        avg += $7
        rate += $9
    }

    if ($1 == 10 && $2 == "|") 
    {
        printf("%9d   %9d   %4d\n", avg / 10, max / 10, rate / 10);
    }
    
    if (bandwidth == 30000 && threads == 8)
    {
        if ($1 == 10 && $2 == "|") 
        {
            algorithm += 1

            if (algorithm <= 4)
            {
                print "=================================================="
                print ""
            }

            if (algorithm == 1)
            {
                print "htb + sfq"
            }
            else if (algorithm == 2)
            {
                print "htb + codel"
            }
            else if (algorithm == 3)
            {
                print "htb + fq_codel"
            }
            else if (algorithm == 4)
            {
                print "htb + bfifo"
            }
            
            if (algorithm <= 4)
            {
                print "=================================================="
                print "Bandwidth | Threads | Avg Delay | Max Delay | Rate"
                print "--------------------------------------------------"
            }
        }
    }
}

END {
    print "=================================================="
}

