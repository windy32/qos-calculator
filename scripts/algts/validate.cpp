// Copyright (C) 2012-2014 Fengyu Gao (feng32tc@gmail.com)
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 3 as
// published by the Free Software Foundation;
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

#include "../core/core.h"
#include <math.h>

/**
 * \file validate.cpp
 * \ingroup Scripts
 * \brief The "Hard limit CoDel" test script
 *
 * Usage:
 * \code
 * validate <local-address-range> <daemon-address> <daemon-port>
 * \endcode
 *
 * \see CoreApplication, ConsoleApplication, Scenario, Emulator, Terminal
 */

/**
 * \brief Execute test
 * \param app The Console Application object
 * \param bandwidth Bandwidth of the emulator
 * \param sessions Number of download sessions
 * \param nPass Specify how many times each test is executed
 * \param desc Description
 * \param printDetail Whether to print detail while executing
 * \param printSummary Whether to print summary after it's finished
 */
void execTest(ConsoleApplication &app, 
             int bandwidth, int sessions, int nPass, // Search options
             bool printDetail, bool printSummary) // Output options
{
    QVector<int> mins, maxs, avgs, rates;

    for (int index = 0; index < nPass; index++)
    {
        if (printDetail)
        {
            printf("\nPass %d / %d\n", index + 1, nPass);
            printf("------------------------------------------\n");
        }
        
        // Setup scenario
        Scenario s(12345, 10); // seed & length

        for (int u = 0; u < 3; u++)
        {
            s.addUser(QString("User %1").arg(u));
            
            if (u == 0)
            {
                s.addTask(QString("User %1").arg(u), new AsyncUdpEchoTask(23));
                s.task()->setAttribute("InputSize", "Uniform 100, 100");
                s.task()->setAttribute("EchoSize", "Uniform 100, 100");
                s.task()->setAttribute("Interval", "Uniform 20, 20");
            }

            for (int i = 0; i < sessions; i++)
            {
                s.addTask(QString("User %1").arg(u), new BulkDownloadTask(80));
                s.task()->setAttribute("MaxBytes", "INFINITE");
                s.task()->setAttribute("MaxRate", "INFINITE");
            }
        }
            
        // Execute
        app.exec(&s);

        // Collect statistics
        RawTraceItem rti;
        app.exportRawTrace("User 0", 0, rti);
        
        int nSamples = rti["Delay"].size();
        int nValid = 0;
        int min = 10000; // init to 10 sec
        int max = 0; // 0 ms
        int avg = 0; // 0 ms
        int count[21]; // 0 - 19ms, 20 - 39ms, ..., 380 - 399ms, 400+ ms
        int rate = 0; // Throughput init to 0 KB/s

        for (int i = 0; i < 21; i++) { count[i] = 0; }

        for (int i = 0; i < nSamples; i++)
        {
            if (rti["Delay"][i] == -1) // packet lost
                continue;

            min = qMin(min, rti["Delay"][i]);
            max = qMax(max, rti["Delay"][i]);
            avg += rti["Delay"][i];

            if (rti["Delay"][i] < 400)
            {
                count[rti["Delay"][i] / 20] += 1;
            }
            else
            {
                count[20] += 1;
            }
            nValid += 1;
        }
        avg /= nValid;
        
        for (int u = 0; u < 3; u++)
        {
            for (int i = 0; i < sessions; i++)
            {
                if (u == 0)
                {
                    app.exportRawTrace(QString("User %1").arg(u), i + 1, rti);
                }
                else
                {
                    app.exportRawTrace(QString("User %1").arg(u), i, rti);
                }
                rate += rti["TotalBytes"].last() / 1024;
            }
        }
        rate /= 10;
        
        mins.append(min);
        maxs.append(max);
        avgs.append(avg);
        rates.append(rate);

        // Output
        if (printDetail)
        {
            printf("%d Samples\n", nSamples);
            printf("Rate: %d KB/s, Min delay: %dms, Max delay: %dms, Avg delay: %dms\n", rate, min, max, avg);

            for(int i = 0; i < 20; i++)
            {
                printf("%4d", (i + 1) * 20);
            }
            printf(" 400+\n");
            
            for (int i = 0; i < 21; i++)
            {
                printf("%4d", count[i]);
            }
            printf("\n");
        }
    
        // Wait for a while
        Utils::msleep(1000);
    }
    
    if (printSummary)
    {
        printf("\nBandwidth = %d kbps, threads = %d", bandwidth, sessions);
        printf("\n=====================================================");
        printf("\nPass | Min Delay | Max Delay | Avg Delay | Throughput");
        printf("\n-----+-----------------------------------------------");
        for (int index = 0; index < nPass; index++)
        {
            printf("\n%4d | %9d | %9d | %9d | %10d", index + 1, mins[index], maxs[index], avgs[index], rates[index]);
        }
        printf("\n=====================================================\n\n");
    }
}

/**
 * \brief The entry point of the sample
 */
