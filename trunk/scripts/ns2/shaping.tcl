set ns [new Simulator]
set startTime 0.0
set stopTime 120.0

global defaultRNG
$defaultRNG seed 12345
ns-random 12345

remove-all-packet-headers
add-packet-header IP TCP UDP

# Parse command line
if {$argc == 7 || $argc == 8} {
    # traffic patterns
    set cvec_file [lindex $argv 0]; # the cvec (connection vector) derived from packet trace
    set num_ftps [lindex $argv 1]
    set num_cbrs [lindex $argv 2]

    # link bandwidth
    set bw [bw_parse [lindex $argv 3]]
    
    # tcp mss
    set mss [lindex $argv 4]

    # shaping paramters
    set pct [lindex $argv 5]
    set qlen [lindex $argv 6]

    # suffix
    if {$argc == 7} {
        set suffix ""
    } else {
        set suffix [lindex $argv 7]
    }
} else {
    puts "Usage:"
    puts "  ns shaping.tcl cvec_file num_ftps num_cbrs bandwidth mss pct qlen [suffix]"
    puts "Example:"
    puts "  ns shaping.tcl inbound.cvec 0 1 4Mb 1460 0.85 2"
    puts "  (Use inbound.cvec with no ftp flow and one cbr flow; the downlink rate is"
    puts "   4Mbps; the tcp mss is 1460 bytes; 15% of the bandwidth is reserved and the"
    puts "   queue length of the token bucket is 2 packets)"
    puts "Notes:"
    puts "  - The queue length should not be set below 2 packets"
    puts "  - The suffix of trace files is optional, which is necessary when this script"
    puts "    is used in \"multi-threaded\" environments"
    exit 0
}

# Set nominal rtt to 100ms
set nominal_rtt 0.1
set bdp [expr round($bw * $nominal_rtt / (($mss + 40) * 8))]

# TCP settings
Agent/TCP set segsize_ [expr $mss]
Agent/TCP set packetSize_ [expr $mss]
Agent/TCP set window_ [expr $bdp * 16]
Agent/TCP set windowInit_ 4

Agent/TCP/FullTcp set segsize_ [expr $mss]
Agent/TCP/FullTcp set packetSize_ [expr $mss]
Agent/TCP/FullTcp set window_ [expr $bdp * 16]
Agent/TCP/FullTcp set windowInit_ 4
Agent/TCP/FullTcp set nodelay_ true; # disabling nagle
Agent/TCP/FullTcp set segsperack_ 2; # delayed ACKs
Agent/TCP/FullTcp set interval_ 0.1; # 100 ms

