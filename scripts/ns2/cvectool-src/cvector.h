#ifndef CVECTOR_H
#define CVECTOR_H

#include <vector>
#include "epoch.h"

struct ConnVector
{
    long long start_time;      // in microseconds
    int id;
    int id_ns;
    int initiator_window_size; // in bytes
    int acceptor_window_size;  // in bytes
    int min_rtt;               // in microseconds
    float loss_rate_forward;   // 0.0 to 1.0 (init -> acc)
    float loss_rate_backward;  // 0.0 to 1.0 (acc -> init)
    
    int tx_bytes;
    int rx_bytes;
    int tx_app_bytes;
    int rx_app_bytes;
    int tx_units; // number of application units
    int rx_units; // number of application units
    int tx_packets; // number of tx packets
    int rx_packets; // number of rx packets
    
    enum VectorType { Seq, Conc } type;

    ConnVector()
    {
        start_time = 0;
        id = 0;
        id_ns = 0;
        initiator_window_size = 0;
        acceptor_window_size = 0;
        min_rtt = 0;
        loss_rate_forward = 0;
        loss_rate_backward = 0;
        
        tx_bytes = 0;
        rx_bytes = 0;
        tx_app_bytes = 0;
        rx_app_bytes = 0;
        tx_units = 0;
        rx_units = 0;
    }
    
    virtual void dump() = 0;
    virtual void scale(double factor) = 0;
    virtual void update_statistics() = 0;
};

// Definition of a sequential connection vector

// SEQ 6851 1 21217 555382       # starts at 6.851 ms and contains 1 exchange (epoch)
// w 64800 6432                  # win sz (bytes): init acc
// r 1176194                     # min RTT (microseconds)
// l 0.000000 0.000000           # loss: init->acc acc->init
// > 245                         # init sends 245 bytes
// t 51257                       # acc waits 51 ms after recv
// < 510                         # acc sends 510 bytes
// t 6304943                     # init waits 6.3 sec after send and then sends FIN

struct SeqVector : public ConnVector
{
    std::vector<SeqEpoch> epochs;
    
    SeqVector()
    {
        type = Seq;
    }
    
    virtual void dump()
    {
        printf("SEQ %lld %d %d %d\n", start_time, epochs.size(), id, id_ns);
        printf("w %d %d\n", initiator_window_size, acceptor_window_size);
        printf("r %d\n", min_rtt);
        printf("l %.6f %.6f\n", loss_rate_forward, loss_rate_backward);

        for (int i = 0; i < epochs.size(); i++)
        {
            int request_size = epochs[i].request_size;
            int response_size = epochs[i].response_size;
            int response_delay = epochs[i].response_delay;
            int interval = epochs[i].interval;
            
            // Type 1: AT
            if (response_size == 0)
            {
                printf("> %d\n", request_size);

                if (i != epochs.size() - 1 || interval != 0)
                {
                    printf("t %d\n", interval);
                }
            }
            else if (request_size == 0) // Type 2: BT
            {
                printf("< %d\n", response_size);

                if (i != epochs.size() - 1 || interval != 0)
                {
                    printf("t %d\n", interval);
                }
            }
            else if (request_size != 0 && response_size != 0)
            {
                printf("> %d\n", request_size);
                printf("t %d\n", response_delay);
                printf("< %d\n", response_size);

                if (i != epochs.size() - 1 || interval != 0)
                {
                    printf("t %d\n", interval);
                }
            }
        }
    }

    virtual void scale(double factor)
    {
        for (int i = 0; i < epochs.size(); i++)
        {
            epochs[i].request_size *= factor;
            epochs[i].response_size *= factor;
        }
        update_statistics();
    }

