#ifndef EPOCH_H
#define EPOCH_H

// Sequencial epoch
struct SeqEpoch
{
    int request_size;   // in bytes
    int response_size;  // in bytes
    int response_delay; // in microseconds
    int interval;       // in microseconds
                        // thus the max interval is about 2147 seconds
    SeqEpoch()
    {
        request_size = 0;
        response_size = 0;
        response_delay = 0;
        interval = 0;
    }
};

// Concurrent epoch
struct ConcEpoch
{
    int block_size;    // in bytes
    int interval;      // in microseconds

    ConcEpoch()
    {
        block_size = 0;
        interval = 0;
    }
};

#endif