# Build topology
# +------------------------+                                              +-----------------------+
# | tmix inbound initiator |                                              | tmix inbound acceptor |
# |                o [n0]  |                       inbound traffic        |                o [n6] |
# |                |       |                    <-------------------      |                |      |
# | tmix delay box o [n1]  |                    <-------------------      | tmix delay box o [n7] |
# |                |       | virtual node                                 |                |      |
# |                +--------------o-------- o ===================== o ---------------------+      |
# |                |       |    [n3]     gateway     bottleneck    ISP    |                |      |
# | ftp / cbr node o [n2]  |              [n4]                     [n5]   | ftp / cbr node o [n8] |
# +------------------------+                                 500ms queue  +-----------------------+
#         Clients                                                       Servers
proc setup_scenario {} {
    global ns cvec_file num_ftps num_cbrs bw mss pct qlen suffix
    global n3 n4 n5
    
    set n0 [$ns node]; # inbound initiator
    set n1 [$ns Tmix_DelayBox];
    set n2 [$ns node]; # ftp / cbr client
    set n3 [$ns node];
    set n4 [$ns node];
    set n5 [$ns node];
    set n6 [$ns node]; # inbound acceptor
    set n7 [$ns Tmix_DelayBox];
    set n8 [$ns node]; # ftp / cbr server

    set local_bw [expr $bw * $pct]
    set local_queue $qlen
    set isp_queue [expr round($bw * 0.5 / (($mss + 40) * 8))]

    $ns duplex-link $n0 $n1 1000Mb 1ms DropTail
    $ns duplex-link $n1 $n3 1000Mb 1ms DropTail
    $ns duplex-link $n2 $n3 1000Mb 1ms DropTail

    $ns duplex-link $n3 $n4 $local_bw 1ms DropTail
    $ns duplex-link $n4 $n5 $bw 7ms DropTail

    $ns duplex-link $n5 $n7 1000Mb 1ms DropTail
    $ns duplex-link $n5 $n8 1000Mb 1ms DropTail
    $ns duplex-link $n6 $n7 1000Mb 1ms DropTail

    $ns queue-limit $n5 $n4 $isp_queue
    $ns queue-limit $n4 $n3 $local_queue

    if {$cvec_file != ""} {
        add_tmix $n0 $n6 $n7 $cvec_file
    }

    for {set i 1} {$i <= $num_ftps} {incr i 1} {
        add_ftp $n8 $n0 [expr 100000 + $i]; # lower flow ids are occupied by tmix traffic
    }

    for {set i 1} {$i <= $num_cbrs} {incr i 1} {
        add_cbr $n8 $n0 [expr $bw / 1000000.0 * 0.05]; # each cbr flow uses 5% of the total bandwidth
    }

    # Disable output in multi-threaded mode
    if {$suffix == ""} {
        puts "Setting up scenario..."
        puts [format "   - bandwidth = %.1f Mbps" [expr $bw / 1000000.0]]
        puts [format "   - %s" $cvec_file]
        puts [format "   - ftp flows = %d" $num_ftps]
        puts [format "   - cbr flows = %d" $num_cbrs]
        puts [format "   - used = %d%%" [expr round($pct * 100.0)]]
        puts [format "   - local queue = %d" $local_queue]
        puts "Running simulation..."
    }
}

# Add tmix
proc add_tmix {initiator acceptor delaybox filename} {
    global ns
    $delaybox set cvfile $filename [$initiator id] [$acceptor id]
    $delaybox set-lossless

    set t [new Tmix]
    $t set-init $initiator
    $t set-acc $acceptor
    $t set-ID 7
    $t set-cvfile $filename
    $ns at 0.0 "$t start"
}

# Add ftp
proc add_ftp {sender receiver flow_id} {
    global ns
    set tcp [new Agent/TCP/Newreno]
    set sink [new Agent/TCPSink]
    $ns attach-agent $sender $tcp
    $ns attach-agent $receiver $sink
    $ns connect $tcp $sink
    $tcp set fid_ $flow_id
    
    set ftp [new Application/FTP]
    $ftp attach-agent $tcp
    $ftp set type_ FTP
    $ns at 0.0 "$ftp start"
}

# Add cbr
proc add_cbr {sender receiver rate} {
    global ns
    set udp [new Agent/UDP]
    set null [new Agent/Null]
    $ns attach-agent $sender $udp
    $ns attach-agent $receiver $null
    $ns connect $udp $null

    set cbr [new Application/Traffic/CBR]
    $cbr set packetSize_ 200
    $cbr set interval_ [expr 0.0008 / $rate]
    $cbr attach-agent $udp
    $ns at 0.0 "$cbr start"
}

# Finish procedure
proc finish {} {
    global ns suffix
    $ns halt
    $ns flush-trace

    # Disable output in multi-threaded mode
    if {$suffix == ""} {
        puts "Done"
    }
    exit 0
}

# Start simulation
setup_scenario
if {$suffix == ""} {
    $ns trace-queue $n5 $n4 [open link-isp.tr w]
    $ns trace-queue $n4 $n3 [open link-local.tr w]
} else {
    $ns trace-queue $n5 $n4 [open [format "link-isp-%s.tr" $suffix] w]
    $ns trace-queue $n4 $n3 [open [format "link-local-%s.tr" $suffix] w]
}
$ns at $stopTime "finish"
$ns run
exit 0

