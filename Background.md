### Background Information ###

<font color='#0068b7'>
<b>This wiki page describes the motivation for creating the project, as well as its precursor, the algts project. It may seem to be strange that I created the algts testbed when there are a lot of free traffic generation systems available on the Internet. The reasons are explained here.</b>
</font>

It all begins when I was an undergraduate at Nanjing University of Aeronautics and Astronautics. At that time my roommates and me decided to purchase the Internet service based on ADSL with a downstream rate of 2 Mbps and an upstream rate of 256 Kbps at a cost of 180 RMB (about 26.5 USD) per month. The link rate is rather low, however at that time it was the only acceptable choice.

As I was the network administrator of our dormitory, it became the start of the battle.

A good feature of a DSL link is that when there's little traffic, the round trip time (ping value) to local websites (e.g., www.baidu.com in China) is expected to be low and stable. When there're some bulk data transfers, however, the delay may be easily raised to serveral hundred milliseconds to one second. As the link rate is rather low, this phenomenon may happen frequently when a single TCP transfer starts, which is not likely to happen in backbone links.

It was really annoying when there were complaints about the network all day, so I started searching for the solution. The "QoS" page in home routers' web configuration interface seemed to be a good start point. Meanwhile, the QoS features in a lot of open source Linux router firmwares seemed to be attractive.

I started to try different wireless routers, and then flash different open source firmwares. In that period I tested a list of routers including

  * TP-Link WR541G+
  * TP-Link WR741N
  * D-Link DIR-600 B2
  * Netgear WGR614 v6
  * Netcore NW618

And also a list of open source firmwares including

  * DD-WRT
  * Tamato (Original, RAF Mod, Speed Mod, DualWAN Mod)
  * OpenWRT
  * WayOS

However, I faild to get an satisfying solution. The earlier version of TP-Link's home routers has a QoS page like this.

<img src='http://qos-calculator.googlecode.com/svn/trunk/web/tp-link.png'>

It was also suggested that only the "maximum bandwidth" mode works effectively, and users should avoid using the "minimum bandwidth" mode. In later versions, this suggestion was removed. However none of the users know what's happened to the system.<br>
<br>
The earlier version of ASUS' home routers has an "EzQoS" system, in which different traffic is classified by ports and assigned to different priority levels. However, as no detailed explanation about what a "priority level" actually is, the function of the system is rather vague.<br>
<br>
<img src='http://qos-calculator.googlecode.com/svn/trunk/web/asus.png'>

I spent plenty of time trying different open source firmwares and different QoS configurations. The advantage of the open source firmwares is that they have much more features, more flexibiliy, more discussions and everything is open. The tomato firmware by Jonathan Zarate is one of the most popular.<br>
<br>
<img src='http://qos-calculator.googlecode.com/svn/trunk/web/tomato-1.png'>
<img src='http://qos-calculator.googlecode.com/svn/trunk/web/tomato-2.png'>

The QoS page in Tamato is presented above. There're much more features than most factory firmwares, isn't it?<br>
<br>
<ul><li>Classification by ip address, protocol and port<br>
</li><li>Classification by number of bytes transferred<br>
</li><li>Classification by the IPP2P module and the Layer-7 filter<br>
</li><li>Prioritization for special packets (for example, ACK, SYN)<br>
</li><li>TCP Vegas</li></ul>

