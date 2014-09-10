#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include "parser.h"

// input parameters
double param_time = 0;
double param_throughput = 0;
double param_scale = 0;

// connection vectors
std::vector<ConnVector *> vectors;

void print_usage()
{
    fprintf(stderr, "\n");
    fprintf(stderr, "Connection Vector Tool v0.1\n");
    fprintf(stderr, "---------------------------\n");
    fprintf(stderr, "A simple tool to analyze and scale down cvec files\n");
    fprintf(stderr, "\n");
    fprintf(stderr, "Usage:\n");
    fprintf(stderr, "    cvectool <input_file> [-t length] [-b bandwidth] [-s scale]\n");
    fprintf(stderr, "Arguments:\n");
    fprintf(stderr, "    -t length (seconds): Use only the head of the cvec file\n");
    fprintf(stderr, "                   Note: You should be careful when using this option, as the\n");
    fprintf(stderr, "                         traffic pattern in the first 5 seconds can be quite\n");
    fprintf(stderr, "                         different from the overall pattern\n");
    fprintf(stderr, "    -b bandwidth (mbps): Reduce average throughput to specified value\n");
    fprintf(stderr, "                   Note: The throughput is reduced by dropping connection\n");
    fprintf(stderr, "                         vectors, so when the throughput becomes lower, the\n");
    fprintf(stderr, "                         connection rate will also be lower\n");
    fprintf(stderr, "    -s scale  (0.3 ~ 2): Scale up / down the sizes of application data units\n");
    fprintf(stderr, "                   Note: This option does not influence connection rate\n");
    fprintf(stderr, "Example:\n");
    fprintf(stderr, "    cvectool sample.cvec\n");
    fprintf(stderr, "       - Analyze the specified file\n");
    fprintf(stderr, "    cvectool sample.cvec -t 300 > new.cvec\n");
    fprintf(stderr, "       - Reduce the length of the file to 5 minutes\n");
    fprintf(stderr, "    cvectool sample.cvec -s 1.1 > new.cvec\n");
    fprintf(stderr, "       - Increase the sizes of application data units by 10%%\n");
    fprintf(stderr, "    cvectool sample.cvec -b 8 > new.cvec\n");
    fprintf(stderr, "       - Reduce the average throughput to 8 Mbps\n");
    fprintf(stderr, "\n");
}

