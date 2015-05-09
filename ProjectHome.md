### QoS Calculator ###

A calculator that can help you set the traffic policing parameters for downstream / rx traffic.

<img src='http://qos-calculator.googlecode.com/svn/trunk/web/calculator-page-1.png'>

<img src='http://qos-calculator.googlecode.com/svn/trunk/web/calculator-page-2.png'>

There's binary compiled in Ubuntu 10.04 (32 bit) in the svn repository. In other systems, you may need to compile from source. The source code is based on Qt 4.<br>
<br>
<font color='#0068b7'><b>Note: due to the use of Tmix traffic generator, checking out the entire trunk may take a few minutes (about 100 MB). For those who just simply want to try the calculator, please check out /trunk/src</b></font>

<h3>Lookup Table</h3>

You can use this lookup table if you don't want to compile from source. Note that in Linux tc commands you should use <b>kbit</b> (kilobit per second) instead of <b>kb</b> (kilobytes per second).<br>
<br>
<table><thead><th> <b>Bandwidth</b> </th><th> <b>Token rate</b> </th><th> <b>Buffer size</b> </th></thead><tbody>
<tr><td> 1 Mbps </td><td> 730 Kbps </td><td> 4971 Bytes </td></tr>
<tr><td> 2 Mbps </td><td> 1490 Kbps </td><td> 5232 Bytes </td></tr>
<tr><td> 3 Mbps </td><td> 2240 Kbps </td><td> 5493 Bytes </td></tr>
<tr><td> 4 Mbps </td><td> 3050 Kbps </td><td> 5754 Bytes </td></tr>
<tr><td> 5 Mbps </td><td> 3900 Kbps </td><td> 6015 Bytes </td></tr>
<tr><td> 6 Mbps </td><td> 4770 Kbps </td><td> 6276 Bytes </td></tr>
<tr><td> 7 Mbps </td><td> 5650 Kbps </td><td> 6537 Bytes </td></tr>
<tr><td> 8 Mbps </td><td> 6560 Kbps </td><td> 6798 Bytes </td></tr>
<tr><td> 9 Mbps </td><td> 7470 Kbps </td><td> 7059 Bytes </td></tr>
<tr><td> 10 Mbps </td><td> 8390 Kbps </td><td> 7320 Bytes </td></tr>
<tr><td> 12 Mbps </td><td> 10240 Kbps </td><td> 7842 Bytes </td></tr>
<tr><td> 14 Mbps </td><td> 12120 Kbps </td><td> 8364 Bytes </td></tr>
<tr><td> 16 Mbps </td><td> 14010 Kbps </td><td> 8886 Bytes </td></tr>
<tr><td> 18 Mbps </td><td> 15910 Kbps </td><td> 9408 Bytes </td></tr>
<tr><td> 20 Mbps </td><td> 17830 Kbps </td><td> 9930 Bytes </td></tr>
<tr><td> 24 Mbps </td><td> 21670 Kbps </td><td> 10974 Bytes </td></tr>
<tr><td> 28 Mbps </td><td> 25540 Kbps </td><td> 12018 Bytes </td></tr>
<tr><td> 32 Mbps </td><td> 29420 Kbps </td><td> 13062 Bytes </td></tr>
<tr><td> 36 Mbps </td><td> 33320 Kbps </td><td> 14106 Bytes </td></tr>
<tr><td> 40 Mbps </td><td> 37220 Kbps </td><td> 15150 Bytes </td></tr>
<tr><td> 50 Mbps </td><td> 47010 Kbps </td><td> 17760 Bytes </td></tr>
<tr><td> 60 Mbps </td><td> 56840 Kbps </td><td> 20370 Bytes </td></tr>
<tr><td> 70 Mbps </td><td> 66690 Kbps </td><td> 22980 Bytes </td></tr>
<tr><td> 80 Mbps </td><td> 76560 Kbps </td><td> 25590 Bytes </td></tr>
<tr><td> 90 Mbps </td><td> 86450 Kbps </td><td> 28200 Bytes </td></tr>
<tr><td> 100 Mbps </td><td> 96340 Kbps </td><td> 30810 Bytes </td></tr></tbody></table>

<h3>Wiki Pages</h3>

You may be interested in:<br>
<br>
<ul><li><a href='https://code.google.com/p/qos-calculator/wiki/Background'>Background information of this project</a>
</li><li><a href='https://code.google.com/p/qos-calculator/wiki/Evidence'>Evidence of 80% / 90% principle</a>