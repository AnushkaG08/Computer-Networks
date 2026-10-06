#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080

int main()
{
    int sockfd;
    struct sockaddr_in receiver;
    socklen_t len = sizeof(receiver);

    int n, i;
    float tt, tp;
    float start_time = 0;
    float transmission_end;
    float arrival_time;
    float ack_time;
    float total_time;

    char buffer[100];
    char ack[100];

    /* Create socket */
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    if (sockfd < 0)
    {
        perror("Socket creation failed");
        exit(1);
    }

    /* Receiver details */
    receiver.sin_family = AF_INET;
    receiver.sin_port = htons(PORT);
    receiver.sin_addr.s_addr = inet_addr("127.0.0.1");

    /* Input */
    printf("Enter Transmission Time (Tt): ");
    scanf("%f", &tt);

    printf("Enter Propagation Time (Tp): ");
    scanf("%f", &tp);

    printf("Enter Number of Packets: ");
    scanf("%d", &n);

    printf("\n========== STOP AND WAIT ==========\n");

    for (i = 1; i <= n; i++)
    {
        sprintf(buffer, "Packet %d", i);

        printf("\n-----------------------------------\n");
        printf("Packet %d\n", i);

        /* Start transmission */
        printf("Start Time          : %.2f\n", start_time);

        /* Actual transmission */
        transmission_end = start_time + tt;

        printf("Transmission Time   : %.2f\n", tt);
        printf("Transmission End    : %.2f\n", transmission_end);

        /* Send packet */
        sendto(sockfd, buffer, strlen(buffer) + 1, 0,
               (struct sockaddr *)&receiver,
               sizeof(receiver));

        /* Packet propagation */
        arrival_time = transmission_end + tp;

        printf("Receiver Arrival    : %.2f\n", arrival_time);

        /* Wait for ACK */
        recvfrom(sockfd, ack, sizeof(ack), 0,
                 (struct sockaddr *)&receiver,
                 &len);

        /* ACK propagation */
        ack_time = arrival_time + tp;

        printf("ACK Received Time   : %.2f\n", ack_time);

        printf("Packet %d Completed  : %.2f\n", i, ack_time);

        /* Next packet starts after ACK */
        start_time = ack_time;
    }

    total_time = start_time;

    printf("\n===================================\n");
    printf("Total Time = %.2f units\n", total_time);
    printf("===================================\n");

    close(sockfd);

    return 0;
}