void process(const char *filename)
{
    // Statistics
    const int header_size = 40; // TCP header + IPv4 header

    int nSeq = 0;
    int nConc = 0;
    long long min_start_time = LONG_LONG_MAX;
    long long max_start_time = 0;
    
    long long tx_bytes = 0; // initiator -> acceptor
    long long rx_bytes = 0; // acceptor -> initiator
    long long tx_app_bytes = 0;
    long long rx_app_bytes = 0;
    int tx_units = 0;
    int rx_units = 0;
    int tx_packets = 0;
    int rx_packets = 0;
    
    for (int i = 0; i < vectors.size(); i++)
    {
        if (vectors[i]->start_time < min_start_time)
            min_start_time = vectors[i]->start_time;
        if (vectors[i]->start_time > max_start_time)
            max_start_time = vectors[i]->start_time;

        tx_app_bytes += vectors[i]->tx_app_bytes;
        rx_app_bytes += vectors[i]->rx_app_bytes;
        tx_bytes += vectors[i]->tx_bytes;
        rx_bytes += vectors[i]->rx_bytes;
        tx_units += vectors[i]->tx_units;
        rx_units += vectors[i]->rx_units;
        tx_packets += vectors[i]->tx_packets;
        rx_packets += vectors[i]->rx_packets;

        if (vectors[i]->type == ConnVector::Seq)
        {
            nSeq += 1;
        }
        else if (vectors[i]->type == ConnVector::Conc)
        {
            nConc += 1;
        }
        else
        {
            fprintf(stderr, "Error: unexpected vector type!\n");
        }
    }

    fprintf(stderr, "---------------------------------------------------------\n");
    fprintf(stderr, "Connection Vector Tool (Statistics)\n");
    fprintf(stderr, "---------------------------------------------------------\n");
    fprintf(stderr, "Filename: %s\n", filename);

    fprintf(stderr, "Total vectors: %d\n", nSeq + nConc);
    fprintf(stderr, "   - Sequential vectors: %d\n", nSeq);
    fprintf(stderr, "   - Concurrent vectors: %d\n", nConc);
    fprintf(stderr, "Min start time: %.3lf seconds\n", min_start_time / 1000000.0);
    fprintf(stderr, "Max start time: %.3lf seconds\n", max_start_time / 1000000.0);

    fprintf(stderr, "\n");
    fprintf(stderr, "Average connection rate: %.2lf connections / second\n", 
        (nSeq + nConc) / ((max_start_time - min_start_time) / 1000000.0));
    fprintf(stderr, "Average tx throughput: %.2lf Mbps\n", 
        tx_bytes * 8.0 / (max_start_time - min_start_time));
    fprintf(stderr, "Average rx throughput: %.2lf Mbps\n", 
        rx_bytes * 8.0 / (max_start_time - min_start_time));

    // network layer bytes
    fprintf(stderr, "\n");
    fprintf(stderr, "Total bytes: %lld bytes (%.1lf MB)\n", 
        tx_bytes + rx_bytes, (tx_bytes + rx_bytes) / 1000000.0);
    fprintf(stderr, "   - Tx (initiator -> acceptor): %lld (%.1lf MB)\n", 
        tx_bytes, tx_bytes / 1000000.0);
    fprintf(stderr, "   - Rx (acceptor -> initiator): %lld (%.1lf MB)\n", 
        rx_bytes, rx_bytes / 1000000.0);

    // application layer bytes
    fprintf(stderr, "\n");
    fprintf(stderr, "Total application layer bytes: %lld bytes (%.1lf MB)\n", 
        tx_app_bytes + rx_app_bytes, (tx_app_bytes + rx_app_bytes) / 1000000.0);
    fprintf(stderr, "   - Tx (initiator -> acceptor): %lld (%.1lf MB)\n", 
        tx_app_bytes, tx_app_bytes / 1000000.0);
    fprintf(stderr, "   - Rx (acceptor -> initiator): %lld (%.1lf MB)\n", 
        rx_app_bytes, rx_app_bytes / 1000000.0);

    // application data units
    fprintf(stderr, "\n");
    fprintf(stderr, "Total application data units: %d\n", tx_units + rx_units);
    fprintf(stderr, "   - Tx (initiator -> acceptor): %d\n", tx_units);
    fprintf(stderr, "   - Rx (acceptor -> initiator): %d\n", rx_units);

    // packets
    fprintf(stderr, "\n");
    fprintf(stderr, "Total packets: %d\n", tx_packets + rx_packets);
    fprintf(stderr, "   - Tx (initiator -> acceptor): %d\n", tx_packets);
    fprintf(stderr, "   - Rx (acceptor -> initiator): %d\n", rx_packets);

    // average packet size
    fprintf(stderr, "\n");
    fprintf(stderr, "Average packet size: %lld\n", (tx_bytes + rx_bytes) / (tx_packets + rx_packets));
    fprintf(stderr, "   - Tx (initiator -> acceptor): %lld\n", tx_bytes / tx_packets);
    fprintf(stderr, "   - Rx (acceptor -> initiator): %lld\n", rx_bytes / rx_packets);

    fprintf(stderr, "---------------------------------------------------------\n");
    
    // Process this file
    if (param_time != 0 || param_throughput != 0 || param_scale != 0)
    {
        // The final list
        std::vector<ConnVector *> list;

        // Discard the vectors out of range
        if (param_time != 0)
        {
            int drop_count = 0;
            fprintf(stderr, "Reduce length to %.1lf seconds...\n", param_time);

            for (int i = 0; i < vectors.size(); i++)
            {
                if (vectors[i]->start_time / 1000000.0 < param_time)
                {
                    list.push_back(vectors[i]);
                }
                else
                {
                    drop_count += 1;
                }
            }
            fprintf(stderr, "   - %d connection vectors dropped\n", drop_count);
        }
        else
        {
            list = vectors;        
        }

        if (param_time == 0)
        {
            param_time = (max_start_time - min_start_time) / 1000000.0;
        }
        
        // Reduce average throughput
        if (param_throughput != 0)
        {
            int drop_count = 0;
            std::vector<bool> drop(list.size(), false);

            fprintf(stderr, "Reduce average throughput to %.1lf Mbps...\n", param_throughput);
        
            // Build a random index list
            std::vector<int> random_list;
            for (int i = 0; i < list.size(); i++)
            {
                random_list.push_back(i);
            }
            
            for (int i = 0; i < random_list.size(); i++) // Randomize
            {
                int target = (rand() * rand() + rand()) % random_list.size();
                int tmp = random_list[i];
                random_list[i] = random_list[target];
                random_list[target] = tmp;
            }
            
            // Calculate current average throughput
            long long rx_bytes = 0; // acceptor -> initiator
            for (int i = 0; i < list.size(); i++)
            {
                rx_bytes += list[i]->rx_bytes;
            }
            double throughput =  rx_bytes * 8 / 1000000.0 / param_time; // mbps
            
            // Reduce throughput
            int index = 0;
            while (throughput >= param_throughput)
            {
                int target = random_list[index++];
                drop[target] = true;
                drop_count += 1;

                // Update average throughput
                rx_bytes -= list[target]->rx_bytes;
                throughput =  rx_bytes * 8 / 1000000.0 / param_time;

                if (index == list.size())
                {
                    fprintf(stderr, "Error: all vectors has been dropped!\n");
                    exit(0);
                }
            }

            // Copy to list
            std::vector<ConnVector *> tmp;
            for (int i = 0; i < list.size(); i++)
            {
                if (!drop[i]) 
                    tmp.push_back(list[i]);
            }
            list = tmp;
            
            fprintf(stderr, "   - %d connection vectors dropped\n", drop_count);
        }
        
        // Scale
        if (param_scale != 0)
        {
            fprintf(stderr, "Scale application data units (%.2lfx)...\n", param_scale);
        
            for (int i = 0; i < list.size(); i++)
            {
                list[i]->scale(param_scale);
            }
        }
        
        fprintf(stderr, "---------------------------------------------------------\n");

        // Dump results
        for (int i = 0; i < list.size(); i++)
        {
            list[i]->dump();
        }
    }
}

