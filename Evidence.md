<font color='#0068b7'><b>Claiming that the 80% / 90% principle is widely used, and that the correct sizing of the buffers of token buckets is not performed by most users without proof is not good. As a result, we collected a lot of evidence and related discussions on the Internet.</b> </font>

There seem to be no formal studies about this topic, thus most discussions are from forums, blogs, and wiki pages of related products and open-source projects.

In the tables below, "shaping" stands for the token bucket rate for outgoing (tx) traffic, and "policing" for incoming (rx) traffic. Note that the "police" here does not necessarily mean that incoming traffic are policed with the "tc filter ... police ..." command; it can also be redirected to a virtual device, e.g., imq or ifb, and then be shaped with htb.

Some of the discussions include both traffic shaping and policing paramters; some scripts have introduced more complex strategies setting the token rate, and they can not be represented by a single percentage.

We can find that most of the suggested percentage is between 80% and 90%, and suggested buffer sizes of the token bucket algorithm include:

  * 128 packets
  * 1000 packets
  * 10240 packets
  * max(49 ms, 6.25 KB)
  * other (10 / 150 / 200 KB, 200 / 500 / 700 packets, 1000 ms)

### Methodology for Collecting the Data ###

A lot of discussions come from the forums of 3rd-party open source projects like DD-WRT, OpenWRT, and Tomato. We first use the internal search function in these forums to locate the posts with keywords like:

  * QoS
  * Quality of Service
  * Bandwidth Control
  * Speed Limit
  * HTB
  * VoIP
  * BitTorrent

Then we did the same for discussions out of these forums with Google. The first 20 pages contain a lot of related discussions, but it then becomes quite difficult to find a related topic.

The search result from Google is not stable. As a result, the list may become quite different several years later. However, for such **anecdotal** evidence, it's the only way we can think of.

### Different Approaches ###

In the calculator application, we can see that the token bucket parameters depend only on the link bandwidth. This strategy is based on numerous simulation experiments based on ns-2 that we have performed.

However, in some of the related discussions, we have also found different approaches. For example, in http://bbs.dualwan.cn/viewthread.php?tid=20122, it is suggested that:

token\_rate = bandwidth `*` (30 + online\_hosts) / (20 + online\_hosts `*` 8)

We cannot find support for this equation. It seems to be quite arbitrary but the basic idea is easy to understand: when there are more online hosts, the traffic load is heavier, so the token rate is lower. However, in the calculator application, the token bucket parameters only depend on the link bandwidth. Traffic load is not considered (actually it has been taken into consideration) and the equation has only one dependency.

We also noticed that a lot of users and router manufactures are against downstream traffic policing. An example is in the support page of Netgear routers:

http://kb.netgear.com/app/answers/detail/a_id/24266/~/how-to-enable-quality-of-service-%28qos%29-on-routers-with-the-genie-user-interface

In this page, we can only find options about tx traffic control (traffic shaping). For tx traffic, we have full control over the packets, and we can configure priority queues and fair queues to improve quality of service, which is relatively easy.

For rx traffic, we don't have direct control over the traffic, but traffic control is still possible. We believe that the negative options will disappear as related research and real-world testings continue.

### Open-source Firmwares and Script Generators ###

| **Target** | **Date** | **Shaping** | **Policing** | **Buffer Size** | **Path** |
|:-----------|:---------|:------------|:-------------|:----------------|:---------|
| Tomato 1.28 | June 28, 2010 | - | - | max(49ms, 6.25 KB) | /src/router/rc/qos.c |
| DD-WRT HTB rev 20939 | March 15, 2013 | - | - | sfq (128p) / codel (1000p) / fq\_codel (10240p) | /src/router/iproute2/svqos.htb |
| DD-WRT HFSC rev 20939 | March 15, 2013 | - | - | sfq (128p) / codel (1000p) / fq\_codel (10240p) | /src/router/iproute2/svqos.hfsc |
| OpenWRT 12.09 | April 25, 2013 | - | - | fq\_codel (10240p) | /package/qos-scripts |
| WRT54 Script Generator 1.02 | 2007 | - | - | sfq (128p) / default (1000p) | - |

