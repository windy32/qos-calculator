#include <stdlib.h>
#include <string.h>
#include "parser.h"

// Parsing state
enum State { A, B, AT, BT, ATB, ATBT, Invalid };

// Parsing error
void parse_error(const char *token, const char *buf, int line)
{
    fprintf(stderr, "Parse error: Unexpected token %s\n", token);
    fprintf(stderr, "Line %d: %s\n", line, buf);
    exit(0);
}

void parse_error(const char *token, const char *after, const char *buf, int line)
{
    fprintf(stderr, "Parse error: Got token %s after %s\n", token, after);
    fprintf(stderr, "Line %d: %s\n", line, buf);
    exit(0);
}

void parse_error(const char *buf, int line)
{
    fprintf(stderr, "Parse error:\n");
    fprintf(stderr, "Line %d: %s\n", line, buf);
    exit(0);
}

// Parse sequential connection vector
SeqVector *parse_seq(const char *header, FILE *fp, int &line)
{
    SeqVector *vector = new SeqVector();
    SeqEpoch epoch;

    // Parse header
    int count;
    if (sscanf(header, "SEQ %lld %d %d %d", 
        &vector->start_time, &count, &vector->id, &vector->id_ns) != 4)
    {
        parse_error(header, line);
    }
    line += 1;

    // Parse body
    char buf[256];
    enum State state = Invalid;

    while (fgets(buf, sizeof(buf), fp) != NULL)
    {
        int bytes = 0;
        double microseconds = 0.0;

        if (buf[0] == '#' || buf[0] == '\n') // Skip comments or empty lines
        {
            line += 1;
        }
        else if (sscanf(buf, "w %d %d", 
            &vector->initiator_window_size, &vector->acceptor_window_size) == 2)
        {
            line += 1;
        }
        else if (sscanf(buf, "r %lf", &microseconds) == 1)
        {
            vector->min_rtt = (int)microseconds;
            line += 1;
        }
        else if (sscanf(buf, "l %f %f",
            &vector->loss_rate_forward, &vector->loss_rate_backward) == 2)
        {
            line += 1;
        }
        else if (sscanf(buf, "> %d", &bytes) == 1)
        {
            if (state == A || state == B || state == ATB)
            {
                parse_error(">", "< or >", buf, line);
            }
            else if (state == AT || state == BT || state == ATBT || state == Invalid)
            {
                // Save previous epoch
                if (state != Invalid)
                {
                    vector->epochs.push_back(epoch);
                }

                // Initialize new epoch
                epoch = SeqEpoch();
                epoch.request_size = bytes;
                epoch.interval = 0;
                
                state = A;
            }
            line += 1;
        }
        else if (sscanf(buf, "< %d", &bytes) == 1)
        {
            if (state == A || state == B || state == ATB)
            {
                parse_error("<", "< or >", buf, line);
            }
            else if (state == AT)
            {
                // now it must be ATBT
                epoch.response_size = bytes;
                epoch.response_delay = epoch.interval;
                epoch.interval = 0;
                state = ATB;
            }
            else if (state == BT || state == ATBT || state == Invalid)
            {
                // Save previous epoch
                if (state != Invalid)
                {
                    vector->epochs.push_back(epoch);
                }
                
                // Initialize new epoch
                epoch = SeqEpoch();
                epoch.request_size = 0;
                epoch.response_delay = 0;
                epoch.response_size = bytes;
                epoch.interval = 0;

                state = B;
            }
            line += 1;
        }
        else if (sscanf(buf, "t %lf", &microseconds) == 1)
        {
            if (state == A)
            {
                // We don't know whether it would be AT or ATBT, suppose it's AT
                epoch.interval = (int)microseconds;
                state = AT;
            }
            else if (state == B)
            {
                epoch.interval = (int)microseconds;
                state = BT;
            }
            else if (state == ATB)
            {
                epoch.interval = (int)microseconds;
                state = ATBT;
            }
            else if (state == AT || state == BT || state == ATBT)
            {
                parse_error("t", "t", buf, line);
            }
            else if (state == Invalid)
            {
                parse_error("t", buf, line);
            }
            line += 1;
        }
        else if (strncmp(buf, "SEQ ", 4) == 0 ||
                 strncmp(buf, "CONC ", 5) == 0) // new vector detected
        {
            // Push this line back to file stream
            for (int i = strlen(buf) - 1; i >= 0; i--)
            {
                ungetc(buf[i], fp);
            }
            break;
        }
        else
        {
            parse_error(buf, line);
        }
    }

    if (state == A || state == B || state == ATB ||
        state == AT || state == BT || state == ATBT)
    {
        vector->epochs.push_back(epoch);
    }

    return vector;
}

