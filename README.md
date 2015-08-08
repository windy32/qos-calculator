# qos-calculator
A calculator that can help you set the traffic policing parameters for downstream / rx traffic.

<img src="https://github.com/windy32/qos-calculator/blob/master/web/calculator-page-1.png" />

<img src="https://github.com/windy32/qos-calculator/blob/master/web/calculator-page-2.png" />

There's binary compiled in Ubuntu 10.04 (32 bit) in the git repository. In other systems, you may need to compile from source. The source code is based on Qt 4.

**Note: due to the use of Tmix traffic generator, cloning the entire repository may take a few minutes (about 100 MB). For those who just simply want to try the calculator, please git archive the /src directory.**

### Lookup Table ###

You can use this lookup table if you don't want to compile from source. Note that in Linux tc commands you should use kbit (kilobit per second) instead of kb (kilobytes per second).

| Bandwidth | Token rate | Buffer size |
| --------- | ---------- | ------------|
| 1 Mbps    | 730 Kbps   | 4971 Bytes  |
| 2 Mbps    | 1490 Kbps  | 5232 Bytes  |
| 3 Mbps    | 2240 Kbps  | 5493 Bytes  |
| 4 Mbps    | 3050 Kbps  | 5754 Bytes  |
| 5 Mbps    | 3900 Kbps  | 6015 Bytes  |
| 6 Mbps    | 4770 Kbps  | 6276 Bytes  |
| 7 Mbps    | 5650 Kbps  | 6537 Bytes  |
| 8 Mbps    | 6560 Kbps  | 6798 Bytes  |
| 9 Mbps    | 7470 Kbps  | 7059 Bytes  |
| 10 Mbps   | 8390 Kbps  | 7320 Bytes  |
| 12 Mbps   | 10240 Kbps | 7842 Bytes  |
| 14 Mbps   | 12120 Kbps | 8364 Bytes  |
| 16 Mbps   | 14010 Kbps | 8886 Bytes  |
| 18 Mbps   | 15910 Kbps | 9408 Bytes  |
| 20 Mbps   | 17830 Kbps | 9930 Bytes  |
| 24 Mbps   | 21670 Kbps | 10974 Bytes |
| 28 Mbps   | 25540 Kbps | 12018 Bytes |
| 32 Mbps   | 29420 Kbps | 13062 Bytes |
| 36 Mbps   | 33320 Kbps | 14106 Bytes |
| 40 Mbps   | 37220 Kbps | 15150 Bytes |
| 50 Mbps   | 47010 Kbps | 17760 Bytes |
| 60 Mbps   | 56840 Kbps | 20370 Bytes |
| 70 Mbps   | 66690 Kbps | 22980 Bytes |
| 80 Mbps   | 76560 Kbps | 25590 Bytes |
| 90 Mbps   | 86450 Kbps | 28200 Bytes |
| 100 Mbps  | 96340 Kbps | 30810 Bytes |

### Wiki Pages ###

You may be interested in:

  * [Background information of this project](https://github.com/windy32/qos-calculator/wiki/Background-Information)
  * [Evidence of 80% / 90% principle](https://github.com/windy32/qos-calculator/wiki/Evidence-of-the-80%25---90%25-Principle)