<font color='#0068b7'><b>This is the most important category.</b></font>

When users want to optimize their home networks, most of them will find that there's limited functionality available in factory firmwares, and then they begin to try open-source firmwares like DD-WRT, OpenWRT, Tomato, etc. The users who enjoy flashing routers and trying different open-source firmwares are only a small fraction of the entire user group, and they are often referred to as "geeks".

As a result, most of the discussions appear in a few technical blogs and forums that are not well known by most ordinary users.

Most "geeks" simply use the features provided in the web interface of different firmwares. That's why we consider it the most important. However, there seem to be a lot of inappropriate settings of the buffer size.

For example, in earlier version of DD-WRT, when the user enables downstream bandwidth limiting, and then added some priority classes (e.g., Premium, Express, Standard, or Bulk), an sfq is attached to each htb leaf class automatically. As each sfq has a buffer of 128 packets, the entire bufffer may be 512 packets or more.

In the "WRT54 Script Generator" known by many Linux router players, it's possible to create an htb leaf node for each user (identified by IP or MAC address). Users can either attach an sfq to each htb leaf, or just leave it alone. For a network with n users, the former choice leads to a total buffer of 128 `*` n packets, and for the latter, 1000 `*` n packets. Clearly, it does not make sense to configure so large a buffer. When different users start bulk transfers at the same time, the htb has to absorb too many packets before taking a drop, which leads to significant delay and jitter.

Similarly, for downstream traffic, the deployment of codel and fq\_codel may lead to large jitter due to bursts of traffic.

The Tomato firmware police traffic in a smarter way. The buffer size of its setting is 49ms (with a lower bound of 6.25 KB), which grows linearly when the token rate increases. The basic idea that a higher rate needs a larger buffer is correct, but the equation needs to be verified.

### Forum Posts ###