// Parse concurrent connection vector
ConcVector *parse_conc(const char *header, FILE *fp, int &line)
{
    ConcVector *vector = new ConcVector();
    ConcEpoch forwardEpoch;
    ConcEpoch backwardEpoch;

    // Parse header
    int count_init; // number of ADUs the initiator sends
    int count_acc; // number of ADUs the acceptor sends
    if (sscanf(header, "CONC %lld %d %d %d %d",
        &vector->start_time, &count_init, &count_acc, &vector->id, &vector->id_ns) != 5)
    {
        parse_error(header, line);
    }
    line += 1;

    // Parse body
    char buf[256];
    enum State forwardState = Invalid;
    enum State backwardState = Invalid;

    while (fgets(buf, sizeof(buf), fp) != NULL)
    {
        int bytes = 0;
        double microseconds = 0.0;

        if (buf[0] == '#' || buf[0] == '\n') // Skip comments or empty lines
        {
            line += 1;
        }
        else if (sscanf(buf, "w %d %d", 
            &vector->initiator_window_size, &vector->acceptor_window_size) == 2)
        {
            line += 1;
        }
        else if (sscanf(buf, "r %lf", &microseconds) == 1)
        {
            vector->min_rtt = (int)microseconds;
            line += 1;
        }
        else if (sscanf(buf, "l %f %f",
            &vector->loss_rate_forward, &vector->loss_rate_backward) == 2)
        {
            line += 1;
        }
        else if (sscanf(buf, "c> %d", &bytes) == 1)
        {
            if (forwardState == A)
            {
                parse_error("c>", "c>", buf, line);
            }
            else if (forwardState == Invalid || forwardState == AT)
            {
                // Save previous epoch
                if (forwardState != Invalid)
                {
                    vector->forwardEpochs.push_back(forwardEpoch);
                }

                // Initialize new epoch
                forwardEpoch = ConcEpoch();
                forwardEpoch.block_size = bytes;
                forwardEpoch.interval = 0;
                
                forwardState = A;
            }
            line += 1;
        }
        else if (sscanf(buf, "t> %lf", &microseconds) == 1)
        {
            if (forwardState == AT || forwardState == Invalid)
            {
                parse_error("t>", buf, line);
            }
            else if (forwardState == A)
            {
                forwardEpoch.interval = (int)microseconds;
                forwardState = AT;
            }
            line += 1;
        }
        else if (sscanf(buf, "c< %d", &bytes) == 1)
        {
            if (backwardState == B)
            {
                parse_error("c<", "c<", buf, line);
            }
            else
            {
                // Save previous epoch
                if (backwardState != Invalid)
                {
                    vector->backwardEpochs.push_back(backwardEpoch);
                }
                
                // Initialize new epoch
                backwardEpoch = ConcEpoch();
                backwardEpoch.block_size = bytes;
                backwardEpoch.interval = 0;
                
                backwardState = B;
            }
            line += 1;
        }
        else if (sscanf(buf, "t< %lf", &microseconds) == 1)
        {
            if (backwardState == BT || backwardState == Invalid)
            {
                parse_error("t<", buf, line);
            }
            else
            {
                backwardEpoch.interval = (int)microseconds;
                backwardState = BT;
            }
            line += 1;
        }
        else if (strncmp(buf, "SEQ ", 4) == 0 ||
                 strncmp(buf, "CONC ", 5) == 0) // new vector detected
        {
            // Push this line back to file stream
            for (int i = strlen(buf) - 1; i >= 0; i--)
            {
                ungetc(buf[i], fp);
            }
            break;
        }
        else
        {
            parse_error(buf, line);
            exit(0);
        }
    }

    if (forwardState == A || forwardState == AT)
    {
        vector->forwardEpochs.push_back(forwardEpoch);
    }
    
    if (backwardState == B || backwardState == BT)
    {
        vector->backwardEpochs.push_back(backwardEpoch);
    }

    return vector;
}

