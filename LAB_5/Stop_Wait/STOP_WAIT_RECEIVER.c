#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080

int main()
{
    int sockfd;
    struct sockaddr_in receiver, sender;
    socklen_t len = sizeof(sender);

    char buffer[100];
    char ack[] = "ACK";

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
    receiver.sin_addr.s_addr = INADDR_ANY;

    /* Bind */
    if (bind(sockfd,
             (struct sockaddr *)&receiver,
             sizeof(receiver)) < 0)
    {
        perror("Bind failed");
        exit(1);
    }

    printf("Receiver is ready...\n");

    while (1)
    {
        recvfrom(sockfd,
                 buffer,
                 sizeof(buffer),
                 0,
                 (struct sockaddr *)&sender,
                 &len);

        printf("\nReceiver: %s received\n", buffer);

        /* Send ACK */
        sendto(sockfd,
               ack,
               strlen(ack) + 1,
               0,
               (struct sockaddr *)&sender,
               len);

        printf("Receiver: ACK sent\n");
    }

    close(sockfd);

    return 0;
}
