#ifndef STOP_WAIT_COMMON_H
#define STOP_WAIT_COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <time.h>

#define PORT 9003
#define MAX_DATA 100

#define ACK 1
#define NO_ACK 0

typedef struct
{
    int seqNo;
    char data[MAX_DATA];
} Packet;

typedef struct
{
    int ackNo;
} AckPacket;

#endif
