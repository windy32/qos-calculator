BEGIN {
    prev_ftp = ""

    # The best buffer size (in packets) for different bandwidth
    QLEN[0.5] = 3
    QLEN[1] = 3
    QLEN[2] = 3
    QLEN[3] = 4
    QLEN[4] = 4
    QLEN[5] = 4
    QLEN[6] = 4
    QLEN[8] = 5
    QLEN[10] = 5
    QLEN[12] = 5
    QLEN[14] = 6
    QLEN[16] = 6
    QLEN[20] = 7
    QLEN[24] = 7
    QLEN[30] = 8
    QLEN[40] = 10
    QLEN[50] = 12
    QLEN[60] = 14
    QLEN[80] = 16 # 17
    QLEN[100] = 20 # 21
}
{
    if (NF == 10)
    {
        if (QLEN[$1] == $4) # Match queue length
        {
            if (prev_ftp != "" && $2 != prev_ftp) # New {bw, ftp}
            {
                if (num_70ms > 0)
                    print record_70ms
                else if (num_80ms > 0)
                    print record_80ms
                else if (num_90ms > 0)
                    print record_90ms
                else
                    print "-"
                
                num_90ms = 0
                num_80ms = 0
                num_70ms = 0
                record_90ms = ""
                record_80ms = ""
                record_70ms = ""
            }
            
            if ($7 <= 90)
            {
                record_90ms = $0
                num_90ms += 1
            }
            if ($7 <= 80)
            {
                record_80ms = $0
                num_80ms += 1
            }
            if ($7 <= 70)
            {
                record_70ms = $0
                num_70ms += 1
            }
                        
            prev_ftp = $2
        }
    }
}

END {
    if (num_70ms > 0)
        print record_70ms
    else if (num_80ms > 0)
        print record_80ms
    else if (num_90ms > 0)
        print record_90ms
    else
        print "-"
}