| **Title** | **Website** | **Shaping** | **Policing** | **Buffer Size** | **In Reply** |
|:----------|:------------|:------------|:-------------|:----------------|:-------------|
| [My "custom" QoS that really works](http://www.dd-wrt.com/phpBB2/viewtopic.php?t=3100) | www.dd-wrt.com | 85% | 85% | 200p, 700p, 1000p |  |
| [R7000 & QoS - recommended settings for games/media strea](http://www.dd-wrt.com/phpBB2/viewtopic.php?p=905429) | www.dd-wrt.com | 80% - 95% | 80% - 95% | fq\_codel (10240p) |  |
| [Vonage and QoS finally figured out!!](http://www.dd-wrt.com/phpBB2/viewtopic.php?p=174421) | www.dd-wrt.com | 80% | 90% | sfq (128p) |  |
| [Using QOS - Tutorial and discussion](http://www.linksysinfo.org/index.php?threads/using-qos-tutorial-and-discussion.28349/) | www.linksysinfo.org | 85% | 66% - 80% | - |  |
| [Can Any Router Guarantee Bandwidth For VoIP?](http://ask.slashdot.org/story/08/06/26/2156201/can-any-router-guarantee-bandwidth-for-voip) | ask.slashdot.org | 75% - 90% | - | - |  |
| [Why BitTorrent Causes Latency and How To Fix It](http://tech.slashdot.org/story/08/06/01/2225228/why-bittorrent-causes-latency-and-how-to-fix-it) | tech.slashdot.org | - | - | sfq (128p) / default (1000p) | Yes |
| [Does setting QoS to 80% of bandwidth limit maximum speed?](http://www.dslreports.com/forum/r19838037-Does-setting-QoS-to-80-of-bandwidth-limit-maximum-speed-) | www.dslreports.com | 80% | - | - |  |
| [Asterisk: QoS question with SpeedBoost on Rogers](http://www.dslreports.com/forum/r26109676-Asterisk-QoS-question-with-SpeedBoost-on-Rogers) | www.dslreports.com | 80% | 80% | - |  |
| [7800N QoS Rules](http://forums.whirlpool.net.au/archive/1746121) | forums.whirlpool.net.au | 80% | - | - |  |
| [Any consumer router than can handle 100mbps+ with QoS on?](http://forums.anandtech.com/showthread.php?t=2342285) | forums.anandtech.com | - | 80% | - |  |
| [Improve Your Bandwidth-Heavy Activities with Router Tweaks](http://lifehacker.com/5710804/improve-your-bandwidth-heavy-activities-with-router-tweaks) | lifehacker.com | 90% - 95% | - | - | Yes |
| [QOS just for p2p downloads/streaming?](https://www.gargoyle-router.com/phpbb/viewtopic.php?f=5&t=2049) | www.gargoyle-router.com | 90% | above 75% | - | Yes |
| [Guide for QoS?](http://forums.smallnetbuilder.com/showthread.php?t=7809) | forums.smallnetbuilder.com | 70% | - | - |  |
| [Voip and QOS](http://www.actuarialoutpost.com/actuarial_discussion_forum/archive/index.php/t-241559.html) | www.actuarialoutpost.com | 80% - 90% | 80% - 90% | - | Yes |
| [Properly setup QOS engine](http://forums.dlink.com/index.php?action=printpage;topic=46039.0) | forums.dlink.com | 80% | - | - |  |
| [qos settings for Wireless Router/Adp WIZARD INTEX](http://www.ulop.net/forum/4-lounge/558495-qos-settings-for-wireless-router-adp-wizard-intex) | www.ulop.net | 80% | 80% | - | Yes |
| [QoS is not functioning correctly.](http://www.myopenrouter.com/forum/thread/31357/QoS-is-not-functioning-correctly./) | www.myopenrouter.com | - | 80% | - |  |
| [MJ & Actiontec Router QOS ](http://www.phoneservicesupport.com/mj-actiontec-router-qos-t12036.html) | www.phoneservicesupport.com | 80% | 80% | - | Yes |
| [Request suggestion to improve WISP Ooma VOIP call quality using a Linksys WRT54G v5?](http://community.linksys.com/t5/Wireless-Routers/Request-suggestion-to-improve-WISP-Ooma-VOIP-call-quality-using/td-p/534448) | community.linksys.com | 80% | - | - |  |
| [Skype and QOS](http://mybroadband.co.za/vb/showthread.php/545501-Skype-and-QOS) | mybroadband.co.za | 80% | 80% | sfq (128p) | Yes |
| [QoS work for any of you?](http://www.rage3d.com/board/showthread.php?t=33873855) | www.rage3d.com | 80% | - | - |  |
| [Engin VOIP + Billion 7402 + Help me setup QoS so my calls arn't choppy.](http://forums.overclockers.com.au/showthread.php?t=441200) | forums.overclockers.com.au | 80% | 80% | - | Yes |
| [Slight lag when running QoS/ Tomato/DD-WRT!](http://forums.na.leagueoflegends.com/board/showthread.php?t=97116) | forums.na.leagueoflegends.com | 80% | 80% | - | Yes |
| [QOS Speed Limits](http://tomato.groov.pl/?page_id=334&bugerator_nav=display&bug_project=1&issue=256) | tomato.groov.pl | 80% | 80% | max(49ms, 6.25 KB) | Yes |
| [Sharing an Internet Connection Evenly Via a Linksys Router](http://forums.stevehoffman.tv/threads/sharing-an-internet-connection-evenly-via-a-linksys-router.203605/) | forums.stevehoffman.tv | 80% | - | - | Yes |
| [Should I use QoS on my home router? Is it worth it?](http://hardforum.com/showthread.php?t=1454426) | hardforum.com | 80% - 90% | 80% - 90% | max(49ms, 6.25 KB) |  |
| [Here are the QoS settings I use on my Actiontec with VZ FiOS. Go to the QoS tab. Select](http://www.vonage-forum.com/Here_are_the_QoS_settings_I_use_on_my_Actiontec_with_VZ_FiOS_Go_to_the_QoS_tab_Select-post-98631.html) | www.vonage-forum.com | 80% | 80% | - |  |
| [Does anyone really understand how HFSC scheduling in Linux/BSD works?](http://serverfault.com/questions/105014/does-anyone-really-understand-how-hfsc-scheduling-in-linux-bsd-works) | serverfault.com | 70% - 80% | 70% - 80% | - | Yes |
| [Best way to go about QOS router settings?](http://pbxinaflash.com/community/index.php?threads/best-way-to-go-about-qos-router-settings.8732/) | pbxinaflash.com | 80% - 90% | 80% - 90% | - | Yes |
| [Correct Tomato QOS Settings](http://forums.plugpbx.org/index.php?topic=251.0;wap2) | forums.plugpbx.org | below 85% | below 85% | max(49ms, 6.25 KB) | Yes |
| [QOS and VOIP - working?](http://forums.untangle.com/networking/6106-qos-voip-working.html) | forums.untangle.com | 95% | 80% | - | Yes |
| [Re: Help needed with HTB](http://www.spinics.net/lists/lartc/msg20832.html) | www.spinics.net | 80% | - | - |  |
| [RE: Prioritizing VOIP traffic without sacrificing throughput](http://www.spinics.net/lists/lartc/msg21740.html) | www.spinics.net | 80% | 80% | - |  |
| [HTB mpu and overhead settings for PPPoE ADSL?](http://www.spinics.net/lists/lartc/msg13594.html) | www.spinics.net | 80% | - | - |  |
| [SEPARATING VOIP AND SURFING](http://mailman.ds9a.nl/pipermail/lartc/2004q4/014257.html) | mailman.ds9a.nl | 80% | - | - |  |
| [A qos script that really works!](https://forum.openwrt.org/viewtopic.php?id=4112) | forum.openwrt.org | 93% | 89% | 1s |  |

### Wiki Pages ###

| **Title** | **Website** | **Shaping** | **Policing** | **Buffer Size** |
|:----------|:------------|:------------|:-------------|:----------------|
| [Quality of Service](http://www.dd-wrt.com/wiki/index.php/QoS) | www.dd-wrt.com | 80% - 95% | 80% - 95% | sfq (128p) |
| [Peer-to-Peer (P2P) Throttling](http://www.dd-wrt.com/wiki/index.php/Peer-to-Peer_%28P2P%29_Throttling) | www.dd-wrt.com | 80% - 95% | - | - |
| [Using Tomato's QOS System](http://tomatousb.org/tut:using-tomato-s-qos-system) | tomatousb.org | 85% | 66% - 80% | sfq (128p) |
| [Configure QoS for VoIP on your router for better voice quality](http://www.voipmechanic.com/setting-qos-voip-linksys.htm) | www.voipmachanic.com | 90% | - | - |
| [Implementing QoS](http://vonage.nmhoy.net/qos.html) | vonage.nmhoy.net | 90% - 95 | - | - |
| [How to: Manage Traffic Using Tomato and QoS](http://www.wi-fiplanet.com/tutorials/article.php/3800416) | www.wi-fiplanet.com | 90% | - | max(49ms, 6.25 KB) |
| [Tomato QoS settings on your VoIP router](http://www.voipmechanic.com/ddwrt-tomato.htm) | www.voipmechanic.com | 80% - 90% | - | - |
| [QoS Setup](http://help.close.io/customer/portal/articles/1286697-qos-settings) | help.close.io | 80% | 80% | - |
| [NetworkPro on Quality of Service](http://wiki.mikrotik.com/wiki/NetworkPro_on_Quality_of_Service) | wiki.mikrotik.com | above 85% | above 85% | - |
| [Setting up a maximum bandwidth (firewall/QoS) in a LANCOM router](https://www2.lancom.de/kb.nsf/1276/07F4E77B74CECC98C12578A2004077D2) | www2.lancom.de | 80% | 80% | - |
| [What is QoS and how do I use it?](http://help.fonality.com/index.php?title=TroubleshootingFAQ/Connect/Setup/What_is_QoS_and_how_do_I_use_it%3F) | help.fonality.com | below 80% | below 80% | - |
| [DrayTek 2920 load balancing router](http://www.increasebroadbandspeed.co.uk/draytek-2920) | www.increasebroadbandspeed.co.uk | 80% | 80% | - |
| [The Ultimate Traffic Conditioner: Low Latency, Fast Up & Downloads](http://lartc.org/howto/lartc.cookbook.ultimate-tc.html) | lartc.org | 90% | 80% | 10 KB |

### Blogs ###

| **Title** | **Website** | **Shaping** | **Policing** | **Buffer Size** |
|:----------|:------------|:------------|:-------------|:----------------|
| [The Beginner’s Guide to Using QoS (Quality of Service) on Your Router](http://www.howtogeek.com/75660/the-beginners-guide-to-qos-on-your-router/) | www.howtogeek.com | 85% - 95% | 85% - 95% | - |
| [Ensure a Fast Internet Connection When You Need It](http://lifehacker.com/326543/ensure-a-fast-internet-connection-when-you-need-it) | lifehacker.com | 85% | 85% | sfq (128p) |
| [The Basics: Configure Quality of Service (QoS) for VoIP](http://dailybeagle.com/2009/09/the-basics-configure-quality-of-service-qos-for-voip/) | dailybeagle.com | 90% - 95% | 80% - 90% | max(49ms, 6.25 KB) |
| [Quick and Easy QoS with Tomato](http://chillingsilence.wordpress.com/2009/11/21/quick-and-easy-qos-with-tomato/) | chillingsilence.wordpress.com | 94% | 90% | max(49ms, 6.25 KB) |
| [How to Prioritize Your Network Traffic with DD-WRT](http://www.howtogeek.com/70659/how-to-give-your-computers-network-traffic-priority-with-dd-wrt/) | www.howtogeek.com | 80% - 95% | 80% - 100% | sfq (128p) |
| [Hierarchical Fair Service Curve (HFSC)](https://calomel.org/pf_hfsc.html) | calomel.org | 97% | - | 500p |
| [Traffic Manager - QOS](http://rtn66u.blogspot.jp/2013/02/traffic-manager-qos.html) | rtn66u.blogspot.jp | 70% - 80% | 70% - 80% | - |

### Source Code ###

| **Title** | **Website** | **Shaping** | **Policing** | **Buffer Size** | **Notes** |
|:----------|:------------|:------------|:-------------|:----------------|:----------|
| [qos-nxt](https://github.com/zcecc22/qos-nxt) | github.com | 80% | 80% | 500p |  |
| [Rob Brewer's Firewall and QoS Scripts for OpenWRT](http://www.robbrewer.me/projects/openwrt/) | www.robbrewer.me | 90% | - | 150 KB |  |
| [A script for QOS](http://www.dslreports.com/forum/r24894358-Tomato-A-script-for-QOS) | www.dslreports.com | - | - | sfq (128p) |  |
| [Dedicated VOIP bandwidth - WRT54 Script Generator](http://www.dslreports.com/forum/r20708380-Dedicated-VOIP-bandwidth-WRT54-Script-Generator) | www.dslreports.com | - | - | sfq (128p) |  |
| [QoS for OpenWrt](http://www.rooot.net/en/geek-stuff/openwrt/1-qos-openwrt.html) | www.rooot.net | - | - | 200 KB |  |
| [An Interesting QOS script example](http://www.linksysinfo.org/index.php?threads/an-interesting-qos-script-example.36542/) | www.linksysinfo.org | 76% | 76% | default (1000p) |  |
| [QoS script for DD-WRT from Alex Rice's blog](http://pastebin.com/cNQ2DxEN) | pastbin.com | 85% - 90% | - | - |  |
| [15.10. Example of a full nat solution with QoS](http://lartc.org/howto/lartc.cookbook.fullnat.intro.html) | lartc.org | 80% | - | - |  |
| [QOS Script for OpenWRT](http://www.docunext.com/blog/2006/08/qos-script-for-openwrt.html) | www.docunext.com | 81% | 95% | 10 KB |  |
| [发一个双wan的智能QOS脚本（2010年6月17日更新附单wan）](http://bbs.dualwan.cn/viewthread.php?tid=32202) | bbs.dualwan.cn | - | 91% | sfq (128p) | (in Chinese) |
| [智能QOS脚本横空出世](http://bbs.dualwan.cn/viewthread.php?tid=20122) | bbs.dualwan.cn | - | - | default (1000p) | (in Chinese, imq0 / class 1:255) |
| [对zhoutao0712 qosv2.0 脚本的增强 第四次更新](http://bbs.dualwan.cn/viewthread.php?tid=110802) | bbs.dualwan.cn | - | - | sfq (128p) | (in Chinese) |
| [技术研究 QOS：周大师V4.0和cmt1978 等脚本的综合修改试用版0.1](http://bbs.dualwan.cn/viewthread.php?tid=148372) | bbs.dualwan.cn | - | - | sfq (128p) | (in Chinese) |
| [P2P惩罚 QOS脚本 V4.0](http://bbs.dualwan.cn/viewthread.php?tid=22482) | bbs.dualwan.cn | - | - | sfq (128p) | (in Chinese) |
| [双wan极速飞驰版QOS脚本，v0.5](http://bbs.dualwan.cn/viewthread.php?tid=120012) | bbs.dualwan.cn | - | - | sfq (128p) | (in Chinese) |
| [智能QOS脚本修改版](http://bbs.dualwan.cn/viewthread.php?tid=20752) | bbs.dualwan.cn | - | - | sfq (128p) | (in Chinese) |
| [小包优先+WEB优先+IP限速+连接数限制 的脚本](http://bbs.dualwan.cn/viewthread.php?tid=12192) | bbs.dualwan.cn | - | - | sfq (128p) | (in Chinese) |
| [儿童节发布K26支持任意网段限速QOS脚本，打得openwrt满地找牙](http://bbs.dualwan.cn/viewthread.php?tid=158102) | bbs.dualwan.cn | - | - | sfq (128p) | (in Chinese) |
| [发布K26 QOS脚本，打压openwrt势力](http://bbs.dualwan.cn/viewthread.php?tid=152972) | bbs.dualwan.cn | - | - | sfq (128p) | (in Chinese) |
| [ADSL QOS脚本 V1.1](http://bbs.dualwan.cn/viewthread.php?tid=188892) | bbs.dualwan.cn | - | - | sfq (128p) | (in Chinese) |
| [小包优先+web优先+游戏爆发+单IP限速+连接数限制 QOS脚本V2.0](http://bbs.dualwan.cn/viewthread.php?tid=14442) | bbs.dualwan.cn | 78% | 88% | sfq (128p) | (in Chinese) |

### Online Documents ###

| **Title** | **Website** | **Shaping** | **Policing** | **Buffer Size** | **Page** |
|:----------|:------------|:------------|:-------------|:----------------|:---------|
| [Setting up a DrayTek 2830n for SYNETY VoIP QoS](https://www.synety.com/support/kb/kb_file.php?id=00PG000000FBkQ7MAL) | www.synety.com | 80% | 80% | - | 2 |