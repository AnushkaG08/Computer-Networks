#include "sliding_common.h"

int main()
{
    int sock;

    struct sockaddr_in receiver;
    struct sockaddr_in sender;

    Packet packet;
    AckPacket ack;

    int received[TOTAL_PACKETS];

    socklen_t senderLength = sizeof(sender);

    // ========================================
    // INITIALIZE ARRAY
    // ========================================

    for(int i = 0; i < TOTAL_PACKETS; i++)
    {
        received[i] = 0;
    }

    // ========================================
    // CREATE SOCKET
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
    receiver.sin_addr.s_addr = INADDR_ANY;
    receiver.sin_port = htons(PORT);

    // ========================================
    // BIND
    // ========================================

    if(bind(
        sock,
        (struct sockaddr *)&receiver,
        sizeof(receiver)) < 0)
    {
        perror("Bind failed");

        close(sock);

        return 1;
    }

    printf("\n");
    printf("====================================================\n");
    printf("           SLIDING WINDOW RECEIVER\n");
    printf("====================================================\n");

    printf(
        "Listening on port %d...\n",
        PORT
    );

    printf(
        "Sequence Number Bits : %d\n",
        SEQUENCE_BITS
    );

    printf(
        "Maximum Sequence No. : %d\n",
        MAX_SEQUENCE_NUMBERS
    );

    printf(
        "Window Size           : %d\n",
        WINDOW_SIZE
    );

    printf("\n");

    // ========================================
    // RECEIVE PACKETS
    // ========================================

    while(1)
    {
        int bytes = recvfrom(
            sock,
            &packet,
            sizeof(packet),
            0,
            (struct sockaddr *)&sender,
            &senderLength
        );

        if(bytes < 0)
        {
            perror("recvfrom failed");
            break;
        }

        // ====================================
        // END PACKET
        // ====================================

        if(packet.type == END_PACKET)
        {
            printf("\n");
            printf("END packet received.\n");
            printf("Receiver finished.\n");

            break;
        }

        // ====================================
        // RECEIVE DATA
        // ====================================

        printf(
            "Receiver <- Packet %d received | %s\n",
            packet.seqNo,
            packet.data
        );

        // ====================================
        // STORE IN ARRAY
        // ====================================

        if(packet.seqNo >= 0 &&
           packet.seqNo < TOTAL_PACKETS)
        {
            received[packet.seqNo] = 1;
        }

        // ====================================
        // DISPLAY RECEIVED ARRAY
        // ====================================

        printf("Received Array: ");

        for(int i = 0; i < TOTAL_PACKETS; i++)
        {
            if(received[i] == 1)
            {
                printf("[%d] ", i);
            }
            else
            {
                printf("[ ] ");
            }
        }

        printf("\n");

        // ====================================
        // SEND ACK
        // ====================================

        ack.ackNo = packet.seqNo;

        sendto(
            sock,
            &ack,
            sizeof(ack),
            0,
            (struct sockaddr *)&sender,
            senderLength
        );

        printf(
            "Receiver -> ACK %d sent\n\n",
            ack.ackNo
        );
    }

    // ========================================
    // CLOSE
    // ========================================

    close(sock);

    return 0;
}
