#ifndef SLIDING_COMMON_H
#define SLIDING_COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/time.h>

#define PORT 9004

#define MAX_DATA 100

/* Fixed values for demonstration */
#define SEQUENCE_BITS 3
#define MAX_SEQUENCE_NUMBERS (1 << SEQUENCE_BITS)

#define TOTAL_PACKETS 8
#define WINDOW_SIZE 4

#define DATA_PACKET 1
#define END_PACKET  2

typedef struct
{
    int type;
    int seqNo;
    char data[MAX_DATA];
} Packet;

typedef struct
{
    int ackNo;
} AckPacket;

#endif
