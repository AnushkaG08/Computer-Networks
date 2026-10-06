#include "sliding_common.h"

int main()
{
    int sock;

    struct sockaddr_in receiver;

    Packet packet;
    AckPacket ack;

    int propagationTime;
    int transmissionTime;

    int packets[TOTAL_PACKETS];

    struct timeval startTime;
    struct timeval endTime;

    socklen_t receiverLength = sizeof(receiver);

    // ========================================
    // CREATE UDP SOCKET
    // ========================================

    sock = socket(AF_INET, SOCK_DGRAM, 0);

    if(sock < 0)
    {
        perror("Socket creation failed");
        return 1;
    }

    // ========================================
    // RECEIVER ADDRESS
    // ========================================

    memset(&receiver, 0, sizeof(receiver));

    receiver.sin_family = AF_INET;
    receiver.sin_port = htons(PORT);

    inet_pton(
        AF_INET,
        "127.0.0.1",
        &receiver.sin_addr
    );

    // ========================================
    // ONLY TWO INPUTS
    // ========================================

    printf("Enter propagation time (ms): ");
    scanf("%d", &propagationTime);

    printf("Enter transmission time (ms): ");
    scanf("%d", &transmissionTime);

    if(propagationTime < 0 || transmissionTime < 0)
    {
        printf("Invalid time.\n");

        close(sock);
        return 1;
    }

    // ========================================
    // CREATE PACKET ARRAY
    // ========================================

    for(int i = 0; i < TOTAL_PACKETS; i++)
    {
        packets[i] = i;
    }

    // ========================================
    // DISPLAY INFORMATION
    // ========================================

    printf("\n");
    printf("====================================================\n");
    printf("           SLIDING WINDOW PROTOCOL\n");
    printf("====================================================\n");

    printf("\nINPUT VALUES\n");
    printf("----------------------------------------------------\n");

    printf(
        "Propagation Time       : %d ms\n",
        propagationTime
    );

    printf(
        "Transmission Time      : %d ms\n",
        transmissionTime
    );

    printf("\nPROTOCOL PARAMETERS\n");
    printf("----------------------------------------------------\n");

    printf(
        "Sequence Number Bits   : %d bits\n",
        SEQUENCE_BITS
    );

    printf(
        "Maximum Sequence No.   : %d\n",
        MAX_SEQUENCE_NUMBERS
    );

    printf(
        "Sequence Number Range  : 0 - %d\n",
        MAX_SEQUENCE_NUMBERS - 1
    );

    printf(
        "Total Packets          : %d\n",
        TOTAL_PACKETS
    );

    printf(
        "Window Size            : %d\n",
        WINDOW_SIZE
    );

    // ========================================
    // DISPLAY ARRAY
    // ========================================

    printf("\n");
    printf("PACKET ARRAY\n");
    printf("----------------------------------------------------\n");

    for(int i = 0; i < TOTAL_PACKETS; i++)
    {
        printf("[%d] ", packets[i]);
    }

    printf("\n");

    // ========================================
    // START TOTAL TIME
    // ========================================

    gettimeofday(&startTime, NULL);

    // ========================================
    // SLIDING WINDOW
    // ========================================

    int base = 0;

    while(base < TOTAL_PACKETS)
    {
        int end = base + WINDOW_SIZE;

        if(end > TOTAL_PACKETS)
        {
            end = TOTAL_PACKETS;
        }

        printf("\n");
        printf("====================================================\n");
        printf("CURRENT WINDOW\n");
        printf("====================================================\n");

        // ------------------------------------
        // Show packet array
        // ------------------------------------

        printf("Array : ");

        for(int i = 0; i < TOTAL_PACKETS; i++)
        {
            printf("[%d] ", packets[i]);
        }

        printf("\n");

        // ------------------------------------
        // Show window
        // ------------------------------------

        printf("Window: ");

        for(int i = 0; i < TOTAL_PACKETS; i++)
        {
            if(i >= base && i < end)
            {
                printf("[%d] ", packets[i]);
            }
            else
            {
                printf("    ");
            }
        }

        printf("\n");

        printf(
            "Window contains packets: %d to %d\n",
            packets[base],
            packets[end - 1]
        );

        // ====================================
        // SEND PACKETS
        // ====================================

        printf("\nSending packets...\n");

        for(int i = base; i < end; i++)
        {
            packet.type = DATA_PACKET;

            packet.seqNo = packets[i];

            sprintf(
                packet.data,
                "Data of Packet %d",
                packets[i]
            );

            sendto(
                sock,
                &packet,
                sizeof(packet),
                0,
                (struct sockaddr *)&receiver,
                receiverLength
            );

            printf(
                "Sender -> Packet %d sent\n",
                packet.seqNo
            );

            // Simulate transmission time
            usleep(transmissionTime * 1000);
        }

        // ====================================
        // RECEIVE ACKS
        // ====================================

        printf("\nReceiving ACKs...\n");

        for(int i = base; i < end; i++)
        {
            recvfrom(
                sock,
                &ack,
                sizeof(ack),
                0,
                NULL,
                NULL
            );

            printf(
                "Sender <- ACK %d received\n",
                ack.ackNo
            );

            // Simulate propagation delay
            usleep(propagationTime * 1000);
        }

        // ====================================
        // WINDOW SLIDES
        // ====================================

        base = end;

        printf("\n");

        if(base < TOTAL_PACKETS)
        {
            printf("Window slides...\n");

            printf(
                "New Window starts from Packet %d\n",
                packets[base]
            );
        }
        else
        {
            printf("All packets are transmitted.\n");
        }
    }

    // ========================================
    // END PACKET
    // ========================================

    packet.type = END_PACKET;
    packet.seqNo = -1;

    strcpy(packet.data, "END");

    sendto(
        sock,
        &packet,
        sizeof(packet),
        0,
        (struct sockaddr *)&receiver,
        receiverLength
    );

    // ========================================
    // END TIME
    // ========================================

    gettimeofday(&endTime, NULL);

    double totalTime =
        (endTime.tv_sec - startTime.tv_sec) * 1000.0;

    totalTime +=
        (endTime.tv_usec - startTime.tv_usec) / 1000.0;

    // ========================================
    // FINAL OUTPUT
    // ========================================

    printf("\n");
    printf("====================================================\n");
    printf("              TRANSMISSION COMPLETE\n");
    printf("====================================================\n");

    printf(
        "Maximum Sequence Numbers : %d\n",
        MAX_SEQUENCE_NUMBERS
    );

    printf(
        "Total Packets            : %d\n",
        TOTAL_PACKETS
    );

    printf(
        "Window Size              : %d\n",
        WINDOW_SIZE
    );

    printf(
        "Propagation Time         : %d ms\n",
        propagationTime
    );

    printf(
        "Transmission Time        : %d ms\n",
        transmissionTime
    );

    printf(
        "Total Transmission Time  : %.3f ms\n",
        totalTime
    );

    printf("====================================================\n");

    close(sock);

    return 0;
}