    virtual void update_statistics()
    {
        const int header_size = 20 + 20 + 14; // TCP header + IPv4 header + MAC header
        const int mss = 1460;

        tx_bytes = 0;
        rx_bytes = 0;
        tx_app_bytes = 0;
        rx_app_bytes = 0;
        tx_units = 0;
        rx_units = 0;
        tx_packets = 0;
        rx_packets = 0;

        for (int i = 0; i < epochs.size(); i++)
        {
            // application data units
            tx_units += (epochs[i].request_size > 0) ? 1 : 0;
            rx_units += (epochs[i].response_size > 0) ? 1 : 0;
            
            // application layer bytes
            tx_app_bytes += epochs[i].request_size;
            rx_app_bytes += epochs[i].response_size;
            
            // number of packets
            int cur_tx_packets = (epochs[i].request_size + mss - 1) / mss;
            int cur_rx_packets = (epochs[i].response_size + mss - 1) / mss;
            
            tx_packets += cur_tx_packets;
            rx_packets += cur_rx_packets;
            
            // network layer bytes
            // Note: SYN, FIN, and RST packets are not counted temporarily
            tx_bytes += epochs[i].request_size + cur_tx_packets * header_size;
            rx_bytes += epochs[i].response_size + cur_rx_packets * header_size;
        }
    }
}; 

// Definition of a concurrent connection vector

// CONC 1429381 2 2 26876 793318 # starts at 1.4 s, init sends 2 ADUs and acc sends 2 ADUs
// w 65535 5840                  # win sz (bytes)
// r 36556                       # min RTT (microseconds)
// l 0.000000 0.000000           # loss rate
// c> 222                        # init sends 222 bytes
// t> 62436302                   # init waits 62 sec
// c< 726                        # acc sends 726 bytes
// t< 62400173                   # acc waits 62 sec
// c> 16                         # init sends 16 bytes
// t> 725                        # init waits 725 us and then sends FIN
// c< 84                         # acc sends 84 bytes
// t< 130                        # acc waits 130 us and then sends FIN

struct ConcVector : public ConnVector
{
    std::vector<ConcEpoch> forwardEpochs;  // sent by init
    std::vector<ConcEpoch> backwardEpochs; // sent by acc

    ConcVector()
    {
        type = Conc;
    }

    virtual void dump()
    {
        printf("CONC %lld %d %d %d %d\n", 
            start_time, forwardEpochs.size(), backwardEpochs.size(), id, id_ns);
        printf("w %d %d\n", initiator_window_size, acceptor_window_size);
        printf("r %d\n", min_rtt);
        printf("l %.6f %.6f\n", loss_rate_forward, loss_rate_backward);

        for (int i = 0; i < forwardEpochs.size(); i++)
        {
            int block_size = forwardEpochs[i].block_size;
            int interval = forwardEpochs[i].interval;
            
            printf("c> %d\n", block_size);
            printf("t> %d\n", interval);
        }

        for (int i = 0; i < backwardEpochs.size(); i++)
        {
            int block_size = backwardEpochs[i].block_size;
            int interval = backwardEpochs[i].interval;
            
            printf("c< %d\n", block_size);
            printf("t< %d\n", interval);
        }
    }

    virtual void scale(double factor)
    {
        for (int i = 0; i < forwardEpochs.size(); i++)
        {
            forwardEpochs[i].block_size *= factor;
        }
        for (int i = 0; i < backwardEpochs.size(); i++)
        {
            backwardEpochs[i].block_size *= factor;
        }
        update_statistics();
    }

    virtual void update_statistics()
    {
        const int header_size = 20 + 20 + 14; // TCP header + IPv4 header + MAC header
        const int mss = 1460;

        tx_bytes = 0;
        rx_bytes = 0;
        tx_app_bytes = 0;
        rx_app_bytes = 0;
        tx_units = 0;
        rx_units = 0;
        tx_packets = 0;
        rx_packets = 0;

        for (int i = 0; i < forwardEpochs.size(); i++)
        {
            tx_units += 1;
            tx_app_bytes += forwardEpochs[i].block_size;
            int cur_tx_packets = (forwardEpochs[i].block_size + mss - 1) / mss;
            tx_packets += cur_tx_packets;
            tx_bytes += forwardEpochs[i].block_size + cur_tx_packets * header_size;
        }

        for (int i = 0; i < backwardEpochs.size(); i++)
        {
            rx_units = 1;
            rx_app_bytes += backwardEpochs[i].block_size;
            int cur_rx_packets = (backwardEpochs[i].block_size + mss - 1) / mss;
            rx_packets += cur_rx_packets;
            rx_bytes += backwardEpochs[i].block_size + cur_rx_packets * header_size;
        }
    
    }
};

#endif