int main(int argc, char *argv[])
{
    // Parse arguments
    if (argc != 2 && argc != 4 && argc != 6 && argc != 8)
    {
        print_usage();
        return 0;
    }
    
    for (int index = 2; index < argc; index += 2)
    {
        if (strcmp(argv[index], "-t") == 0)
        {
            if (sscanf(argv[index + 1], "%lf", &param_time) != 1)
            {
                print_usage();
                return 0;
            }
        }
        else if (strcmp(argv[index], "-b") == 0)
        {
            if (sscanf(argv[index + 1], "%lf", &param_throughput) != 1)
            {
                print_usage();
                return 0;
            }
        }
        else if (strcmp(argv[index], "-s") == 0)
        {
            if (sscanf(argv[index + 1], "%lf", &param_scale) != 1)
            {
                print_usage();
                return 0;
            }
            else if (param_scale < 0.2999 || param_scale > 2.0001)
            {
                print_usage();
                return 0;
            }
        }
        else
        {
            print_usage();
            return 0;
        }
    }
    
    // Open file
    char *filename = argv[1];
    FILE *fp = fopen(filename, "r");
    if (fp == NULL)
    {
        printf("Cannot open file!\n");
        return 0;
    }

    // Read file
    char buf[256];
    int line = 1;
    
    while (fgets(buf, sizeof(buf), fp) != NULL)
    {
        // Skip comment or empty line
        if (buf[0] == '#' || buf[0] == '\n')
        {
            line += 1;
            continue;
        }

        // New vector detected
        if (strncmp(buf, "SEQ ", 4) == 0)
        {
            SeqVector *vec = parse_seq(buf, fp, line);
            vec->update_statistics();
            vectors.push_back(vec);
        }
        else if (strncmp(buf, "CONC ", 5) == 0)
        {
            ConcVector *vec = parse_conc(buf, fp, line);
            vec->update_statistics();
            vectors.push_back(vec);
        }
        else
        {
            fprintf(stderr, "Unrecognized vector type at line %d\n", line);
            return 0;
        }
    }
    
    // Process this file
    process(filename);
    
    return 0;
}