As a result, Tomato became my working system for the longest period of time, which works although not perfectly. The most important parameter of the QoS system, max bandwidth, is selected according to the well known 80 / 90 percent principle, which is discussed in another wiki page.<br>
<br>
Most users just configure QoS through the Web interface, thus rich features in the web interface has become significantly important for most users. However still there're a few users who decided to dig into the system for even better flexibility and performance. Most the work are based on Shell scripts. On the <a href='http://bbs.dualwan.cn/'>Tomato DualWAN Mod forum</a> (Chinese), there're plenty of scripts shared by different users.<br>
<br>
For example, one of the scripts originally writtern by zhoutao0712 and then modified by cmt1978 is like this (available at <a href='http://bbs.dualwan.cn/viewthread.php?tid=110802'>http://bbs.dualwan.cn/viewthread.php?tid=110802</a> (in Chinese)):<br>
<br>
<pre><code><br>
#copyright by zhoutao0712<br>
UIP="192.168.1."<br>
NET="192.168.1.0/24"<br>
IPS="2"<br>
IPE="254"<br>
UP=35<br>
DOWN=400<br>
UPLOADR=3<br>
UPLOADC=8<br>
DOWNLOADR=20<br>
DOWNLOADC=$(($DOWN*9/10))<br>
<br>
modprobe imq<br>
modprobe ipt_IMQ<br>
modprobe ipt_web<br>
modprobe ipt_length<br>
modprobe ipt_hashlimit<br>
modprobe xt_IMQ<br>
modprobe xt_length<br>
modprobe xt_hashlimit<br>
ifconfig imq1 up<br>
ifconfig imq0 up<br>
iptables -t mangle -N QOSDOWN<br>
iptables -t mangle -N QOSUP<br>
iptables -t mangle -A PREROUTING -s $NET -d ! $NET -p ! icmp -j QOSUP<br>
iptables -t mangle -I POSTROUTING -d $NET -s ! $NET -p ! icmp -j QOSDOWN<br>
iptables -t mangle -A OUTPUT -o br0 -j ACCEPT<br>
iptables -t mangle -A INPUT -i br0 -j ACCEPT<br>
iptables -t mangle -A OUTPUT -j QOSUP<br>
iptables -t mangle -A INPUT -j QOSDOWN<br>
iptables -t mangle -A QOSDOWN -j IMQ --todev 0<br>
iptables -t mangle -A QOSUP -j IMQ --todev 1<br>
<br>
iptables -t mangle -A FORWARD -p icmp -j RETURN<br>
iptables -t mangle -A QOSUP -p tcp --syn -m connlimit --connlimit-above 100 -j RETURN<br>
iptables -t mangle -A QOSUP -m state --state NEW -p ! tcp -m connlimit --connlimit-above 200 -j DROP<br>
iptables -t mangle -N GAME_BURST<br>
iptables -t mangle -A QOSDOWN -m length --length 256: -j GAME_BURST<br>
iptables -t mangle -A GAME_BURST -m hashlimit --hashlimit 10/sec --hashlimit-burst 100 --hashlimit-mode dstip --hashlimit-name game_burst -j RETURN<br>
iptables -t mangle -A GAME_BURST -m recent --rdest --name game_burst --set -j RETURN<br>
iptables -t mangle -A QOSDOWN -m recent --rdest --name game_burst ! --rcheck  --seconds 5 -j MARK --set-mark-return 253<br>
iptables -t mangle -A QOSUP -p tcp -m mport --dports 80,443 -j BCOUNT<br>
iptables -t mangle -A QOSUP -p tcp -m mport --dports 80,443 -m bcount --range :102400 -j MARK --set-mark-return 255<br>
iptables -t mangle -N CONNLMT<br>
iptables -t mangle -I QOSUP -m state --state NEW -s $NET -j CONNLMT<br>
iptables -t mangle -I CONNLMT -p tcp -m web --path ".jpg .htm .gif .swf .html .xml .asp .js .css" -j RETURN<br>
iptables -t mangle -I CONNLMT -p udp -m mport --sports 53,67,68 -j RETURN<br>
iptables -t mangle -I CONNLMT -p udp -m mport --dports 53,67,68,7000,8000:8005  -j RETURN<br>
iptables -t mangle -I CONNLMT -p tcp -m mport --dports 25,80,110,443 -j RETURN<br>
iptables -t mangle -A QOSDOWN -j MARK --ipaddr 1<br>
iptables -t mangle -A QOSUP -j MARK --ipaddr 0<br>
<br>
tc qdisc del dev imq0 root<br>
tc qdisc del dev imq1 root<br>
tc qdisc add dev imq0 root handle 1: htb<br>
tc qdisc add dev imq1 root handle 1: htb<br>
tc class add dev imq1 parent 1: classid 1:1 htb rate $((UP))kbps<br>
tc class add dev imq0 parent 1: classid 1:1 htb rate $((DOWN))kbps<br>
<br>
tc class add dev imq1 parent 1:1 classid 1:254 htb rate $((UP))kbps quantum 12000 prio 0<br>
tc class add dev imq1 parent 1:1 classid 1:255 htb rate $((UP/5))kbps ceil $((UP/2))kbps quantum 2000 prio 3<br>
tc filter add dev imq1 parent 1:0 protocol ip prio 5 handle 255 fw flowid 1:255<br>
tc filter add dev imq1 parent 1:0 protocol ip prio 4 handle 254 fw flowid 1:254<br>
tc class add dev imq0 parent 1:1 classid 1:254 htb rate $((DOWN))kbps quantum 12000 prio 0<br>
tc class add dev imq0 parent 1:1 classid 1:255 htb rate $((DOWN*20/100))kbps ceil $((DOWN*80/100))kbps prio 3<br>
tc filter add dev imq0 parent 1:0 protocol ip prio 5 handle 255 fw flowid 1:255<br>
tc filter add dev imq0 parent 1:0 protocol ip prio 4 handle 254 fw flowid 1:254<br>
tc class add dev imq0 parent 1:1 classid 1:253 htb rate $((DOWN/2))kbps quantum 5000 prio 1<br>
tc filter add dev imq0 parent 1:0 protocol ip prio 5 handle 253 fw flowid 1:253<br>
<br>
i=$IPS;<br>
while [ $i -le $IPE ]<br>
do<br>
tc class add dev imq1 parent 1:1 classid 1:$i htb rate $((UPLOADR))kbps ceil $((UPLOADC))kbps quantum 1000 prio 5<br>
tc qdisc add dev imq1 parent 1:$i handle $i: sfq perturb 15<br>
tc filter add dev imq1 parent 1:0 protocol ip prio 6 handle $i fw classid 1:$i<br>
tc class add dev imq0 parent 1:1 classid 1:$i htb rate $((DOWNLOADR))kbps ceil $((DOWNLOADC))kbps quantum 1000 prio 5<br>
tc qdisc add dev imq0 parent 1:$i handle $i: sfq perturb 15<br>
tc filter add dev imq0 parent 1:0 protocol ip prio 6 handle $i fw classid 1:$i<br>
i=expr $i + 1<br>
done<br>
<br>
cat &gt;/tmp/qos_scheduler &lt;&lt;"EOF"<br>
echo 1 &gt;/tmp/state_scheduler<br>
if [ $(cat /proc/net/arp|fgrep -c 0x2) -le 2 ]<br>
then<br>
ifconfig imq0 down<br>
exit<br>
fi<br>
if [ $(ifconfig |grep -c imq0) -eq 0 ]<br>
then<br>
ifconfig imq0 up<br>
fi<br>
ip neigh flush dev $(nvram get lan_ifname)<br>
EOF<br>
chmod +x /tmp/qos_scheduler<br>
cru d qos_scheduler<br>
echo -e '*/1 * * * *  sh /tmp/qos_scheduler #qos_scheduler#' &gt;&gt;/tmp/var/spool/cron/crontabs/root</code></pre>

The script seems to be rather complex. And there're still a number of scripts which are even more complicated. Most the the scripts claim to be <b>Smart Qos Scripts</b>, which means that all you need to do is to enable it without having to tune the parameters.<br>
<br>
At that time there was also a firmware system known by many Chinese Linux players which claimed to support smart QoS, which is from a unfamous Chinese network device manufactory called WayOS. The firmware system is then also referred to as the wayos system. It is believed by many users that the system is based on Tamato, bacause its supported devices are exactly the same with the ones supported by Tomato. However, as the wayos system is not an open-source project, the only way to evaluate it is to have a try.<br>
<br>
As there were so many choices (e.g., factory firmwares, open-source firmwares with  a list of configurable settings, smart QoS scripts that does not need tuning), I became totally doubtful about all the options available. Clearly, trying them one by one is not an efficient solution. Then I decided to develop a <b>benchmark system</b> for the QoS performance for routers in home network environments.<br>
<br>
The basic idea of calculating scores is quite straightforward:<br>
<br>
<ul><li>Higher link utilization generally leads to higher scores<br>
</li><li>If a flows of a realtime application suffer from high latency and jitter, the score would be lower<br>
</li><li>If the allocation of available bandwidth is unfair (e.g., the rate of bulk data transfers of user A is 10 times higher than that of user B just because A has used 10 threads for his download task but B used only one), the score would be lower</li></ul>

A score related to traffic rate is relatively easy calculate, but for realtime applications it would be a bit difficult. As a result, we may just provide an interface for creating the function curve and then let users to decide which function to use.<br>
<br>
For realtime applications that cares only about the latency, the interface may look like this:<br>
<br>
<img src='http://qos-calculator.googlecode.com/svn/trunk/web/rt-1.png'>

And for applications that are concerned about both the latency and packet loss, it may look like this:<br>
<br>
<img src='http://qos-calculator.googlecode.com/svn/trunk/web/rt-2.png'>

In the left image of the latter, the x axis is the latency in milliseconds, and the y axis is the packet loss rate in 1%. The max score is 1.0, and minimum 0.0.<br>
<br>
In my imagination, the final system would be a benchmark system with data management functionality. The report generated may looks like this (the scores below are imaginary):<br>
<br>
<table cellpadding='5' cellspacing='0' border='1'>
<tr>
<blockquote><th>Router Model</th>
<th>Firmware</th>
<th>Bandwidth</th>
<th>QoS Scheme</th>
<th>Parameters</th>
<th>Score</th>
<th>Notes</th>
</tr>
<tr>
<th>Throughput</th>
<th>Delay Control</th>
<th>Fairness</th>
<th>Details</th>
</tr>
<tr>
<td>D-Link DIR-600 B2</td>
<td>Factory Default (2.01)</td>
<td>2 Mbps / 256 Kbps</td>
<td>None</td>
<td>-</td>
<td>0.97</td>
<td>0.32</td>
<td>0.27</td>
<td>...</td>
<td></td>
</tr>
<tr>
<td>D-Link DIR-600 B2</td>
<td>DD-WRT (rev. 14896)</td>
<td>2 Mbps / 256 Kbps</td>
<td>WAN / HTB</td>
<td>1700 / 240</td>
<td>0.89</td>
<td>0.77</td>
<td>0.69</td>
<td>...</td>
<td></td>
</tr>
<tr>
<td>Netcore NW618</td>
<td>WayOS BCM (2.1.115)</td>
<td>2 Mbps / 256 Kbps</td>
<td>Automatic</td>
<td>-</td>
<td>0.95</td>
<td>0.82</td>
<td>0.77</td>
<td>...</td>
<td></td>
</tr>
<tr>
<td>Netgear WGR614 v6</td>
<td>Tomato DualWAN 1.28.0542</td>
<td>2 Mbps / 256 Kbps</td>
<td>QoS Script v4</td>
<td>...</td>
<td>0.85</td>
<td>0.66</td>
<td>0.71</td>
<td>...</td>
<td></td>
</tr></table></blockquote>

Well, the blueprint may sound good. However, after develop for months, I realized that I have lumped too many features together into a single system. So it is not likely to be finished in a reasonable period of time.<br>
<br>
The features in the first blueprint include:<br>
<br>
<ul><li>Traffic generation and measurements<br>
</li><li>Visualized scenario edit (i.e., at what time which user starts an applicaiton)<br>
</li><li>Visualized RND edit<br>
</li><li>A benchmark system with GUI<br>
</li><li>Automatic configuration of routers (via telnet, ssh, etc. Of course it works only for open source firmwares)<br>
</li><li>Management for experiment records</li></ul>

As all the operations are integrated into a single graphics user interface, the complexity of the entire system exploded and I was unable to fix it.<br>
<br>
This is the begin of the algts project (<a href='https://code.google.com/p/algts'>https://code.google.com/p/algts</a>).<br>
<br>
When realized that the initial task is impossible, I decided to remove all of the source code related to GUI. The benchmark system has also been removed. As a result, algts became a script driven traffic generator with some utilities (e.g., traffic measurement, trace generation, telnet / ssh operations). It still works, but it's not the system that I've imagined any longer. The major problem now is that why is it necessary develop such a system when there're so many open source traffic generators available.<br>
<br>
Well the initial blueprint is nice, but the result was unexpected. I just ignored the questions and went on.<br>
<br>
Although the algts project has been rather different from the initial blueprint, it's not bad either. The major advantage is that the experiments can be automatic. Under Windows when you use IxChariot as the traffic generator, it's impossible and you have to click on menus and buttons every few minites. Under Linux where other script driven traffic generators are available, the good news is that you don't have to learn about Bash scripts and how to send data to a telnet / ssh console in a script. Such hacking may be simple or difficult, but under algts, there's totally nothing to worry. Evevything is prepared and easy to use, ready for automation. You can also emulate a number of hosts with different IP addresses within a single host, which is not supported by most other traffic generators.<br>
<br>
I used algts to study the downstream QoS problem. Simulation experiments are under ns-2, the algts is responsible for real experiments. As a result, you can find a "ns2" folder under "utils" in algts' svn repositoty, in which there are a few ns-2 scripts and logs.<br>
<br>
As the reviews suggested, it is not really necessary to include the real experiments if the results are similar. Although the algts platform has been in company with me for a long time, I have to admit that it's true: there's actually no need to mention the algts platform.<br>
<br>
Some of the ns-2 scripts from the algts repository are moved here for further improvements. This project is intended to be the final work of the studies related to downstream QoS. It's a calculator used to decide token bucket parameters. To make it more useful, it can also generate scripts like the "WRT54G Script Generator".<br>
<br>
This is the end of the story. Maybe it will continue, so let's see.