int main(int argc, char *argv[])
{
    // Start console application and check arguments
    ConsoleApplication app(argc, argv);
    
    // Enable logging
    Log::enable(Log::LOG_LEVEL_INFO);
    
    // Setup
    SshTerminal gateway("-p admin ssh root@172.16.0.1"); // use sshpass
    SshTerminal server("-p voyage ssh root@10.0.0.1");
    gateway.start();
    server.start();

    // In OpenWRT, sch modules are not loaded automatically on startup
    // However, the server is running Voyage based on x86 Debian
    gateway.enter("insmod sch_htb\n");
    gateway.enter("insmod sch_sfq\n");
    gateway.enter("insmod sch_codel\n");
    gateway.enter("insmod sch_fq_codel\n");
    gateway.enter("insmod cls_u32\n");

    // Exec tests
    int bandwidths[3] = { 2000, 8000, 30000 };

    int sessions[6] = { 1, 2, 3, 4, 6, 8 };
    // Flow number cannot be too large, otherwise there would be a warning:
    // TCP: Possible SYN flooding on port 80. Dropping request.

    int bytes_opt[3] = { 5232, 6798, 12540 };
    int token_rate_opt[3] = { 1496, 6555, 27478 };
    
    for (int algorithm = 0; algorithm < 5; algorithm++)
    {
    for (int i = 0; i < 3; i++) // bandwidth_index
    {
    for (int j = 0; j < 6; j++) // session_index
    {
        int bw = bandwidths[i];
        int flows = sessions[j];
        int rate = bandwidths[i] * 8 / 10;
        int rate_opt = token_rate_opt[i];
        int bytes = bytes_opt[i];
        
        // Setup traffic policing
        if (algorithm < 4) // default, sfq, codel and fq_codel
        {
            gateway.enter("tc qdisc add dev eth0 root handle 1: htb default 10\n");
            gateway.enter(QString("tc class add dev eth0 parent 1: classid 1:1 htb rate %1kbit ceil %2kbit quantum 1540\n").arg(rate).arg(rate));
            gateway.enter(QString("tc class add dev eth0 parent 1:1 classid 1:10 htb rate %1kbit ceil %2kbit quantum 1540\n").arg(rate / 4).arg(rate));
            gateway.enter(QString("tc class add dev eth0 parent 1:1 classid 1:20 htb rate %1kbit ceil %2kbit quantum 1540\n").arg(rate / 4).arg(rate));
            gateway.enter(QString("tc class add dev eth0 parent 1:1 classid 1:30 htb rate %1kbit ceil %2kbit quantum 1540\n").arg(rate / 4).arg(rate));
            gateway.enter(QString("tc class add dev eth0 parent 1:1 classid 1:40 htb rate %1kbit ceil %2kbit quantum 1540\n").arg(rate / 4).arg(rate));

            if (algorithm == 0) // htb (default)
            {
                // nothing to do here
            }
            else if (algorithm == 1) // htb + sfq
            {
                gateway.enter("tc qdisc add dev eth0 parent 1:10 handle 10: sfq perturb 10\n");
                gateway.enter("tc qdisc add dev eth0 parent 1:20 handle 20: sfq perturb 10\n");
                gateway.enter("tc qdisc add dev eth0 parent 1:30 handle 30: sfq perturb 10\n");
                gateway.enter("tc qdisc add dev eth0 parent 1:40 handle 40: sfq perturb 10\n");
            }
            else if (algorithm == 2) // htb + codel
            {
                gateway.enter("tc qdisc add dev eth0 parent 1:10 handle 10: codel\n");
                gateway.enter("tc qdisc add dev eth0 parent 1:20 handle 20: codel\n");
                gateway.enter("tc qdisc add dev eth0 parent 1:30 handle 30: codel\n");
                gateway.enter("tc qdisc add dev eth0 parent 1:40 handle 40: codel\n");
            }
            else if (algorithm == 3) // htb + fq_codel
            {
                gateway.enter("tc qdisc add dev eth0 parent 1:10 handle 10: fq_codel\n");
                gateway.enter("tc qdisc add dev eth0 parent 1:20 handle 20: fq_codel\n");
                gateway.enter("tc qdisc add dev eth0 parent 1:30 handle 30: fq_codel\n");
                gateway.enter("tc qdisc add dev eth0 parent 1:40 handle 40: fq_codel\n");
            }
            gateway.enter("tc filter add dev eth0 parent 1: protocol ip prio 1 u32 match ip dst 172.16.0.16 flowid 1:10\n");
            gateway.enter("tc filter add dev eth0 parent 1: protocol ip prio 1 u32 match ip dst 172.16.0.17 flowid 1:20\n");
            gateway.enter("tc filter add dev eth0 parent 1: protocol ip prio 1 u32 match ip dst 172.16.0.18 flowid 1:30\n");
            gateway.enter("tc filter add dev eth0 parent 1: protocol ip prio 1 u32 match ip dst 172.16.0.19 flowid 1:40\n");
        }
        else // bfifo (opt)
        {
            gateway.enter("tc qdisc add dev eth0 root handle 1: htb default 1\n");
            gateway.enter(QString("tc class add dev eth0 parent 1: classid 1:1 htb rate %1kbit ceil %2kbit quantum 1540\n").arg(rate_opt).arg(rate_opt));
            gateway.enter(QString("tc qdisc add dev eth0 parent 1:1 handle 10: bfifo limit %1\n").arg(bytes));
        }
        
        // Setup link
        server.enter(QString("tc qdisc add dev eth1 root handle 1: tbf burst 6250b latency 500ms rate %1kbit mtu 100000\n").arg(bw));

        // Execute Test
        execTest(app, bw, flows, 10, true, true);

        // Reset
        gateway.enter("tc qdisc del dev eth0 root\n");
        server.enter("tc qdisc del dev eth1 root\n");
    }
    }
    }
    gateway.close();
    server.close();

    // Exit
    return 0;
}

